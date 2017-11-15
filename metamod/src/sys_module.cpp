#include "precompiled.h"

const module_handle_t CSysModule::INVALID_HANDLE = (module_handle_t)0;

CSysModule::CSysModule() : m_handle(INVALID_HANDLE), m_base(0), m_size(0), m_free(true)
{
}

bool CSysModule::is_opened() const
{
	return m_handle != INVALID_HANDLE;
}

char *CSysModule::find_string_push(const char *string)
{
	return mem_find_string_push((char *)getbase(), string, getsize());
}

char *CSysModule::find_pattern(char *pos, int range, const char *pattern, size_t len)
{
	for (auto c = pos + range - len; pos < c; ++pos) {
		if (mem_compare(pos, pattern, len))
			return pos;
	}

	return nullptr;
}

#ifdef _WIN32

module_handle_t CSysModule::load(void *addr)
{
	MEMORY_BASIC_INFORMATION mem;
	VirtualQuery(addr, &mem, sizeof(mem));

	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)mem.AllocationBase;
	IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS *)((uintptr_t)dos + (uintptr_t)dos->e_lfanew);

	if (pe->Signature != IMAGE_NT_SIGNATURE)
		return INVALID_HANDLE;

	m_free = false;
	m_base = (uintptr_t)mem.AllocationBase;
	m_size = (size_t)pe->OptionalHeader.SizeOfImage;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>(addr), &m_handle);
	return m_handle;
}

module_handle_t CSysModule::load(const char *filepath)
{
	if (!m_handle) {
		m_handle = LoadLibrary(filepath);

		MODULEINFO module_info;
		if (GetModuleInformation(GetCurrentProcess(), m_handle, &module_info, sizeof(module_info))) {
			m_base = (uintptr_t)module_info.lpBaseOfDll;
			m_size = module_info.SizeOfImage;
		}
	}

	return m_handle;
}

bool CSysModule::unload()
{
	if (m_handle == INVALID_HANDLE) {
		return false;
	}

	bool ret = true;
	if (m_free) {
		ret = FreeLibrary(m_handle) != ERROR;
	}

	m_handle = INVALID_HANDLE;
	m_base = 0;
	m_size = 0;

	return ret;
}

void *CSysModule::getsym(const char *name) const
{
	return m_handle ? GetProcAddress(m_handle, name) : nullptr;
}
#else

static ElfW(Addr) dlsize(void *base)
{
	ElfW(Ehdr) *ehdr;
	ElfW(Phdr) *phdr;
	ElfW(Addr) end;

	ehdr = (ElfW(Ehdr) *)base;

	// Find the first program header
	phdr = (ElfW(Phdr)*)((ElfW(Addr))ehdr + ehdr->e_phoff);

	// Find the final PT_LOAD segment's extent
	for (int i = 0; i < ehdr->e_phnum; ++i)
		if (phdr[i].p_type == PT_LOAD)
			end = phdr[i].p_vaddr + phdr[i].p_memsz;

	// The start (virtual) address is always zero, so just return end.
	return end;
}

module_handle_t CSysModule::load(void *addr)
{
	Dl_info dlinfo;
	if ((!dladdr(addr, &dlinfo) && !dlinfo.dli_fbase) || !dlinfo.dli_fname) {
		return INVALID_HANDLE;
	}

	m_free = false;
	m_base = (uintptr_t)dlinfo.dli_fbase;
	m_size = (size_t)dlsize(dlinfo.dli_fbase);

	m_handle = dlopen(dlinfo.dli_fname, RTLD_NOW | RTLD_NOLOAD);
	return m_handle;
}

module_handle_t CSysModule::load(const char *filepath)
{
	if (!m_handle) {
		m_handle = dlopen(filepath, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);

		char buf[1024], dummy[1024], path[260];
		sprintf(buf, "/proc/%i/maps", getpid());

		FILE* fp = fopen(buf, "r");

		while (fgets(buf, sizeof buf, fp)) {
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
	}

	return m_handle;
}

bool CSysModule::unload()
{
	if (m_handle == INVALID_HANDLE) {
		return false;
	}

	bool ret = true;
	if (m_free) {
		ret = dlclose(m_handle) != 0;
	}

	m_handle = INVALID_HANDLE;
	m_base = 0;
	m_size = 0;

	return ret;
}

void* CSysModule::getsym(const char *name) const
{
	return m_handle ? dlsym(m_handle, name) : nullptr;
}
#endif

module_handle_t CSysModule::gethandle() const
{
	return m_handle;
}

uintptr_t CSysModule::getbase() const
{
	return m_base;
}

size_t CSysModule::getsize() const
{
	return m_size;
}

bool CSysModule::contain(void *addr) const
{
	return addr && uintptr_t(addr) >= m_base && uintptr_t(addr) < m_base + m_size;
}

const char *CSysModule::getloaderror()
{
#ifdef _WIN32
	static char buf[1024];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buf, sizeof(buf) - 1, nullptr);
	return buf;
#else
	return dlerror();
#endif
}
