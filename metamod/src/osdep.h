#pragma once

// Various differences between WIN32 and Linux.
#include "types_meta.h"		// mBOOL
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

extern mBOOL dlclose_handle_invalid;

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
			dlclose_handle_invalid = mTRUE;
			return 1;
		}

		dlclose_handle_invalid = mFALSE;
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
			dlclose_handle_invalid = mTRUE;
			return 1;
		}
		dlclose_handle_invalid = mFALSE;
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
mBOOL IS_VALID_PTR(void *memptr);

// Attempt to call the given function pointer, without segfaulting.
mBOOL os_safe_call(REG_CMD_FN pfn);

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

// Our handler for new().
//
// Thanks to notes from:
//    http://dragon.klte.hu/~kollarl/C++/node45.html
//
// At one point it appeared MSVC++ was no longer different from gcc, according
// to:
//    http://msdn.microsoft.com/library/en-us/vclang98/stdlib/info/NEW.asp
//
// However, this page is apparently no longer available from MSDN.  The
// only thing now is:
//    http://msdn.microsoft.com/library/en-us/vccore98/HTML/_crt_malloc.asp
//
// According to Fritz Elfert <felfert@to.com>:
//    set_new_handler() is just a stub which (according to comments in the
//    MSVCRT debugging sources) should never be used. It is just an ugly
//    hack to make STL compile. It does _not_ set the new handler but
//    always calls _set_new_handler(0) instead. _set_new_handler is the
//    "real" function and uses the "old" semantic; handler-type is:
//       int newhandler(size_t)
//
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
	void __cdecl meta_new_handler();
#elif defined(_MSC_VER)
	int meta_new_handler(size_t size);
#else
	void meta_new_handler();
#endif

// To keep the rest of the sources clean and keep not only OS but also
// compiler dependant differences in this file, we define a local function
// to set the new handler.
void mm_set_new_handler( void );

// Thread handling...
#if defined(__linux) || defined(__APPLE__)
	#include <pthread.h>
	typedef	pthread_t THREAD_T;
	// returns 0==success, non-zero==failure
	inline int THREAD_CREATE(THREAD_T *tid, void (*func)())
	{
		int ret;
		ret = pthread_create(tid, NULL, (void *(*)(void*)) func, NULL);
		if (ret != 0)
		{
			META_ERROR("Failure starting thread: %s", strerror(ret));
			return ret;
		}

		ret = pthread_detach(*tid);
		if (ret != 0)
			META_ERROR("Failure detaching thread: %s", strerror(ret));
		return ret;
	}
#elif defined(_WIN32)
	// See:
	//    http://msdn.microsoft.com/library/en-us/dllproc/prothred_4084.asp
	typedef	DWORD THREAD_T;
	// returns 0==success, non-zero==failure
	inline int THREAD_CREATE(THREAD_T *tid, void (*func)())
	{
		HANDLE ret;
		// win32 returns NULL==failure, non-NULL==success
		ret = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, NULL, 0, tid);
		if (ret == NULL)
			META_ERROR("Failure starting thread: %s", str_GetLastError());
		return ret == NULL;
	}
#endif // _WIN32
#define THREAD_OK	0


// Mutex handling...
#if defined(__linux) || defined(__APPLE__)
	typedef pthread_mutex_t		MUTEX_T;
	inline int MUTEX_INIT(MUTEX_T *mutex)
	{
		int ret;
		ret = pthread_mutex_init(mutex, NULL);
		if (ret != THREAD_OK)
			META_ERROR("mutex_init failed: %s", strerror(ret));
		return ret;
	}
	inline int MUTEX_LOCK(MUTEX_T *mutex)
	{
		int ret;
		ret = pthread_mutex_lock(mutex);
		if (ret != THREAD_OK)
			META_ERROR("mutex_lock failed: %s", strerror(ret));
		return ret;
	}
	inline int MUTEX_UNLOCK(MUTEX_T *mutex)
	{
		int ret;
		ret = pthread_mutex_unlock(mutex);
		if (ret != THREAD_OK)
			META_ERROR("mutex_unlock failed: %s", strerror(ret));
		return ret;
	}
