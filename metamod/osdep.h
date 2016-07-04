// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// osdep.h - operating system dependencies

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef OSDEP_H
#define OSDEP_H

#include <string.h>			// strerror()
#include <ctype.h>			// isupper, tolower
#include <errno.h>			// errno

// Various differences between WIN32 and Linux.

#include "comp_dep.h"
#include "types_meta.h"		// mBOOL
#include "mreg.h"			// REG_CMD_FN, etc
#include "log_meta.h"		// LOG_ERROR, etc

// String describing platform/DLL-type, for matching lines in plugins.ini.
#ifdef linux
	#define PLATFORM	"linux"
#  if defined(__x86_64__) || defined(__amd64__)
	#define PLATFORM_SPC	"lin64"
#  else
	#define PLATFORM_SPC	"lin32"
#  endif
	#define PLATFORM_DLEXT	".so"
#elif defined(_WIN32)
	#define PLATFORM	"mswin"
	#define PLATFORM_SPC	"win32"
	#define PLATFORM_DLEXT	".dll"
#else /* unknown */
	#error "OS unrecognized"
#endif /* unknown */

// Macro for function-exporting from DLL..
// from SDK dlls/cbase.h:
//! C functions for external declarations that call the appropriate C++ methods

// Windows uses "__declspec(dllexport)" to mark functions in the DLL that
// should be visible/callable externally.
//
// It also apparently requires WINAPI for GiveFnptrsToDll().
//
// See doc/notes_windows_coding for more information..

// Attributes to specify an "exported" function, visible from outside the
// DLL.
#undef DLLEXPORT
#ifdef _WIN32
	#define DLLEXPORT	__declspec(dllexport)
	// WINAPI should be provided in the windows compiler headers.
	// It's usually defined to something like "__stdcall".
#elif defined(linux)
	#define DLLEXPORT	/* */
	#define WINAPI		/* */
#endif /* linux */

// Simplified macro for declaring/defining exported DLL functions.  They
// need to be 'extern "C"' so that the C++ compiler enforces parameter
// type-matching, rather than considering routines with mis-matched
// arguments/types to be overloaded functions...
//
// AFAIK, this is os-independent, but it's included here in osdep.h where
// DLLEXPORT is defined, for convenience.
#define C_DLLEXPORT		extern "C" DLLEXPORT

// Special version that fixes vsnprintf bugs.
#ifndef DO_NOT_FIX_VARARG_ENGINE_API_WARPERS
int DLLINTERNAL safe_vsnprintf(char* s,  size_t n,  const char *format, va_list ap);
int DLLINTERNAL safe_snprintf(char* s, size_t n, const char* format, ...);
#endif
void DLLINTERNAL safevoid_vsnprintf(char* s, size_t n, const char *format, va_list ap);
void DLLINTERNAL safevoid_snprintf(char* s, size_t n, const char* format, ...);

// Functions & types for DLL open/close/etc operations.
extern mBOOL dlclose_handle_invalid DLLHIDDEN;
#ifdef linux
	#include <dlfcn.h>
	typedef void* DLHANDLE;
	typedef void* DLFUNC;
	inline DLHANDLE DLLINTERNAL DLOPEN(const char *filename) {
		return(dlopen(filename, RTLD_NOW));
	}
	inline DLFUNC DLLINTERNAL DLSYM(DLHANDLE handle, const char *string) {
		return(dlsym(handle, string));
	}
	//dlclose crashes if handle is null.
	inline int DLLINTERNAL DLCLOSE(DLHANDLE handle) {
		if(!handle) {
			dlclose_handle_invalid = mTRUE;
			return(1);
		}
		
		dlclose_handle_invalid = mFALSE;
		return(dlclose(handle));
	}
	inline const char * DLLINTERNAL DLERROR(void) {
		if(dlclose_handle_invalid)
			return("Invalid handle.");
		return(dlerror());
	}
#elif defined(_WIN32)
	typedef HINSTANCE DLHANDLE;
	typedef FARPROC DLFUNC;
	inline DLHANDLE DLLINTERNAL DLOPEN(const char *filename) {
		return(LoadLibraryA(filename));
	}
	inline DLFUNC DLLINTERNAL DLSYM(DLHANDLE handle, const char *string) {
		return(GetProcAddress(handle, string));
	}
	inline int DLLINTERNAL DLCLOSE(DLHANDLE handle) {
		if(!handle) {
			dlclose_handle_invalid = mTRUE;
			return(1);
		}
		
		dlclose_handle_invalid = mFALSE;
		
		// NOTE: Windows FreeLibrary returns success=nonzero, fail=zero,
		// which is the opposite of the unix convention, thus the '!'.
		return(!FreeLibrary(handle));
	}
	// Windows doesn't provide a function corresponding to dlerror(), so
	// we make our own.
	char * DLLINTERNAL str_GetLastError(void);
	inline const char * DLLINTERNAL DLERROR(void) {
		if(dlclose_handle_invalid)
			return("Invalid handle.");
		return(str_GetLastError());
	}
