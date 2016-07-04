// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// types_meta.h - common internal type, etc definitions

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

#ifndef TYPES_META_H
#define TYPES_META_H

#include "comp_dep.h"


// Our own boolean type, for stricter type matching.
typedef enum mBOOL {
	mFALSE = 0,
	mTRUE,
} mBOOL;

// Like C's errno, for our various functions; describes causes of failure
// or mFALSE returns.
typedef enum {
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
} META_ERRNO;
extern META_ERRNO meta_errno DLLHIDDEN;

#define RETURN_ERRNO(retval, errval) \
	do { meta_errno=errval; return(retval); } while(0)

#define RETURN_LOGERR_ERRNO(errargs, retval, errval) \
	do { META_ERROR errargs ; meta_errno=errval; return(retval); } while(0)

#endif /* TYPES_META_H */
