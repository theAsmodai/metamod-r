#pragma once

// Various differences between WIN32 and Linux.
#include "types_meta.h"		// bool
#include "mreg.h"		// REG_CMD_FN, etc
#include "log_meta.h"		// LOG_ERROR, etc

// String describing platform/DLL-type, for matching lines in plugins.ini.
#ifdef _WIN32
	#define UNUSED		/**/

	#define PLATFORM	"mswin"
	#define PLATFORM_SPC	"win32"
	#define PLATFORM_DLEXT	".dll"
#else
	#define UNUSED		__attribute__((unused))

	#define PLATFORM	"linux"
	#define PLATFORM_SPC	"lin32"
	#define PLATFORM_DLEXT	".so"
#endif

extern bool dlclose_handle_invalid;

// Functions & types for DLL open/close/etc operations.
#ifdef _WIN32
	typedef HINSTANCE DLHANDLE;
	typedef FARPROC DLFUNC;
	inline DLHANDLE DLOPEN(const char *filename)
	{
		return LoadLibraryA(filename);
	}
	inline DLFUNC DLSYM(DLHANDLE handle, const char *string)
	{
		return GetProcAddress(handle, string);
	}
	inline int DLCLOSE(DLHANDLE handle)
	{
		if (!handle)
		{
			dlclose_handle_invalid = true;
			return 1;
		}

		dlclose_handle_invalid = false;
		// NOTE: Windows FreeLibrary returns success=nonzero, fail=zero,
		// which is the opposite of the unix convention, thus the '!'.
		return !FreeLibrary(handle);
	}
	// Windows doesn't provide a function corresponding to dlerror(), so
	// we make our own.
	const char *str_GetLastError();
	inline const char *DLERROR()
	{
		if (dlclose_handle_invalid)
			return "Invalid handle.";

		return str_GetLastError();
	}
#else
	typedef void *DLHANDLE;
	typedef void *DLFUNC;
	inline DLHANDLE DLOPEN(const char *filename)
	{
		return dlopen(filename, RTLD_NOW);
	}
	inline DLFUNC DLSYM(DLHANDLE handle, const char *string)
	{
		return dlsym(handle, string);
	}
	// dlclose crashes if handle is null.
	inline int DLCLOSE(DLHANDLE handle)
	{
		if (!handle)
		{
			dlclose_handle_invalid = true;
			return 1;
		}
		dlclose_handle_invalid = false;
		return dlclose(handle);
	}
	inline const char *DLERROR()
	{
		if (dlclose_handle_invalid)
			return "Invalid handle.";

		return dlerror();
	}
#endif

const char *DLFNAME(void *memptr);
bool IS_VALID_PTR(void *memptr);

// Attempt to call the given function pointer, without segfaulting.
bool os_safe_call(REG_CMD_FN pfn);

// Windows doesn't have an strtok_r() routine, so we write our own.
#ifdef _WIN32
	#define strtok_r(s, delim, ptrptr)	my_strtok_r(s, delim, ptrptr)
	char *my_strtok_r(char *s, const char *delim, char **ptrptr);
#else
// Linux doesn't have an strlwr() routine, so we write our own.
	#define strlwr(s) my_strlwr(s)
	char *my_strlwr(char *s);
#endif // _WIN32


// Set filename and pathname maximum lengths.  Note some windows compilers
// provide a <limits.h> which is incomplete and/or causes problems; see
// doc/windows_notes.txt for more information.
//
// Note that both OS's include room for null-termination:
//   linux:    "# chars in a path name including nul"
//   win32:    "note that the sizes include space for 0-terminator"
#if defined(__linux) || defined(__APPLE__)
	#include <limits.h>
#elif defined(_WIN32)
	#include <stdlib.h>
	#define NAME_MAX	_MAX_FNAME
	#define PATH_MAX	_MAX_PATH
#endif // _WIN32

// Various other windows routine differences.
#if defined(__linux) || defined(__APPLE__)
	#include <unistd.h>	// sleep
	#ifndef O_BINARY
    	#define O_BINARY 0
	#endif
