// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// osdep.cpp - routines for operating system differences

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

#ifdef linux
// enable extra routines in system header files, like dladdr
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#include <dlfcn.h>			// dlopen, dladdr, etc
#endif /* linux */

#include <string.h>			// strpbrk, etc

#include <extdll.h>			// always

#include "osdep.h"			// me
#include "mreg.h"			// RG_VALID, etc
#include "log_meta.h"		// META_ERROR, etc
#include "types_meta.h"		// mBOOL
#include "support_meta.h"	// MAX_STRBUF_LEN
#include "limits.h"		// INT_MAX


mBOOL dlclose_handle_invalid;

#ifdef _WIN32
// Since windows doesn't provide a verison of strtok_r(), we include one
// here.  This may or may not operate exactly like strtok_r(), but does
// what we need it it do.
char * DLLINTERNAL my_strtok_r(char *s, const char *delim, char **ptrptr) {
	char *begin=NULL;
	char *end=NULL;
	char *rest=NULL;
	if(s)
		begin=s;
	else
		begin=*ptrptr;
	if(!begin)
		return(NULL);
	end=strpbrk(begin, delim);
	if(end) {
		*end='\0';
		rest=end+1;
		*ptrptr=rest+strspn(rest, delim);
	}
	else
		*ptrptr=NULL;
	return(begin);
}
#endif /* _WIN32 */


#ifdef linux
char * DLLINTERNAL my_strlwr(char *s) {
	char *c;
	if(!s)
		return(0);
	for(c=s;*c;c++)
		*c = tolower(*c);
	return(s);
}
#endif


#ifndef DO_NOT_FIX_VARARG_ENGINE_API_WARPERS
// Microsoft's msvcrt.dll:vsnprintf is buggy and so is vsnprintf on some glibc versions.
// We use wrapper function to fix bugs.
//  from: http://sourceforge.net/tracker/index.php?func=detail&aid=1083721&group_id=2435&atid=102435
int DLLINTERNAL safe_vsnprintf(char* s, size_t n, const char *format, va_list src_ap) { 
	va_list ap;
	int res;
	char *tmpbuf;
	size_t bufsize = n;
	
	if(s && n>0)
		s[0]=0;
	
	// If the format string is empty, nothing to do.
	if(!format || !*format)
		return(0);
	
	// The supplied count may be big enough. Try to use the library
     	// vsnprintf, fixing up the case where the library function
	// neglects to terminate with '/0'.
	if(n > 0)
	{
		// A NULL destination will cause a segfault with vsnprintf.
		// if n > 0.  Nor do we want to copy our tmpbuf to NULL later.
		if(!s)
			return(-1);
		
		va_copy(ap, src_ap);
		res = vsnprintf(s, n, format, ap);
		va_end(ap);
		
		if(res > 0) {
			if((unsigned)res == n)
				s[res - 1] = 0;
	  		return(res);
		}
		
		// If n is already larger than INT_MAX, increasing it won't
		// help.
		if(n > INT_MAX)
			return(-1);
		
		// Try a larger buffer.
		bufsize *= 2;
	}
	
	if(bufsize < 1024)
		bufsize = 1024;
	
	tmpbuf = (char *)malloc(bufsize * sizeof(char));
	if(!tmpbuf)
		return(-1);
	
	va_copy(ap, src_ap);
  	res = vsnprintf(tmpbuf, bufsize, format, ap);
  	va_end(ap);
	
	// The test for bufsize limit is probably not necesary
	// with 2GB address space limit, since, in practice, malloc will
	// fail well before INT_MAX.
	while(res < 0 && bufsize <= INT_MAX) {
		char * newbuf;
		
		bufsize *= 2;
		newbuf = (char*)realloc(tmpbuf, bufsize * sizeof(char));
		
		if(!newbuf)
			break;
		
		tmpbuf = newbuf;
		
		va_copy(ap, src_ap);
		res = vsnprintf(tmpbuf, bufsize, format, ap);
		va_end(ap);
	}
	
	if(res > 0 && n > 0) {
		if(n > (unsigned)res)
			memcpy(s, tmpbuf, (res + 1) * sizeof (char));
		else {
			memcpy(s, tmpbuf, (n - 1) * sizeof (char));
			s[n - 1] = 0;
		}
	}
	
	free(tmpbuf);
	return(res);
}

int DLLINTERNAL safe_snprintf(char* s, size_t n, const char* format, ...) {
	int res;
	va_list ap;
	
	va_start(ap, format);
	res = safe_vsnprintf(s, n, format, ap);
	va_end(ap);
	
	return(res);
}
#endif

void DLLINTERNAL safevoid_vsnprintf(char* s, size_t n, const char *format, va_list ap) { 
	int res;
	
	if(!s || n <= 0)
		return;
	
	// If the format string is empty, nothing to do.
	if(!format || !*format) {
		s[0]=0;
		return;
	}
	
	res = vsnprintf(s, n, format, ap);
	
	// w32api returns -1 on too long write, glibc returns number of bytes it could have written if there were enough space
	// w32api doesn't write null at all, some buggy glibc don't either
	if(res < 0 || (size_t)res >= n)
		s[n-1]=0;
}

void DLLINTERNAL safevoid_snprintf(char* s, size_t n, const char* format, ...) {
	va_list ap;
	
	va_start(ap, format);
	safevoid_vsnprintf(s, n, format, ap);
	va_end(ap);
}


#ifdef _WIN32
// Windows doesn't provide a functon analagous to dlerr() that returns a
// string describing the error, so we include one here, as exampled at:
//    http://msdn.microsoft.com/library/en-us/debug/errors_0sdh.asp
// except without FORMAT_MESSAGE_ALLOCATE_BUFFER, since we use a local
// static buffer.
char * DLLINTERNAL str_GetLastError(void) {
	static char buf[MAX_STRBUF_LEN];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //! Default language 
			(LPTSTR) &buf, MAX_STRBUF_LEN-1, NULL);
	return(buf);
}
#endif /* _WIN32 */


