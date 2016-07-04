// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// autodetect.cpp - GameDLL search and autodetection.

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

#include <extdll.h>				// always
#include "osdep_p.h"				// is_gamedll, ...
#include "game_autodetect.h"			// me
#include "support_meta.h"			// full_gamedir_path,


// Search gamedir/dlls/*.dll for gamedlls
//TODO: add META_DEBUG
const char * DLLINTERNAL autodetect_gamedll(const gamedll_t *gamedll, const char *knownfn)
{
	static char buf[256];
	char dllpath[256];
	char fnpath[256];
	DIR *dir;
	struct dirent *ent;
	unsigned int fn_len;
		
	// Generate dllpath
	safevoid_snprintf(buf, sizeof(buf), "%s/dlls", gamedll->gamedir);
	if(!full_gamedir_path(buf, dllpath)) {
		//whine & return
		META_WARNING("GameDLL-Autodetection: Directory '%s' doesn't exist.", buf);
		return(0);
	}
	
	// Generate knownfn path
	safevoid_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, knownfn);
	
	// Check if knownfn exists and is valid gamedll
	if(is_gamedll(fnpath)) {
		// knownfn exists and is loadable gamedll, return 0.
		return(0);
	}
	
	// Open directory
	if(!(dir = opendir(dllpath))) {
		//whine & return
		META_WARNING("GameDLL-Autodetection: Couldn't open directory '%s'.", dllpath);
		return(0);
	}
	
	while((ent = readdir(dir)) != 0) {
		fn_len = strlen(ent->d_name);
		
		if(fn_len <= strlen(PLATFORM_DLEXT)) {
			// Filename is too short
			continue;
		}
		
		// Compare end of filename with PLATFORM_DLEXT
		if(!strcasematch(&ent->d_name[fn_len - strlen(PLATFORM_DLEXT)], PLATFORM_DLEXT)) {
			// File isn't dll
			continue;
		}
		
		// Exclude all metamods
		if(strncasematch(ent->d_name, "metamod", strlen("metamod"))) {
			continue;
		}
		
		// Exclude all bots
		STRNCPY(buf, ent->d_name, sizeof(buf));
		strlwr(buf);
		if(strstr(buf, "bot.")) {
			continue;
		}
#ifdef linux
		//bot_iX86.so, bot_amd64.so, bot_x86_64.so
		if(strstr(buf, "bot_i") || strstr(buf, "bot_amd64.so") || strstr(buf, "bot_x86")) {
			continue;
		}
#endif
		
		// Generate full path
		safevoid_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, ent->d_name);
		
		// Check if dll is gamedll
		if(is_gamedll(fnpath)) {
			META_DEBUG(8, ("is_gamedll(%s): ok.", fnpath));
			//gamedll detected
			STRNCPY(buf, ent->d_name, sizeof(buf));
			closedir(dir);
			return(buf);
		}
		META_DEBUG(8, ("is_gamedll(%s): failed.", fnpath));
	}
	
	//not found
	META_WARNING("GameDLL-Autodetection: Couldn't find gamedll in '%s'.", dllpath);
	closedir(dir);
	
	return(0);
}