#elif defined(_WIN32)
	#define snprintf	_snprintf
	#define vsnprintf	_vsnprintf
	#define sleep(x)	Sleep(x*1000)
    #include <io.h>
    #define open _open
    #define read _read
    #define write _write
    #define close _close
#endif // _WIN32

#ifdef __GNUC__
	#include <unistd.h>	// _getcwd
#elif defined(_MSC_VER)
	#include <direct.h>	// _getcwd
#endif /* _MSC_VER */

#include <sys/stat.h>
#ifndef S_ISREG
	// Linux gcc defines this; earlier mingw didn't, later mingw does;
	// MSVC doesn't seem to.
	#define S_ISREG(m)	((m) & S_IFREG)
#endif /* not S_ISREG */
#ifdef _WIN32
	// The following two are defined in mingw but not in MSVC
    #ifndef S_IRUSR
        #define S_IRUSR _S_IREAD
    #endif
    #ifndef S_IWUSR
        #define S_IWUSR _S_IWRITE
    #endif

	// The following two are defined neither in mingw nor in MSVC
    #ifndef S_IRGRP
        #define S_IRGRP S_IRUSR
    #endif
    #ifndef S_IWGRP
        #define S_IWGRP S_IWUSR
    #endif
#endif // _WIN32

// Normalize/standardize a pathname.
//  - For win32, this involves:
//    - Turning backslashes (\) into slashes (/), so that config files and
//      Metamod internal code can be simpler and just use slashes (/).
//    - Turning upper/mixed case into lowercase, since windows is
//      non-case-sensitive.
//  - For linux, this requires no work, as paths uses slashes (/) natively,
//    and pathnames are case-sensitive.
#if defined(__linux) || defined(__APPLE__)
#define normalize_pathname(a)
#elif defined(_WIN32)
inline void normalize_pathname(char *path)
{
	char *cp;

	META_DEBUG(8, ("normalize: %s", path));
	for (cp = path; *cp; cp++)
	{
		if (isupper(*cp)) *cp = tolower(*cp);
		if (*cp == '\\') *cp = '/';
	}

	META_DEBUG(8, ("normalized: %s", path));
}
#endif // _WIN32

// Indicate if pathname appears to be an absolute-path.  Under linux this
// is a leading slash (/).  Under win32, this can be:
//  - a drive-letter path (ie "D:blah" or "C:\blah")
//  - a toplevel path (ie "\blah")
//  - a UNC network address (ie "\\srv1\blah").
// Also, handle both native and normalized pathnames.
inline int is_absolute_path(const char *path)
{
	if (path[0] == '/') return TRUE;
#ifdef _WIN32
	if (path[1] == ':') return TRUE;
	if (path[0] == '\\') return TRUE;
#endif // _WIN32
	return FALSE;
}

#ifdef _WIN32
// Buffer pointed to by resolved_name is assumed to be able to store a
// string of PATH_MAX length.
inline char *realpath(const char *file_name, char *resolved_name)
{
	int ret;
	ret = GetFullPathName(file_name, PATH_MAX, resolved_name, NULL);
	if (ret > PATH_MAX)
	{
		errno = ENAMETOOLONG;
		return NULL;
	}
	else if (ret > 0)
	{
		HANDLE handle;
		WIN32_FIND_DATA find_data;
		handle = FindFirstFile(resolved_name, &find_data);
		if (INVALID_HANDLE_VALUE == handle)
		{
			errno = ENOENT;
			return NULL;
		}

		FindClose(handle);
		normalize_pathname(resolved_name);
		return resolved_name;
	}

	return NULL;
}
#endif // _WIN32

// Generic "error string" from a recent OS call.  For linux, this is based
// on errno.  For win32, it's based on GetLastError.
inline const char *str_os_error()
{
#ifdef _WIN32
	return str_GetLastError();
#else
	return strerror(errno);
#endif
}
