// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// game_support.cpp - info to recognize different HL mod "games"

/*
 * Copyright (c) 2001-2013 Will Day <willday@hpgx.net>
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
 *    link the code of this program with the Half-Life Game g_engine ("HL
 *    g_engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL g_engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include "precompiled.h"

// Adapted from adminmod h_export.cpp:
//! this structure contains a list of supported mods and their dlls names
//! To add support for another mod add an entry here, and add all the 
//! exported entities to link_func.cpp
game_modlist_t known_games = {
	// name/gamedir	 linux_so			win_dll			osx_dylib		desc
	//
	// Previously enumerated in this sourcefile, the list is now kept in a
	// separate file, generated based on game information stored in a 
	// convenient db.
	//
#include "games.h"


	// End of list terminator:
	{NULL, NULL, NULL, NULL, NULL}
};

// Find a modinfo corresponding to the given game name.
game_modinfo_t* lookup_game(const char* name)
{
	for (int i = 0; known_games[i].name; i++) {
		auto imod = &known_games[i];
		if (strcasematch(imod->name, name))
			return imod;
	}
	// no match found
	return NULL;
}

mBOOL install_gamedll(char* from, const char* to)
{
	int length_in;
	int length_out;

	if (NULL == from) return mFALSE;
	if (NULL == to) to = from;

	byte* cachefile = LOAD_FILE_FOR_ME(from, &length_in);

	// If the file seems to exist in the cache.
	if (NULL != cachefile) {

		int fd = open(to, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0) {
			META_DEBUG(3, ("Installing gamedll from cache: Failed to create file %s: %s\n", to, strerror(errno)) );
			FREE_FILE(cachefile);
			return mFALSE;
		}

		length_out = write(fd, cachefile, length_in);
		FREE_FILE(cachefile);
		close(fd);

		// Writing the file was not successfull
		if (length_out != length_in) {
			META_DEBUG(3,("Installing gamedll from cache: Failed to write all %d bytes to file, only %d written: %s\n",
				length_in, length_out, strerror(errno)) );
			// Let's not leave a mess but clean up nicely.
			if (length_out >= 0) _unlink(to);
			return mFALSE;
		}

		META_LOG("Installed gamedll %s from cache.\n", to);

	}
	else {
		META_DEBUG(3, ("Failed to install gamedll from cache: file %s not found in cache.\n", from) );
		return mFALSE;
	}

	return mTRUE;
}

// Set all the fields in the gamedll struct, - based either on an entry in
// known_games matching the current gamedir, or on one specified manually 
// by the server admin.
//
// meta_errno values:
//  - ME_NOTFOUND	couldn't recognize game
mBOOL setup_gamedll(gamedll_t* gamedll)
{
	static char override_desc_buf[256];
	game_modinfo_t* known;
	const char* cp;
	const char* knownfn = 0;
	const char* usedfn = 0;
	char* strippedfn = 0;
	bool override = false;

	// Check for old-style "metagame.ini" file and complain.
	if (valid_gamedir_file(OLD_GAMEDLL_TXT))
		META_ERROR("File '%s' is no longer supported; instead, specify override gamedll in %s or with '+localinfo mm_gamedll <dllfile>'", OLD_GAMEDLL_TXT, CONFIG_INI);

	// First, look for a known game, based on gamedir.
	if ((known = lookup_game(gamedll->name))) {
#ifdef _WIN32
		knownfn = _strdup(known->win_dll);
#elif defined(linux)
		knownfn=_strdup(known->linux_so);
#elif defined(__APPLE__)
		knownfn=_strdup(known->osx_dylib);
#else
#error "OS unrecognized"
#endif /* _WIN32 */
	}

	// Neither override nor auto-detect found a gamedll.
	if (!known && !g_config->gamedll)
	RETURN_ERRNO(mFALSE, ME_NOTFOUND);

	// Use override-dll if specified.
	if (g_config->gamedll) {
		strncpy(gamedll->pathname, g_config->gamedll, sizeof gamedll->pathname - 1);
		gamedll->pathname[sizeof gamedll->pathname - 1] = '\0';
		override = true;

		// If the path is relative, the gamedll file will be missing and
		// it might be found in the cache file.
		if (!is_absolute_path(gamedll->pathname)) {
			// I abuse the real_pathname member here to pass a full pathname to
			// the install_gamedll function. I am somewhat opposed to be pushing
			// another MAX_PATH sized array on the stack, that's why.
			snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/%s", gamedll->gamedir, gamedll->pathname);
			// If we could successfully install the gamedll from the cache we
			// rectify the pathname to be a full pathname.
			if (install_gamedll(gamedll->pathname, gamedll->real_pathname)) {
				strncpy(gamedll->pathname, gamedll->real_pathname, sizeof gamedll->pathname - 1);
				gamedll->pathname[sizeof gamedll->pathname - 1] = '\0';
			}
		}
	}
	// Else use Auto-detect dll.
	else {
#ifdef linux
		// The engine changed game dll lookup behaviour in that it strips
		// anything after the last '_' from the name and tries to load the
		// resulting name. The DSO names were changed and do not have the
		// '_i386' part in them anymore, so cs_i386.so became cs.so. We
		// have to adapt to that and try to load the DSO name without the
		// '_*' part first, to see if we have a new version file available.

		strippedfn = _strdup(knownfn);

		char *loc = strrchr(strippedfn, '_');

		// A small safety net here: make sure that we are dealing with
		// a file name at least four characters long and ending in
		// '.so'. This way we can be sure that we can safely overwrite
		// anything from the '_' on with '.so'.
		int size = 0;
		const char *ext;
		if(0 != loc) {
			size = strlen(strippedfn);
			ext = strippedfn + (size - 3);
		}

		if(0 != loc && size > 3 && 0 == _stricmp(ext, ".so")) {
			
			strcpy(loc, ".so");
			META_DEBUG(4, ("Checking for new version game DLL name '%s'.\n", strippedfn) );

		// Again, as above, I abuse the real_pathname member to store the full pathname
		// and the pathname member to store the relative name to pass it to the 
		// install_gamedll function to save stack space. They are going
		// to get overwritten later on, so that's ok.
			snprintf(gamedll->pathname, sizeof(gamedll->pathname), "dlls/%s", 
					 strippedfn);
		// Check if the gamedll file exists. If not, try to install it from
		// the cache.
			mBOOL ok = mTRUE;
			if(!valid_gamedir_file(gamedll->pathname)) {
				snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/dlls/%s", 
						 gamedll->gamedir, strippedfn);
				ok = install_gamedll(gamedll->pathname, gamedll->real_pathname);
			}
			if(ok) usedfn = strippedfn;
		}
		else {
			META_DEBUG(4, ("Known game DLL name does not qualify for checking for a stripped version, skipping: '%s'.\n",
							strippedfn) );
		}
