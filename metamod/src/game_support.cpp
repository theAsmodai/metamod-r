#include "precompiled.h"

// Adapted from adminmod h_export.cpp:
//! this structure contains a list of supported mods and their dlls names
//! To add support for another mod add an entry here, and add all the
//! exported entities to link_func.cpp
const game_modinfo_t known_games[] = {
	// name/gamedir		linux_so		win_dll			desc
	//
	// Previously enumerated in this sourcefile, the list is now kept in a
	// separate file, generated based on game information stored in a
	// convenient db.
	{ "cstrike",		"cs.so",		"mp.dll",		"Counter-Strike" },
	{ "czero",		"cs.so",		"mp.dll",		"Counter-Strike:Condition Zero" },

	// End of list terminator:
	{ NULL, NULL, NULL, NULL }
};

// Find a modinfo corresponding to the given game name.
inline const game_modinfo_t *lookup_game(const char *name)
{
	for (auto& known : known_games)
	{
		if (known.name && Q_stricmp(known.name, name))
			return &known;
	}

	// no match found
	return nullptr;
}

// Installs gamedll from Steam cache
mBOOL install_gamedll(char *from, const char *to)
{
	int length_in;
	int length_out;

	if (!from)
		return mFALSE;

	if (!to)
		to = from;

	byte *cachefile = LOAD_FILE_FOR_ME(from, &length_in);

	// If the file seems to exist in the cache.
	if (cachefile)
	{
		int fd = open(to, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0)
		{
			META_DEBUG(3, ("Installing gamedll from cache: Failed to create file %s: %s", to, strerror(errno)) );
			FREE_FILE(cachefile);
			return mFALSE;
		}

		length_out=write(fd, cachefile, length_in);
		FREE_FILE(cachefile);
		close(fd);

		// Writing the file was not successfull
		if (length_out != length_in)
		{
			META_DEBUG(3,("Installing gamedll from chache: Failed to write all %d bytes to file, only %d written: %s", length_in, length_out, strerror(errno)));

			// Let's not leave a mess but clean up nicely.
			if (length_out >= 0)
				unlink(to);

			return mFALSE;
		}

		META_LOG("Installed gamedll %s from cache.", to);
	}
	else
	{
		META_DEBUG(3, ("Failed to install gamedll from cache: file %s not found in cache.", from));
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
mBOOL setup_gamedll(gamedll_t *gamedll)
{
	const game_modinfo_t *known;
	const char *knownfn = nullptr;

	// Check for old-style "metagame.ini" file and complain.
	if (valid_gamedir_file(OLD_GAMEDLL_TXT))
	{
		META_WARNING("File '%s' is no longer supported; instead, specify override gamedll in %s or with '+localinfo mm_gamedll <dllfile>'", OLD_GAMEDLL_TXT, CONFIG_INI);
	}

	// First, look for a known game, based on gamedir.
	if ((known = lookup_game(gamedll->name)))
	{
#ifdef _WIN32
		knownfn = known->win_dll;
#else
		knownfn = known->linux_so;
#endif

		META_DEBUG(4, ("Checking for old version game DLL name '%s'.\n", knownfn));
		safevoid_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "dlls/%s", knownfn);

		// Check if the gamedll file exists. If not, try to install it from the cache.
		if (!valid_gamedir_file(gamedll->pathname))
		{
			safevoid_snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);
			install_gamedll(gamedll->pathname, gamedll->real_pathname);
		}
	}
	else
	{
		// Neither known-list found a gamedll.
		RETURN_ERRNO(mFALSE, ME_NOTFOUND);
	}


	safevoid_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);

	// get filename from pathname
	char *cp = Q_strrchr(gamedll->pathname, '/');
	if (cp)
		cp++;
	else
		cp = gamedll->pathname;

	gamedll->file = cp;

	Q_strncpy(gamedll->real_pathname, gamedll->pathname, sizeof(gamedll->real_pathname) - 1);
	gamedll->real_pathname[sizeof(gamedll->real_pathname) - 1] = '\0';

	gamedll->desc = known->desc;
	META_LOG("Recognized game '%s'; using dllfile '%s'", gamedll->name, gamedll->file);

	return mTRUE;
}
