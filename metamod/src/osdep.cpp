#include "precompiled.h"

CSysModule::CSysModule() : m_handle(0), m_base(0), m_size(0)
{
}

#ifdef _WIN32
module_handle_t CSysModule::load(const char* filepath)
{
	m_handle = LoadLibrary(filepath);

	MODULEINFO module_info;
	if (GetModuleInformation(GetCurrentProcess(), m_handle, &module_info, sizeof(module_info))) {
		m_base = (uintptr_t)module_info.lpBaseOfDll;
		m_size = module_info.SizeOfImage;
	}

	return m_handle;
}

bool CSysModule::unload()
{
	bool ret = false;

	if (m_handle) {
		ret = FreeLibrary(m_handle) != ERROR;
		m_handle = 0;
		m_base = 0;
		m_size = 0;
	}

	return ret;
}

void* CSysModule::getsym(const char* name) const
{
	return GetProcAddress(m_handle, name);
}
#else
static ElfW(Addr) dlsize(void* base)
{
	ElfW(Ehdr) *ehdr;
	ElfW(Phdr) *phdr;
	ElfW(Addr) end;

	ehdr = (ElfW(Ehdr) *)base;

	/* Find the first program header */
	phdr = (ElfW(Phdr)*)((ElfW(Addr))ehdr + ehdr->e_phoff);

	/* Find the final PT_LOAD segment's extent */
	for (int i = 0; i < ehdr->e_phnum; ++i)
		if (phdr[i].p_type == PT_LOAD)
			end = phdr[i].p_vaddr + phdr[i].p_memsz;

	/* The start (virtual) address is always zero, so just return end.*/
	return end;
}

module_handle_t CSysModule::load(const char* filepath)
{
	m_handle = dlopen(filepath, RTLD_NOW);

	char buf[1024], dummy[1024], path[260];
	sprintf(buf, "/proc/%i/maps", getpid());

	FILE* fp = fopen(buf, "r");

	while (fgets(buf, sizeof(buf), fp)) {
		uintptr_t start, end;

		int args = sscanf(buf, "%x-%x %128s %128s %128s %128s %255s", &start, &end, dummy, dummy, dummy, dummy, path);
		if (args != 7) {
			continue;
		}

		if (!Q_stricmp(path, filepath)) {
			m_base = start;
			m_size = end - start;
			break;
		}
	}

	fclose(fp);
	return m_handle;
}

bool CSysModule::unload()
{
	bool ret = false;

	if (m_handle) {
		ret = dlclose(m_handle) != 0;
		m_handle = 0;
		m_base = 0;
		m_size = 0;
	}

	return ret;
}

void* CSysModule::getsym(const char* name) const
{
	return dlsym(m_handle, name);
}
#endif

module_handle_t CSysModule::gethandle() const
{
	return m_handle;
}

bool CSysModule::contain(void* addr) const
{
	return addr && uintptr_t(addr) > m_base && uintptr_t(addr) < m_base + m_size;
}

const char* CSysModule::getloaderror()
{
#ifdef _WIN32
	return str_GetLastError();
#else
	return dlerror;
#endif
}

#ifdef _WIN32
// Windows doesn't provide a functon analagous to dlerr() that returns a
// string describing the error, so we include one here, as exampled at:
// http://msdn.microsoft.com/library/en-us/debug/errors_0sdh.asp
// except without FORMAT_MESSAGE_ALLOCATE_BUFFER, since we use a local
// static buffer.
const char *str_GetLastError()
{
	static char buf[MAX_STRBUF_LEN];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buf, MAX_STRBUF_LEN - 1, NULL);
	return buf;
}

const char* str_os_error()
{
#ifdef _WIN32
	return str_GetLastError();
#else
	return strerror(errno);
#endif
}
#endif

// Determine whether the given memory location is valid (ie whether we
// should expect to be able to reference strings or functions at this
// location without segfaulting).
#if defined(linux) || defined(__APPLE__)
// Simulate this with dladdr.  I'm not convinced this will be as generally
// applicable as the native windows routine below, but it should do what
// we need it for in this particular situation.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching sharedlib for this ptr
bool IS_VALID_PTR(void *memptr)
{
	Dl_info dli;
	Q_memset(&dli, 0, sizeof(dli));
	if (dladdr(memptr, &dli))
		return true;
	else
		RETURN_ERRNO(false, ME_NOTFOUND);
}
#elif defined(_WIN32)
// Use the native windows routine IsBadCodePtr.
// meta_errno values:
//  - ME_BADMEMPTR	not a valid memory pointer
bool IS_VALID_PTR(void *memptr)
{
	if (IsBadCodePtr((FARPROC) memptr))
		RETURN_ERRNO(false, ME_BADMEMPTR);
	else
		return true;
}
#endif // _WIN32

// This used to be OS-dependent, as it used a SEGV signal handler under
// linux, but that was removed because (a) it masked legitimate segfaults
// in plugin commands and produced confusing output ("plugin has been
// unloaded", when really it segfaultd), and (b) wasn't necessary since
// IS_VALID_PTR() should cover the situation.
bool os_safe_call(REG_CMD_FN pfn)
{
	// try and see if this is a valid memory location
	if (!IS_VALID_PTR((void *)pfn))
		// meta_errno should be already set in is_valid_ptr()
		return false;

	pfn();
	return true;
}
