#include "precompiled.h"

bool is_yes(const char* str)
{
	return !Q_strcmp(str, "true") || !Q_strcmp(str, "yes") || !Q_strcmp(str, "1");
}

bool is_no(const char* str)
{
	return !Q_strcmp(str, "false") || !Q_strcmp(str, "no") || !Q_strcmp(str, "0");
}

char* ENTITY_KEYVALUE(edict_t* entity, char* key)
{
	return INFOKEY_VALUE(GET_INFOKEYBUFFER(entity), key);
}

const char* LOCALINFO(char* key)
{
	return ENTITY_KEYVALUE(NULL, key);
}

char* execmem_allocator::allocate(const size_t n)
{
	if (!m_pages.size() || m_used + n > Pagesize)
		allocate_page();

	auto ptr = reinterpret_cast<char *>(m_pages.back()) + m_used;
	m_used += n;
	return ptr;
}

void execmem_allocator::deallocate_all()
{
	for (auto page : m_pages)
#ifdef WIN32
		VirtualFree(page, 0, MEM_RELEASE);
#else
		munmap(page, Pagesize);
#endif

	m_pages.clear();
}

size_t execmem_allocator::memoryUsed() const
{
	return (m_pages.size() - 1) * Pagesize + m_used;
}

void execmem_allocator::allocate_page()
{
#ifdef WIN32
	auto page = VirtualAlloc(NULL, Pagesize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	auto page = mmap(NULL, Pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
#endif

	m_used = 0;
	m_pages.push_back(page);
}

#ifdef _WIN32
// Since windows doesn't provide a verison of strtok_r(), we include one
// here.  This may or may not operate exactly like strtok_r(), but does
// what we need it it do.
char *mm_strtok_r(char *s, const char *delim, char **ptrptr)
{
	char *begin = nullptr;
	char *end = nullptr;
	char *rest = nullptr;
	if (s)
		begin = s;
	else
		begin = *ptrptr;
	if (!begin)
		return nullptr;

	end = strpbrk(begin, delim);
	if (end) {
		*end = '\0';
		rest = end + 1;
		*ptrptr = rest + strspn(rest, delim);
	}
	else
		*ptrptr = nullptr;

	return begin;
}
#endif // _WIN32

void normalize_pathname(char *path)
{
#ifdef _WIN32
	char *cp;

	META_DEBUG(8, ("normalize: %s", path));
	for (cp = path; *cp; cp++) {
		if (isupper(*cp)) *cp = tolower(*cp);
		if (*cp == '\\') *cp = '/';
	}

	META_DEBUG(8, ("normalized: %s", path));
#endif
}

bool is_absolute_path(const char *path)
{
	if (path[0] == '/') return true;
#ifdef _WIN32
	if (path[1] == ':') return true;
	if (path[0] == '\\') return true;
#endif // _WIN32
	return false;
}

char *realpath(const char *file_name, char *resolved_name)
{
	int ret = GetFullPathName(file_name, PATH_MAX, resolved_name, NULL);
	
	if (ret > PATH_MAX) {
		errno = ENAMETOOLONG;
		return NULL;
	}
	
	if (ret > 0) {
		HANDLE handle;
		WIN32_FIND_DATA find_data;
		handle = FindFirstFile(resolved_name, &find_data);
		if (INVALID_HANDLE_VALUE == handle) {
			errno = ENOENT;
			return NULL;
		}

		FindClose(handle);
		normalize_pathname(resolved_name);
		return resolved_name;
	}

	return NULL;
}
