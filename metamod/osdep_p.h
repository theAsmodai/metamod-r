/*
 * Copyright (c) 2004-2006 Jussi Kivilinna
 *
 *    This file is part of "Metamod All-Mod-Support"-patch for Metamod.
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

#ifndef OSDEP_P_H
#define OSDEP_P_H

#include "types_meta.h"		// mBOOL
#include "osdep.h"		// PATH_MAX

// Checks if file is hlsdk api game dll 
//   (osdep_detect_gamedll_linux.cpp and osdep_detect_gamedll_win32.cpp)
//  --Jussi Kivilinna
mBOOL DLLINTERNAL is_gamedll(const char *filename);

// MSVC doesn't provide opendir/readdir/closedir, so we write our own.
//  --Jussi Kivilinna
#ifdef _WIN32
	struct my_dirent {
		char d_name[PATH_MAX];
	};
	typedef struct {
		HANDLE			 handle;
		WIN32_FIND_DATAA find_data;
		struct my_dirent ent;
		int              not_found;
	} my_DIR;
	
	#define dirent my_dirent
	#define DIR my_DIR
	
	DIR * DLLINTERNAL my_opendir(const char *);
	struct dirent * DLLINTERNAL my_readdir(DIR *);
	void DLLINTERNAL my_closedir(DIR *);
	
	#define opendir(x) my_opendir(x)
	#define readdir(x) my_readdir(x)
	#define closedir(x) my_closedir(x)
#else
	#include <dirent.h>
#endif /* _WIN32 */

DLHANDLE DLLINTERNAL get_module_handle_of_memptr(void * memptr);

#ifdef linux
	void * DLLINTERNAL get_dlsym_pointer(void);
#endif

#endif /* OSDEP_P_H */
