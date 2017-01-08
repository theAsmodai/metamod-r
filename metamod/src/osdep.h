#pragma once

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

#include "mreg.h"

#ifdef _WIN32
typedef HINSTANCE module_handle_t;
#else
typedef void* module_handle_t;
#endif

class CSysModule
{
public:
	CSysModule();
	module_handle_t load(const char* filename);
	bool unload();
	void* getsym(const char* name) const;
	module_handle_t gethandle() const;
	bool contain(void* addr) const;
	static const char* getloaderror();

private:
	module_handle_t m_handle;
	uintptr_t m_base;
	uintptr_t m_size;
};

// Windows doesn't have an strtok_r() routine, so we write our own.
#ifdef _WIN32
	#define strtok_r(s, delim, ptrptr)	mm_strtok_r(s, delim, ptrptr)
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

	const char *str_GetLastError();
#endif // _WIN32

const char* str_os_error();
