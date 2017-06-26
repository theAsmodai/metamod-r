#include "precompiled.h"

const char* g_platform_postfixes[4] =
{
	"_i386.so",
	"_i486.so",
	"_i586.so",
	"_i686.so",
};

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
	return ENTITY_KEYVALUE(nullptr, key);
}

static_allocator::static_allocator(memory_protection protection) : m_protection(protection)
{
}

char* static_allocator::allocate(const size_t n)
{
	if (!m_pages.size() || m_used + n > Pagesize)
		allocate_page();

	auto ptr = reinterpret_cast<char *>(m_pages.back()) + m_used;
	m_used += n;
	return ptr;
}

char* static_allocator::strdup(const char* string)
{
	size_t len = strlen(string) + 1;
	return (char *)memcpy(allocate(len), string, len);
}

void static_allocator::deallocate_all()
{
	for (auto page : m_pages)
#ifdef WIN32
		VirtualFree(page, 0, MEM_RELEASE);
#else
		munmap(page, Pagesize);
#endif

	m_pages.clear();
}

size_t static_allocator::memory_used() const
{
	return (m_pages.size() - 1) * Pagesize + m_used;
}

bool static_allocator::contain(uint32 addr)
{
	for (auto p : m_pages) {
		if (uint32(p) <= addr && addr < uint32(p) + Pagesize)
			return true;
	}
	return false;
}

char* static_allocator::find_pattern(char* pattern, size_t len)
{
	for (auto p : m_pages) {
		for (char* c = (char *)p, *e = c + Pagesize - len; c < e; c++) {
			if (mem_compare(c, pattern, len))
				return c;
		}
	}
	return nullptr;
}

void static_allocator::allocate_page()
{
#ifdef WIN32
	auto page = VirtualAlloc(nullptr, Pagesize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	auto page = mmap(nullptr, Pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
#endif

	m_used = 0;
	m_pages.push_back(page);
}

#ifdef _WIN32
// Since windows doesn't provide a verison of strtok_r(), we include one
// here.  This may or may not operate exactly like strtok_r(), but does
// what we need it it do.
char* mm_strtok_r(char* s, const char* delim, char** ptrptr)
{
	char* begin = nullptr;
	char* end = nullptr;
	char* rest = nullptr;
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

char* trimbuf(char* str)
{
	char* ibuf;

	if (str == nullptr) return nullptr;
	for (ibuf = str; *ibuf && (byte)(*ibuf) < (byte)0x80 && isspace(*ibuf); ++ibuf)
		;

	int i = strlen(ibuf);
	if (str != ibuf)
		memmove(str, ibuf, i);

	while (--i >= 0) {
		if (!isspace(str[i]))
			break;
	}
	str[i + 1] = '\0';
	return str;
}

void normalize_path(char* path)
{
#ifdef _WIN32
	for (char* cp = path; *cp; cp++) {
		if (isupper(*cp))
			*cp = tolower(*cp);

		if (*cp == '\\')
			*cp = '/';
	}
#endif
}

bool is_abs_path(const char* path)
{
	if (path[0] == '/') return true;
#ifdef _WIN32
	if (path[1] == ':') return true;
	if (path[0] == '\\') return true;
#endif // _WIN32
	return false;
}

bool is_valid_path(const char* path)
{
	struct stat st;
	return !stat(path, &st) && S_ISREG(st.st_mode);
}

bool is_platform_postfix(const char* pf)
{
	if (pf) {
		for (size_t i = 0; i < arraysize(g_platform_postfixes); i++) {
			if (!Q_strcmp(pf, g_platform_postfixes[i]))
				return true;
		}
	}
	return false;
}

#ifdef _WIN32
char* realpath(const char* file_name, char* resolved_name)
{
	int ret = GetFullPathName(file_name, PATH_MAX, resolved_name, nullptr);
	if (ret > PATH_MAX) {
		errno = ENAMETOOLONG;
		return nullptr;
	}

	if (ret > 0) {
		WIN32_FIND_DATA find_data;
		HANDLE handle = FindFirstFile(resolved_name, &find_data);
		if (INVALID_HANDLE_VALUE == handle) {
			errno = ENOENT;
			return nullptr;
		}

		FindClose(handle);
		normalize_path(resolved_name);
		return resolved_name;
	}

	return nullptr;
}
#endif // _WIN32

void __declspec(noreturn) do_exit(int exitval)
{
	//Allahu Akbar!!
	*((int *)nullptr) = 0;
}

// Checks for a non-empty file, relative to the gamedir if necessary.
// Formerly used LOAD_FILE_FOR_ME, which provided a simple way to check for
// a file under the gamedir, but which would _also_ look in the sibling
// "valve" directory, thus sometimes finding files that weren't desired.
// Also, formerly named just "valid_file".
//
// Special-case-recognize "/dev/null" as a valid file.
bool is_file_exists_in_gamedir(const char* path)
{
	char buf[PATH_MAX];

	if (!path)
		return false;

	if (!Q_strcmp(path, "/dev/null"))
		return true;

	if (is_abs_path(path)) {
		Q_strncpy(buf, path, sizeof buf);
		buf[sizeof buf - 1] = '\0';
	}
	else
		snprintf(buf, sizeof buf, "%s/%s", g_GameDLL.gamedir, path);

	struct stat st;
	int ret = stat(buf, &st);
	if (ret != 0) {
		META_DEBUG(5, "Unable to stat '%s': %s", buf, strerror(errno));
		return false;
	}

	int reg = S_ISREG(st.st_mode);
	if (!reg) {
		META_DEBUG(5, "Not a regular file: %s", buf);
		return false;
	}

	if (!st.st_size) {
		META_DEBUG(5, "Empty file: %s", buf);
		return false;
	}

	if (ret == 0 && reg)
		return true;

	return false;
}

// Turns path into a full path:
//  - if not absolute, prepends gamedir
//  - calls realpath() to collapse ".." and such
//  - calls NormalizePath() to fix backslashes, etc
//
// Much like realpath, buffer pointed to by fullpath is assumed to be
// able to store a string of PATH_MAX length.
char* full_gamedir_path(const char* path, char* fullpath)
{
	char buf[PATH_MAX];

	// Build pathname from filename, plus gamedir if relative path.
	if (is_abs_path(path)) {
		Q_strncpy(buf, path, sizeof buf - 1);
		buf[sizeof buf - 1] = '\0';
	}
	else snprintf(buf, sizeof buf, "%s/%s", g_GameDLL.gamedir, path);

	// Remove relative path components, if possible.
	if (!realpath(buf, fullpath)) {
		META_DEBUG(4, "Unable to get realpath for '%s': %s", buf, str_os_error());
		Q_strncpy(fullpath, path, sizeof fullpath - 1);
		fullpath[sizeof fullpath - 1] = '\0';
	}

	// Replace backslashes, etc.
	normalize_path(fullpath);
	return fullpath;
}

bool mem_compare(const char* addr, const char* pattern, size_t len)
{
	for (auto c = pattern, pattern_end = pattern + len; c < pattern_end; ++c, ++addr) {
		if (*c == *addr || *c == '\x2A')
			continue;
		return false;
	}
	return true;
}