#elif defined(_WIN32)
	// Win32 has "mutexes" as well, but CS's are simpler.
	// See:
	//    http://msdn.microsoft.com/library/en-us/dllproc/synchro_2a2b.asp
	typedef CRITICAL_SECTION	MUTEX_T;
	// Note win32 routines don't return any error (return void).
	inline int MUTEX_INIT(MUTEX_T *mutex)
	{
		InitializeCriticalSection(mutex);
		return THREAD_OK;
	}
	inline int MUTEX_LOCK(MUTEX_T *mutex)
	{
		EnterCriticalSection(mutex);
		return THREAD_OK;
	}
	inline int MUTEX_UNLOCK(MUTEX_T *mutex)
	{
		LeaveCriticalSection(mutex);
		return THREAD_OK;
	}
#endif // _WIN32


// Condition variables...
#if defined(__linux) || defined(__APPLE__)
	typedef pthread_cond_t	COND_T;
	inline int COND_INIT(COND_T *cond)
	{
		int ret;
		ret = pthread_cond_init(cond, NULL);
		if (ret != THREAD_OK)
			META_ERROR("cond_init failed: %s", strerror(ret));
		return ret;
	}
	inline int COND_WAIT(COND_T *cond, MUTEX_T *mutex)
	{
		int ret;
		ret = pthread_cond_wait(cond, mutex);
		if (ret != THREAD_OK)
			META_ERROR("cond_wait failed: %s", strerror(ret));
		return ret;
	}
	inline int COND_SIGNAL(COND_T *cond)
	{
		int ret;
		ret = pthread_cond_signal(cond);
		if (ret != THREAD_OK)
			META_ERROR("cond_signal failed: %s", strerror(ret));
		return ret;
	}
#elif defined(_WIN32)
	// Since win32 doesn't provide condition-variables, we have to model
	// them with mutex/critical-sections and win32 events.  This uses the
	// second (SetEvent) solution from:
	//
	//    http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
	//
	// but without the waiters_count overhead, since we don't need
	// broadcast functionality anyway.  Or actually, I guess it's more like
	// the first (PulseEvent) solution, but with SetEven rather than
	// PulseEvent. :)
	//
	// See also:
	//    http://msdn.microsoft.com/library/en-us/dllproc/synchro_8ann.asp
	typedef HANDLE COND_T;
	inline int COND_INIT(COND_T *cond)
	{
		*cond = CreateEvent(NULL,	// security attributes (none)
							FALSE,	// manual-reset type (false==auto-reset)
							FALSE,	// initial state (unsignaled)
							NULL);	// object name (unnamed)
		// returns NULL on error
		if (*cond == NULL)
		{
			META_ERROR("cond_init failed: %s", str_GetLastError());
			return -1;
		}
		else
			return 0;
	}
	inline int COND_WAIT(COND_T *cond, MUTEX_T *mutex)
	{
		DWORD ret;
		LeaveCriticalSection(mutex);
		ret = WaitForSingleObject(*cond, INFINITE);
		EnterCriticalSection(mutex);
		// returns WAIT_OBJECT_0 if object was signaled; other return
		// values indicate errors.
		if (ret == WAIT_OBJECT_0)
			return 0;
		else
		{
			META_ERROR("cond_wait failed: %s", str_GetLastError());
			return -1;
		}
	}
	inline int COND_SIGNAL(COND_T *cond)
	{
		BOOL ret;
		ret = SetEvent(*cond);
		// returns zero on failure
		if (ret == 0)
		{
			META_ERROR("cond_signal failed: %s", str_GetLastError());
			return -1;
		}
		else
			return 0;
	}
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