// Find the filename of the DLL/shared-lib where the given memory location
// exists.
#ifdef linux
// Errno values:
//  - ME_NOTFOUND	couldn't find a sharedlib that contains memory location
const char * DLLINTERNAL DLFNAME(void *memptr) {
	Dl_info dli;
	memset(&dli, 0, sizeof(dli));
	if(dladdr(memptr, &dli))
		return(dli.dli_fname);
	else
		RETURN_ERRNO(NULL, ME_NOTFOUND);
}
#elif defined(_WIN32)
// Implementation for win32 provided by Jussi Kivilinna <kijuhe00@rhea.otol.fi>:
//
//    1. Get memory location info on memptr with VirtualQuery.
//    2. Check if memory location info is valid and use MBI.AllocationBase
//       as module start point.
//    3. Get module file name with GetModuleFileName.
//
//    Simple and should work pretty much same way as 'dladdr' in linux.
//    VirtualQuery and GetModuleFileName work even with win32s.
//
// Note that GetModuleFileName returns longfilenames rather than 8.3.
//
// Note also, the returned filename is local static storage, and should be
// copied by caller if it needs to keep it around.
//
// Also note, normalize_pathname() should really be done by the caller, but
// is done here to preserve "const char *" return consistent with linux
// version.
//
// Errno values:
//  - ME_NOTFOUND	couldn't find a DLL that contains memory location
const char * DLLINTERNAL DLFNAME(void *memptr) {
	MEMORY_BASIC_INFORMATION MBI;
	static char fname[PATH_MAX];

	memset(fname, 0, sizeof(fname));

	if(!VirtualQuery(memptr, &MBI, sizeof(MBI)))
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	if(MBI.State != MEM_COMMIT)
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	if(!MBI.AllocationBase)
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	
	// MSDN indicates that GetModuleFileName will leave string
	// null-terminated, even if it's truncated because buffer is too small.
	if(!GetModuleFileNameA((HMODULE)MBI.AllocationBase, fname, sizeof(fname)-1))
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	if(!fname[0])
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	
	normalize_pathname(fname);
	return(fname);
}
#endif /* _WIN32 */


#ifdef _WIN32
// Normalize/standardize a pathname.
//  - For win32, this involves:
//    - Turning backslashes (\) into slashes (/), so that config files and
//      Metamod internal code can be simpler and just use slashes (/).
//    - Turning upper/mixed case into lowercase, since windows is
//      non-case-sensitive.
//  - For linux, this requires no work, as paths uses slashes (/) natively,
//    and pathnames are case-sensitive.
void DLLINTERNAL normalize_pathname(char *path) {
	char *cp;

	META_DEBUG(8, ("normalize: %s", path));
	for(cp=path; *cp; cp++) {
		/*if(isupper(*cp))*/
			*cp=tolower(*cp);
		
		if(*cp=='\\')
			*cp='/';
	}
	META_DEBUG(8, ("normalized: %s", path));
}

// Buffer pointed to by resolved_name is assumed to be able to store a
// string of PATH_MAX length.
char * DLLINTERNAL realpath(const char *file_name, char *resolved_name) {
	int ret;
	ret=GetFullPathNameA(file_name, PATH_MAX, resolved_name, NULL);
	if(ret > PATH_MAX) {
		errno=ENAMETOOLONG;
		return(NULL);
	}
	else if(ret > 0) {
		HANDLE handle;
		WIN32_FIND_DATAA find_data;
		handle=FindFirstFileA(resolved_name, &find_data);
		if(INVALID_HANDLE_VALUE == handle) {
			errno=ENOENT;
			return(NULL);
		}
		FindClose(handle);
		normalize_pathname(resolved_name);
		return(resolved_name);
	}
	else 
		return(NULL);
}
#endif /*_WIN32*/


// Determine whether the given memory location is valid (ie whether we
// should expect to be able to reference strings or functions at this
// location without segfaulting).
#ifdef linux
// Simulate this with dladdr.  I'm not convinced this will be as generally
// applicable as the native windows routine below, but it should do what
// we need it for in this particular situation.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching sharedlib for this ptr
mBOOL DLLINTERNAL IS_VALID_PTR(void *memptr) {
	Dl_info dli;
	memset(&dli, 0, sizeof(dli));
	if(dladdr(memptr, &dli))
		return(mTRUE);
	else
		RETURN_ERRNO(mFALSE, ME_NOTFOUND);
}
#elif defined(_WIN32)
// Use the native windows routine IsBadCodePtr.
// meta_errno values:
//  - ME_BADMEMPTR	not a valid memory pointer
mBOOL DLLINTERNAL IS_VALID_PTR(void *memptr) {
	if(IsBadCodePtr((FARPROC) memptr))
		RETURN_ERRNO(mFALSE, ME_BADMEMPTR);
	else
		return(mTRUE);
}
#endif /* _WIN32 */

// This used to be OS-dependent, as it used a SEGV signal handler under
// linux, but that was removed because (a) it masked legitimate segfaults
// in plugin commands and produced confusing output ("plugin has been
// unloaded", when really it segfaultd), and (b) wasn't necessary since
// IS_VALID_PTR() should cover the situation.
mBOOL DLLINTERNAL os_safe_call(REG_CMD_FN pfn) {
	// try and see if this is a valid memory location
	if(!IS_VALID_PTR((void *) pfn))
		// meta_errno should be already set in is_valid_ptr()
		return(mFALSE);

	pfn();
	return(mTRUE);
}