#endif /* _WIN32 */
const char * DLLINTERNAL DLFNAME(void *memptr);
mBOOL DLLINTERNAL IS_VALID_PTR(void *memptr);


// Attempt to call the given function pointer, without segfaulting.
mBOOL DLLINTERNAL os_safe_call(REG_CMD_FN pfn);


// Windows doesn't have an strtok_r() routine, so we write our own.
#ifdef _WIN32
	#define strtok_r(s, delim, ptrptr)	my_strtok_r(s, delim, ptrptr)
	char * DLLINTERNAL my_strtok_r(char *s, const char *delim, char **ptrptr);
#endif /* _WIN32 */


// Linux doesn't have an strlwr() routine, so we write our own.
#ifdef linux
	#define strlwr(s) my_strlwr(s)
	char * DLLINTERNAL my_strlwr(char *s);
#endif /* _WIN32 */


// Set filename and pathname maximum lengths.  Note some windows compilers
// provide a <limits.h> which is incomplete and/or causes problems; see
// doc/windows_notes.txt for more information.
//
// Note that both OS's include room for null-termination:
//   linux:    "# chars in a path name including nul"
//   win32:    "note that the sizes include space for 0-terminator"
#ifdef linux
	#include <limits.h>
#elif defined(_WIN32)
	#include <stdlib.h>
	#define NAME_MAX	_MAX_FNAME
	#ifndef PATH_MAX
		#define PATH_MAX	_MAX_PATH
	#endif
#endif /* _WIN32 */

// Various other windows routine differences.
#ifdef linux
	#include <unistd.h>	// sleep
	#ifndef O_BINARY
    		#define O_BINARY 0
	#endif	
#elif defined(_WIN32)
	#include <io.h>
	#include <direct.h>
	
	#define sleep(x) Sleep(x*1000)

	// Fixed MSVC compiling, by Nikolay "The Storm" Baklicharov.
	#if defined(__GNUC__) || defined (_MSC_VER) && _MSC_VER >= 1400
		#define snprintf	_snprintf
		#define vsnprintf	_vsnprintf
		#define unlink		_unlink
		#define strlwr		_strlwr
		#define strdup		_strdup
		#define strcasecmp	_stricmp
		#define strncasecmp	_strnicmp
		#define getcwd		_getcwd
		#define open		_open
		#define read		_read
		#define write		_write
		#define close		_close
	#endif /* GCC or MSVC 8.0+ */
#endif /* _WIN32 */

#if !defined WIN32 && !defined _MSC_VER
#include <unistd.h>	// getcwd
#endif

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
#endif /* _WIN32 */

// Normalize/standardize a pathname.
//  - For win32, this involves:
//    - Turning backslashes (\) into slashes (/), so that config files and
//      Metamod internal code can be simpler and just use slashes (/).
//    - Turning upper/mixed case into lowercase, since windows is
//      non-case-sensitive.
//  - For linux, this requires no work, as paths uses slashes (/) natively,
//    and pathnames are case-sensitive.
#ifdef linux
#define normalize_pathname(a)
#elif defined(_WIN32)
void DLLINTERNAL normalize_pathname(char *path);
#endif /* _WIN32 */

// Indicate if pathname appears to be an absolute-path.  Under linux this
// is a leading slash (/).  Under win32, this can be:
//  - a drive-letter path (ie "D:blah" or "C:\blah")
//  - a toplevel path (ie "\blah")
//  - a UNC network address (ie "\\srv1\blah").
// Also, handle both native and normalized pathnames.
inline mBOOL DLLINTERNAL is_absolute_path(const char *path) {
	if(path[0]=='/') return(mTRUE);
#ifdef _WIN32
	if(path[1]==':') return(mTRUE);
	if(path[0]=='\\') return(mTRUE);
#endif /* _WIN32 */
	return(mFALSE);
}

#ifdef _WIN32
// Buffer pointed to by resolved_name is assumed to be able to store a
// string of PATH_MAX length.
char * DLLINTERNAL realpath(const char *file_name, char *resolved_name);
#endif /* _WIN32 */

// Generic "error string" from a recent OS call.  For linux, this is based
// on errno.  For win32, it's based on GetLastError.
inline const char * DLLINTERNAL str_os_error(void) {
#ifdef linux
	return(strerror(errno));
#elif defined(_WIN32)
	return(str_GetLastError());
#endif /* _WIN32 */
}


#endif /* OSDEP_H */
