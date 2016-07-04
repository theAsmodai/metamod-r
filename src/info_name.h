#ifndef INFO_NAME_H
#define INFO_NAME_H

#include "vers_meta.h"		// VDATE, VVERSION, etc

#define VNAME		"Metamod"
#define VAUTHOR		"Will Day"
#define VURL		"http://www.metamod.org/"

#define COPYRIGHT_YEAR "2013"

// Various strings for the Windows DLL Resources in res_meta.rc
#define RC_COMMENTS		"Metamod-P is enhanced version of Metamod. Metamod allows running multiple mod-like plugin DLLs, to add functionality or change the behavior of the running HLDS game mod.  See " VURL
#define RC_DESC			"Metamod-P Half-Life MOD DLL"
#define RC_FILENAME		"METAMOD.DLL"
#define RC_INTERNAL		"METAMOD-P"
#define RC_COPYRIGHT		"Copyright© 2001-" COPYRIGHT_YEAR  " Will Day; 2004-" VPATCH_COPYRIGHT_YEAR " Jussi Kivilinna; GPL licensed"
#define RC_LICENSE		"Licensed under the GNU General Public License"

#endif /* INFO_NAME_H */
