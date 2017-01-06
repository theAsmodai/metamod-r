#include "precompiled.h"

bool dlclose_handle_invalid;

#ifdef _WIN32
// Since windows doesn't provide a verison of strtok_r(), we include one
// here.  This may or may not operate exactly like strtok_r(), but does
// what we need it it do.
char *my_strtok_r(char *s, const char *delim, char **ptrptr)
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
	if (end)
	{
		*end = '\0';
		rest = end + 1;
		*ptrptr = rest + strspn(rest, delim);
	}
	else
		*ptrptr = nullptr;

	return begin;
}
#endif // _WIN32

#ifdef _WIN32
// Windows doesn't provide a functon analagous to dlerr() that returns a
// string describing the error, so we include one here, as exampled at:
// http://msdn.microsoft.com/library/en-us/debug/errors_0sdh.asp
// except without FORMAT_MESSAGE_ALLOCATE_BUFFER, since we use a local
// static buffer.
const char *str_GetLastError()
{
	static char buf[MAX_STRBUF_LEN];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buf, MAX_STRBUF_LEN - 1, NULL);
	return buf;
}
#endif

// Find the filename of the DLL/shared-lib where the given memory location
// exists.
#if defined(linux) || defined(__APPLE__)
// Errno values:
//  - ME_NOTFOUND	couldn't find a sharedlib that contains memory location
const char *DLFNAME(void *memptr)
{
	Dl_info dli;
	Q_memset(&dli, 0, sizeof(dli));
	if (dladdr(memptr, &dli))
		return dli.dli_fname;
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
const char *DLFNAME(void *memptr)
{
	MEMORY_BASIC_INFORMATION MBI;
	static char fname[PATH_MAX];

	if (!VirtualQuery(memptr, &MBI, sizeof(MBI)))
		RETURN_ERRNO(NULL, ME_NOTFOUND);

	if (MBI.State != MEM_COMMIT)
		RETURN_ERRNO(NULL, ME_NOTFOUND);

	if (!MBI.AllocationBase)
		RETURN_ERRNO(NULL, ME_NOTFOUND);

	// MSDN indicates that GetModuleFileName will leave string
	// null-terminated, even if it's truncated because buffer is too small.
	if (!GetModuleFileName((HMODULE)MBI.AllocationBase, fname, sizeof(fname) - 1))
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	if (!fname[0])
		RETURN_ERRNO(NULL, ME_NOTFOUND);

	normalize_pathname(fname);
	return fname;
}
#endif // _WIN32


// Determine whether the given memory location is valid (ie whether we
// should expect to be able to reference strings or functions at this
// location without segfaulting).
#if defined(linux) || defined(__APPLE__)
// Simulate this with dladdr.  I'm not convinced this will be as generally
// applicable as the native windows routine below, but it should do what
// we need it for in this particular situation.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching sharedlib for this ptr
bool IS_VALID_PTR(void *memptr)
{
	Dl_info dli;
	Q_memset(&dli, 0, sizeof(dli));
	if (dladdr(memptr, &dli))
		return true;
	else
		RETURN_ERRNO(false, ME_NOTFOUND);
}
#elif defined(_WIN32)
// Use the native windows routine IsBadCodePtr.
// meta_errno values:
//  - ME_BADMEMPTR	not a valid memory pointer
bool IS_VALID_PTR(void *memptr)
{
	if (IsBadCodePtr((FARPROC) memptr))
		RETURN_ERRNO(false, ME_BADMEMPTR);
	else
		return true;
}
#endif // _WIN32

// This used to be OS-dependent, as it used a SEGV signal handler under
// linux, but that was removed because (a) it masked legitimate segfaults
// in plugin commands and produced confusing output ("plugin has been
// unloaded", when really it segfaultd), and (b) wasn't necessary since
// IS_VALID_PTR() should cover the situation.
bool os_safe_call(REG_CMD_FN pfn)
{
	// try and see if this is a valid memory location
	if (!IS_VALID_PTR((void *)pfn))
		// meta_errno should be already set in is_valid_ptr()
		return false;

	pfn();
	return true;
}