#endif /* linux */

		// If no file to be used was found, try the old known DLL file
		// name.
		if (0 == usedfn) {
			META_DEBUG(4, ("Checking for old version game DLL name '%s'.\n", knownfn) );
			snprintf(gamedll->pathname, sizeof(gamedll->pathname), "dlls/%s", knownfn);
			// Check if the gamedll file exists. If not, try to install it from
			// the cache.
			if (!valid_gamedir_file(gamedll->pathname)) {
				snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/dlls/%s",
				         gamedll->gamedir, knownfn);
				install_gamedll(gamedll->pathname, gamedll->real_pathname);
			}
			usedfn = knownfn;
		}

		// Now make an absolute path
		snprintf(gamedll->pathname, sizeof(gamedll->pathname), "%s/dlls/%s",
		         gamedll->gamedir, usedfn);
	}

	// get filename from pathname
	cp = strrchr(gamedll->pathname, '/');
	if (cp) cp++;
	else cp = gamedll->pathname;
	gamedll->file = cp;

	// If found, store also the supposed "real" dll path based on the
	// gamedir, in case it differs from the "override" dll path.
	if (known && override)
	snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname),
	         "%s/dlls/%s", gamedll->gamedir, usedfn);
	else { // !known or !override
		strncpy(gamedll->real_pathname, gamedll->pathname, sizeof gamedll->real_pathname - 1);
		gamedll->real_pathname[sizeof gamedll->pathname - 1] = '\0';
	}

	if (override) {
		// generate a desc
		snprintf(override_desc_buf, sizeof(override_desc_buf),
		         "%s (override)", gamedll->file);
		gamedll->desc = override_desc_buf;
		// log result
		META_LOG("Overriding game '%s' with dllfile '%s'", gamedll->name,
		         gamedll->file);
	}
	else {
		gamedll->desc = known->desc;
		META_LOG("Recognized game '%s'; using dllfile '%s'", gamedll->name,
		         gamedll->file);
	}

	if (0 != strippedfn) free(strippedfn);

	return (mTRUE);
}
