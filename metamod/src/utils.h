#pragma once

// Turn a variable/function name into the corresponding string, optionally
// stripping off the leading "len" characters.  Useful for things like
// turning 'pfnClientCommand' into "ClientCommand" so we don't have to
// specify strings used for all the debugging/log messages.
#define STRINGIZE(name, len)		#name+len

// Max description length for plugins.ini and other places.
#define MAX_DESC_LEN 256

// For various character string buffers.
#define MAX_STRBUF_LEN 1024

template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

bool is_yes(const char* str);
bool is_no(const char* str);

const char* LOCALINFO(char* key);

#ifdef _WIN32
char *mm_strtok_r(char *s, const char *delim, char **ptrptr);
char *realpath(const char *file_name, char *resolved_name);
#endif // _WIN32

char* trimbuf(char *str);
void normalize_path(char *path);
bool is_abs_path(const char *path);
bool is_valid_path(const char *path);
bool is_platform_postfix(const char *pf);
bool is_file_exists_in_gamedir(const char *path);
char *full_gamedir_path(const char *path, char *fullpath);
bool mem_compare(const char* addr, const char* pattern, size_t len);
void NORETURN Sys_Error(const char *error, ...);

extern const char* g_platform_postfixes[4];
