#pragma once

// Our own boolean type, for stricter type matching.
enum mBOOL { mFALSE = 0, mTRUE, };

// Like C's errno, for our various functions; describes causes of failure
// or mFALSE returns.
enum META_ERRNO
{
	ME_NOERROR = 0,
	ME_FORMAT,			// invalid format
	ME_COMMENT,			// ignored comment
	ME_BLANK,			// ignored blank (empty) line
	ME_ALREADY,			// request had already been done
	ME_DELAYED,			// request is delayed
	ME_NOTALLOWED,		// request not allowed
	ME_SKIPPED,			// request is being skipped for whatever reason
	ME_BADREQ,			// invalid request for this <whatever>
	ME_ARGUMENT,		// invalid arguments
	ME_NULLRESULT,		// resulting data was empty or null
	ME_MAXREACHED,		// reached max/limit
	ME_NOTUNIQ,			// not unique (ambigious match)
	ME_NOTFOUND,		// in find operation, match not found
	ME_NOFILE,			// file empty or missing
	ME_NOMEM,			// malloc failed
	ME_BADMEMPTR,		// invalid memory address
	ME_OSNOTSUP,		// OS doesn't support this operation
	ME_DLOPEN,			// failed to open shared lib/dll
	ME_DLMISSING,		// symbol missing in lib/dll
	ME_DLERROR,			// some other error encountered calling functions from dll
	ME_IFVERSION,		// incompatible interface version
	ME_UNLOAD_UNLOADER,	// tried to unload unloader
	ME_UNLOAD_SELF,		// tried to unload self
};

extern META_ERRNO meta_errno;

#define RETURN_ERRNO(retval, errval) \
	do { meta_errno=errval; return retval; } while (0)

#define RETURN_LOGERR_ERRNO(errargs, retval, errval) \
	do { META_ERROR errargs ; meta_errno=errval; return retval; } while (0)
