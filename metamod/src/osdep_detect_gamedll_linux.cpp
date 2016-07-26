#include "precompiled.h"

/*
 * GLIBC 2.11+ intercept longjmp with __longjmp_chk. However we want
 * binary compability with older versions of GLIBC.
 */
#ifdef __amd64__
	__asm__(".symver __longjmp_chk,longjmp@GLIBC_2.2.5");
#else
	__asm__(".symver __longjmp_chk,longjmp@GLIBC_2.0");
#endif /*__amd64__*/

// On linux manually search for exports from dynamic library file.
//  --Jussi Kivilinna
static jmp_buf signal_jmp_buf;

// Signal handler for is_gamedll()
static void signal_handler_sigsegv(int) {
	longjmp(signal_jmp_buf, 1);
}

#define invalid_elf_ptr(x) (((unsigned long)&(x)) > file_end - 1)
#define invalid_elf_offset(x) (((unsigned long)(x)) > filesize - 1)
#define elf_error_exit() \
	do { \
		sigaction(SIGSEGV, &oldaction, 0); \
		META_DEBUG(3, ("is_gamedll(%s): Invalid ELF.", filename)); \
		munmap(ehdr, filesize); \
		return mFALSE; \
	} while (0)

mBOOL is_gamedll(const char *filename) {
	// When these are not static there are some mysterious hidden bugs that I can't find/solve.
	// So this is simple workaround.
	static struct sigaction action;
	static struct sigaction oldaction;
	static ElfW(Ehdr)  * ehdr = 0;
	static ElfW(Shdr)  * shdr = 0;
	static ElfW(Sym)   * symtab = 0;
	static char        * strtab = 0;
	static FILE        * pf = 0;
	static unsigned long filesize = 0;
	static unsigned long strtab_size = 0;
	static unsigned long nsyms = 0;
	static unsigned long i = 0;
	static unsigned long file_end = 0;
	static char        * funcname = 0;
	static int           has_GiveFnptrsToDll = 0;
	static int           has_GetEntityAPI2 = 0;
	static int           has_GetEntityAPI = 0;

	ehdr = 0;
	shdr = 0;
	symtab = 0;
	strtab = 0;
	pf = 0;
	filesize = 0;
	file_end = 0;
	strtab_size = 0;
	nsyms = 0;
	i = 0;
	funcname = 0;
	has_GiveFnptrsToDll = 0;
	has_GetEntityAPI2 = 0;
	has_GetEntityAPI = 0;

	// Try open file and get filesize
	if ((pf = fopen(filename, "rb"))) {
		fseek(pf, 0, SEEK_END);
		filesize = ftell(pf);
		fseek(pf, 0, SEEK_SET);
	} else {
		META_DEBUG(3, ("is_gamedll(%s): Failed, cannot fopen() file.", filename));

		return mFALSE;
	}

	// Check that filesize is atleast size of ELF header!
	if (filesize < sizeof(ElfW(Ehdr)))
	{
		META_DEBUG(3, ("is_gamedll(%s): Failed, file is too small to be ELF32. [%i < %i]", filename, filesize, sizeof(ElfW(Ehdr))));
		fclose(pf);

		return mFALSE;
	}

	// mmap library for easy reading
	ehdr = (ElfW(Ehdr) *)mmap(0, filesize, PROT_READ|PROT_WRITE, MAP_PRIVATE, fileno(pf), 0);
	file_end = (unsigned long)ehdr + filesize;

	// not needed anymore
	fclose(pf);

	// check if mmap was successful
	if (!ehdr || (void*)ehdr==(void*)-1) {
		META_DEBUG(3, ("is_gamedll(%s): Failed, mmap() [0x%x]", filename, ehdr));

		return mFALSE;
	}

	//In case that ELF file is incomplete (because bad upload etc), we protect memory-mapping access with signal-handler
	if (!setjmp(signal_jmp_buf)) {
		Q_memset(&action, 0, sizeof(struct sigaction));
		Q_memset(&oldaction, 0, sizeof(struct sigaction));

		// Not returning from signal, set SIGSEGV handler.
		action.sa_handler = signal_handler_sigsegv;
		action.sa_flags = SA_RESETHAND | SA_NODEFER;
		sigemptyset(&action.sa_mask);
		sigaction(SIGSEGV, &action, &oldaction);
	} else {
		// Reset signal handler.
		sigaction(SIGSEGV, &oldaction, 0);

		META_DEBUG(3, ("is_gamedll(%s): Failed, signal SIGSEGV.", filename));

		munmap(ehdr, filesize);

		return mFALSE;
	}

	if (Q_strncmp((char *)ehdr, ELFMAG, SELFMAG) != 0 || ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
		// Reset signal handler.
		sigaction(SIGSEGV, &oldaction, 0);

		META_DEBUG(3, ("is_gamedll(%s): Failed, file isn't ELF (%02x%02x%02x%02x:%x).", filename, ((char *)ehdr)[0], ((char *)ehdr)[1], ((char *)ehdr)[2], ((char *)ehdr)[3], ehdr->e_ident[EI_VERSION]));

		munmap(ehdr, filesize);
		return mFALSE;
	}

	// check if x86-shared-library
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 || ehdr->e_type != ET_DYN || ehdr->e_machine != EM_386)
	{
		// Reset signal handler.
		sigaction(SIGSEGV, &oldaction, 0);

		META_DEBUG(3, ("is_gamedll(%s): Failed, ELF isn't for target:i386. [%x:%x:%x]", filename, ehdr->e_ident[EI_CLASS], ehdr->e_type, ehdr->e_machine));
		munmap(ehdr, filesize);
		return mFALSE;
	}

	//Get symtab and strtab info
	shdr = (ElfW(Shdr) *)((char *)ehdr + ehdr->e_shoff);
	if (invalid_elf_ptr(shdr[ehdr->e_shnum]))
		elf_error_exit();

	for (i = 0; i < ehdr->e_shnum; i++) {
		// searching for dynamic linker symbol table
		if (shdr[i].sh_type == SHT_DYNSYM) {
			if (invalid_elf_offset(shdr[i].sh_offset) ||
			   invalid_elf_ptr(shdr[shdr[i].sh_link]) ||
			   invalid_elf_offset(shdr[shdr[i].sh_link].sh_offset) ||
			   invalid_elf_ptr(strtab[strtab_size]) ||
			   invalid_elf_ptr(symtab[nsyms]))
				elf_error_exit();

			symtab      = (ElfW(Sym) *)((char *)ehdr + shdr[i].sh_offset);
			strtab      = (char *)((char *)ehdr + shdr[shdr[i].sh_link].sh_offset);
			strtab_size = shdr[shdr[i].sh_link].sh_size;
			nsyms       = shdr[i].sh_size / shdr[i].sh_entsize;

			break;
		}
	}

	if (!symtab) {
		//Another method for finding symtab
		for (i = 0; i < ehdr->e_shnum; i++) {
			if (shdr[i].sh_type == SHT_SYMTAB) {
				if (invalid_elf_offset(shdr[i].sh_offset) ||
				   invalid_elf_ptr(shdr[shdr[i].sh_link]) ||
				   invalid_elf_offset(shdr[shdr[i].sh_link].sh_offset) ||
				   invalid_elf_ptr(strtab[strtab_size]) ||
				   invalid_elf_ptr(symtab[nsyms]))
					elf_error_exit();

				symtab      = (ElfW(Sym) *)((char *)ehdr + shdr[i].sh_offset);
				strtab      = (char *)((char *)ehdr + shdr[shdr[i].sh_link].sh_offset);
				strtab_size = shdr[shdr[i].sh_link].sh_size;
				nsyms       = shdr[i].sh_size / shdr[i].sh_entsize;

				break;
			}
		}
	}

	if (!symtab) {
		// Reset signal handler.
		sigaction(SIGSEGV, &oldaction, 0);
		META_DEBUG(3, ("is_gamedll(%s): Failed, couldn't locate symtab.", filename));
		munmap(ehdr, filesize);
		return mFALSE;
	}

	//Search symbols for exports
	for (i = 0; i < nsyms; i++)
	{
		// Export?
		if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC || ELF32_ST_BIND(symtab[i].st_info) != STB_GLOBAL)
			continue;

		// string outside strtab?
		if (symtab[i].st_name <= 0 || symtab[i].st_name >= strtab_size)
			continue;

		funcname = &strtab[symtab[i].st_name];

		// Check
		// Fast check for 'G' first
		if (funcname[0] == 'G') {
			// Collect export information
			if (!has_GiveFnptrsToDll)
				has_GiveFnptrsToDll = !Q_strcmp(funcname, "GiveFnptrsToDll");
			if (!has_GetEntityAPI2)
				has_GetEntityAPI2   = !Q_strcmp(funcname, "GetEntityAPI2");
	  		if (!has_GetEntityAPI)
	  			has_GetEntityAPI    = !Q_strcmp(funcname, "GetEntityAPI");
	  	}
		// Check if metamod plugin
		else if (funcname[0] == 'M')
		{
			if (!Q_strcmp(funcname, "Meta_Init") || !Q_strcmp(funcname, "Meta_Query") || !Q_strcmp(funcname, "Meta_Attach") || !Q_strcmp(funcname, "Meta_Detach"))
			{
				// Metamod plugin.. is not gamedll
				META_DEBUG(5, ("is_gamedll(%s): Detected Metamod plugin, library exports [%s].", filename, funcname));

				// Reset signal handler.
				sigaction(SIGSEGV, &oldaction, 0);
				munmap(ehdr, filesize);
				return mFALSE;
			}
		}
	}

	// Check if gamedll
	if (has_GiveFnptrsToDll && (has_GetEntityAPI2 || has_GetEntityAPI)) {
		// This is gamedll!
		META_DEBUG(5, ("is_gamedll(%s): Detected GameDLL.", filename));

		// Reset signal handler.
		sigaction(SIGSEGV, &oldaction, 0);
		munmap(ehdr, filesize);
		return mTRUE;
	} else {
		META_DEBUG(5, ("is_gamedll(%s): Library isn't GameDLL.", filename));
	}

	// Reset signal handler.
	sigaction(SIGSEGV, &oldaction, 0);
	munmap(ehdr, filesize);
	return mFALSE;
}
