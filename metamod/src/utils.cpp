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

char* trimbuf(char* str)
{
	char* ibuf;

	if (str == nullptr) return nullptr;
	for (ibuf = str; *ibuf && (byte)(*ibuf) < (byte)0x80 && isspace(*ibuf); ++ibuf)
		;

	int i = Q_strlen(ibuf);
	if (str != ibuf)
		Q_memmove(str, ibuf, i);

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
	struct stat64 st;
	return !stat64(path, &st) && S_ISREG(st.st_mode);
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
	int ret = GetFullPathName(file_name, MAX_PATH, resolved_name, nullptr);
	if (ret > MAX_PATH) {
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

bool is_file_exists(const char* file)
{
	struct stat64 st;
	int ret = stat64(file, &st);
	if (ret != 0) {
		META_DEBUG(5, "Unable to stat '%s': %s", file, strerror(errno));
		return false;
	}

	int reg = S_ISREG(st.st_mode);
	if (!reg) {
		META_DEBUG(5, "Not a regular file: %s", file);
		return false;
	}

	if (!st.st_size) {
		META_DEBUG(5, "Empty file: %s", file);
		return false;
	}

	return true;
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
	char buf[MAX_PATH];

	if (!path)
		return false;

	if (!Q_strcmp(path, "/dev/null"))
		return true;

	if (is_abs_path(path)) {
		Q_strlcpy(buf, path);
	}
	else {
		Q_snprintf(buf, sizeof buf, "%s/%s", g_GameDLL.gamedir, path);
	}

	return is_file_exists(buf);
}

// Turns path into a full path:
//  - if not absolute, prepends gamedir
//  - calls realpath() to collapse ".." and such
//  - calls NormalizePath() to fix backslashes, etc
//
// Much like realpath, buffer pointed to by fullpath is assumed to be
// able to store a string of MAX_PATH length.
char* full_gamedir_path(const char* path, char (&fullpath)[MAX_PATH])
{
	char buf[MAX_PATH];

	// Build pathname from filename, plus gamedir if relative path.
	if (is_abs_path(path)) {
		Q_strlcpy(buf, path);
	}
	else {
		Q_snprintf(buf, sizeof buf, "%s/%s", g_GameDLL.gamedir, path);
	}

	// Remove relative path components, if possible.
	if (!realpath(buf, fullpath)) {
		META_DEBUG(4, "Unable to get realpath for '%s': %s", buf,  strerror(errno));
		Q_strlcpy(fullpath, path);
	}

	// Replace backslashes, etc.
	normalize_path(fullpath);
	return fullpath;
}

void NORETURN Sys_Error(const char *error, ...)
{
	va_list argptr;
	static char text[1024];

	va_start(argptr, error);
	Q_vsnprintf(text, sizeof(text), error, argptr);
	va_end(argptr);

	META_CONS("FATAL ERROR (shutting down): %s\n", text);
	META_ERROR(text);

#ifdef _WIN32
	MessageBox(GetForegroundWindow(), text, "Fatal error - Metamod", MB_ICONERROR | MB_OK);
#endif // _WIN32

	// Allow chance to read the message, before any window closes.
	sleep(3);

	int *null = nullptr;
	*null = 0;
	exit(-1);
}
