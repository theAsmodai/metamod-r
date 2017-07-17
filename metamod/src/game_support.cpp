#include "precompiled.h"

// Adapted from adminmod h_export.cpp:
//! this structure contains a list of supported mods and their dlls names
//! To add support for another mod add an entry here, and add all the
//! exported entities to link_func.cpp
const game_modinfo_t g_known_games[] = {
	// name/gamedir		linux_so		win_dll			desc
	//
	// Previously enumerated in this sourcefile, the list is now kept in a
	// separate file, generated based on game information stored in a
	// convenient db.
	{ "valve",      "hl.so",        "hl.dll",       "Half-Life" },
	{ "cstrike",    "cs.so",        "mp.dll",       "Counter-Strike" },
	{ "czero",      "cs.so",        "mp.dll",       "Counter-Strike:Condition Zero" },

	// End of list terminator:
	{ nullptr, nullptr, nullptr, nullptr }
};

// Find a modinfo corresponding to the given game name.
static const game_modinfo_t *lookup_game(const char *name)
{
	for (auto& known : g_known_games) {
		if (known.name && !Q_stricmp(known.name, name))
			return &known;
	}

	// no match found
	return nullptr;
}

// Installs gamedll from Steam cache
bool install_gamedll(char *from, const char *to)
{
	if (!from)
		return false;

	if (!to)
		to = from;

	int length_in;
	byte *cachefile = LOAD_FILE_FOR_ME(from, &length_in);

	// If the file seems to exist in the cache.
	if (cachefile) {
		int fd = open(to, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0) {
			META_DEBUG(3, "Installing gamedll from cache: Failed to create file %s: %s", to, strerror(errno));
			FREE_FILE(cachefile);
			return false;
		}

		int length_out = Q_write(fd, cachefile, length_in);
		FREE_FILE(cachefile);
		close(fd);

		// Writing the file was not successfull
		if (length_out != length_in) {
			META_DEBUG(3, "Installing gamedll from chache: Failed to write all %d bytes to file, only %d written: %s", length_in, length_out, strerror(errno));

			// Let's not leave a mess but clean up nicely.
			if (length_out >= 0)
				_unlink(to);

			return false;
		}

		META_LOG("Installed gamedll %s from cache.", to);
	}
	else {
		META_DEBUG(3, "Failed to install gamedll from cache: file %s not found in cache.", from);
		return false;
	}

	return true;
}

// Set all the fields in the gamedll struct, - based either on an entry in
// known_games matching the current gamedir, or on one specified manually
// by the server admin.
//
// meta_errno values:
//  - ME_NOTFOUND	couldn't recognize game
bool setup_gamedll(gamedll_t *gamedll)
{
	bool override = false;
	const game_modinfo_t *known;
	const char *knownfn = nullptr;

	// First, look for a known game, based on gamedir.
	if ((known = lookup_game(gamedll->name))) {
#ifdef _WIN32
		knownfn = known->win_dll;
#else
		knownfn = known->linux_so;
#endif
	}

	// Neither override nor known-list found a gamedll.
	if (!known && !g_config->m_gamedll)
		return false;

	// Use override-dll if specified.
	if (g_config->m_gamedll) {
		Q_strncpy(gamedll->pathname, g_config->m_gamedll, sizeof gamedll->pathname - 1);
		gamedll->pathname[sizeof gamedll->pathname - 1] = '\0';

		// If the path is relative, the gamedll file will be missing and
		// it might be found in the cache file.
		if (!is_abs_path(gamedll->pathname)) {
			char szInstallPath[MAX_PATH];
			Q_snprintf(szInstallPath, sizeof(szInstallPath), "%s/%s", gamedll->gamedir, gamedll->pathname);

			// If we could successfully install the gamedll from the cache we
			// rectify the pathname to be a full pathname.
			if (install_gamedll(gamedll->pathname, szInstallPath)) {
				Q_strncpy(gamedll->pathname, szInstallPath, sizeof(gamedll->pathname));
			}
		}

		override = true;
	}
	// Else use Known-list dll.
	else if (known) {
		Q_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);
	}
	else {
		// Neither override nor known-list found a gamedll.
		return false;
	}

	// get filename from pathname
	char *cp = Q_strrchr(gamedll->pathname, '/');
	if (cp)
		cp++;
	else
		cp = gamedll->pathname;

	gamedll->file = cp;

	// If found, store also the supposed "real" dll path based on the
	// gamedir, in case it differs from the "override" dll path.
	if (known && override) {
		Q_snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);
	}
	else {
		Q_strncpy(gamedll->real_pathname, gamedll->pathname, sizeof gamedll->real_pathname - 1);
		gamedll->real_pathname[sizeof gamedll->real_pathname - 1] = '\0';
	}

	if (override) {
		// generate a desc
		Q_snprintf(gamedll->desc, sizeof(gamedll->desc), "%s (override)", gamedll->file);

		// log result
		META_LOG("Overriding game '%s' with dllfile '%s'", gamedll->name, gamedll->file);
	}
	else if (known) {
		Q_strncpy(gamedll->desc, known->desc, sizeof gamedll->desc - 1);
		gamedll->desc[sizeof gamedll->desc - 1] = '\0';

		META_LOG("Recognized game '%s'; using dllfile '%s'", gamedll->name, gamedll->file);
	}

	return true;
}
