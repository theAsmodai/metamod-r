#include "precompiled.h"

// Constructor
MPluginList::MPluginList(const char* ifile) : size(MAX_PLUGINS), endlist(0)
{
	// store filename of ini file
	Q_strncpy(inifile, ifile, sizeof inifile - 1);
	inifile[sizeof inifile - 1] = '\0';

	// initialize array
	Q_memset(plist, 0, sizeof(plist));
	for (int i = 0; i < size; i++)
	{
		plist[i].index = i + 1; // 1-based
	}

	endlist = 0;
}

// Find a plugin based on the plugin handle.
// meta_errno values:
//  - ME_ARGUMENT	invalid pindex
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin *MPluginList::find(DLHANDLE handle)
{
	if (!handle)
		RETURN_ERRNO(NULL, ME_ARGUMENT);

	for (int i = 0; i < endlist; i++)
	{
		if (plist[i].status < PL_VALID)
			continue;
		if (plist[i].handle == handle)
			return &plist[i];
	}

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin based on the plugin index #.
// meta_errno values:
//  - ME_ARGUMENT	invalid pindex
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin *MPluginList::find(int pindex)
{
	if (pindex <= 0)
		RETURN_ERRNO(NULL, ME_ARGUMENT);

	auto pfound = &plist[pindex - 1];
	if (pfound->status < PL_VALID)
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	else
		return pfound;
}

// Find a plugin with the given plid.
// meta_errno values:
//  - ME_ARGUMENT	null plid_t
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin *MPluginList::find(plid_t id)
{
	if (!id)
		RETURN_ERRNO(NULL, ME_ARGUMENT);

	for (int i = 0; i < endlist; i++)
	{
		if (plist[i].status < PL_VALID)
			continue;

		if (plist[i].info == id)
			return &plist[i];
	}

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin with the given pathname.
// meta_errno values:
//  - ME_ARGUMENT	null path
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin *MPluginList::find(const char* findpath)
{
	if (!findpath)
		RETURN_ERRNO(NULL, ME_ARGUMENT);

	META_DEBUG(8, ("Looking for loaded plugin with dlfnamepath: %s", findpath));

	for (int i = 0; i < endlist; i++)
	{
		META_DEBUG(9, ("Looking at: plugin %s loadedpath: %s", plist[i].file, plist[i].pathname));

		if (plist[i].status < PL_VALID)
			continue;

		if (!Q_strcmp(plist[i].pathname, findpath))
		{
			META_DEBUG(8, ("Found loaded plugin %s", plist[i].file));
			return &plist[i];
		}
	}

	META_DEBUG(8, ("No loaded plugin found with path: %s", findpath));
	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin that uses the given memory location.
// meta_errno values:
//  - ME_ARGUMENT	null memptr
//  - ME_NOTFOUND	couldn't find a matching plugin
//  - errno's from DLFNAME()
MPlugin *MPluginList::find_memloc(void *memptr)
{
	const char* dlfile;

	if (!memptr)
		RETURN_ERRNO(NULL, ME_ARGUMENT);

	if (!(dlfile = DLFNAME(memptr)))
	{
		META_DEBUG(8, ("DLFNAME failed to find memloc %d", memptr));
		// meta_errno should be already set in DLFNAME
		return NULL;
	}

	return find(dlfile);
}

// Find a plugin with non-ambiguous prefix string matching desc, file,
// name, or logtag.
// meta_errno values:
//  - ME_ARGUMENT	null prefix
//  - ME_NOTFOUND	couldn't find a matching plugin
//  - ME_NOTUNIQ	found multiple matches; no unique match
MPlugin *MPluginList::find_match(const char *prefix)
{
	int i, len;
	MPlugin *iplug, *pfound;
	char buf[NAME_MAX ];

	if (!prefix)
	{
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	}

	pfound = NULL;
	len = Q_strlen(prefix);
	Q_snprintf(buf, sizeof(buf), "mm_%s", prefix);
	for (i = 0; i < endlist; i++)
	{
		iplug = &plist[i];
		if (iplug->status < PL_VALID)
			continue;

		if (iplug->info && Q_strnicmp(iplug->info->name, prefix, len) == 0)
		{
			if (pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);

			pfound = iplug;
			continue;
		}
		else if (Q_strnicmp(iplug->desc, prefix, len) == 0)
		{
			if (pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);

			pfound = iplug;
			continue;
		}
		else if (Q_strnicmp(iplug->file, prefix, len) == 0)
		{
			if (pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);

			pfound = iplug;
			continue;
		}
		else if (Q_strnicmp(iplug->file, buf, Q_strlen(buf)) == 0)
		{
			if (pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);

			pfound = iplug;
			continue;
		}
		else if (iplug->info && Q_strnicmp(iplug->info->logtag, prefix, len) == 0)
		{
			if (pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);

			pfound = iplug;
			continue;
		}
	}

	if (pfound)
		return pfound;

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin with same file, logtag, desc or significant
// prefix of file. Uses the platform_match() method of MPlugin.
// meta_errno values:
//  - ME_ARGUMENT	null prefix
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin *MPluginList::find_match(MPlugin* pmatch)
{
	int i;
	MPlugin *iplug, *pfound;
	if (!pmatch)
	{
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	}

	pfound = NULL;
	for (i = 0; i < endlist; i++)
	{
		iplug = &plist[i];
		if (pmatch->platform_match(iplug))
		{
			pfound = iplug;
			break;
		}
	}

	if (pfound)
		return pfound;

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

MPlugin* MPluginList::plugin_addload(plid_t plid, const char* fname, PLUG_LOADTIME now)
{
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added, *pl_loader;

	if (!(pl_loader = find(plid)))
	{
		META_DEBUG(1, ("Couldn't find plugin that gave this loading request!"));
		RETURN_ERRNO(NULL, ME_BADREQ);
	}

	Q_memset(&pl_temp, 0, sizeof(pl_temp));

	if (!pl_temp.plugin_parseline(fname, pl_loader->index))
	{
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}

	if (pl_temp.resolve() != true)
	{
		META_DEBUG(1, ("Couldn't resolve given path into a file: %s", pl_temp.file));
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}

	if ((pl_found = find(pl_temp.pathname)))
	{
		META_DEBUG(1, ("Plugin '%s' already in current list; file=%s desc='%s'",
			pl_temp.file, pl_found->file, pl_found->desc));
		RETURN_ERRNO(NULL, ME_ALREADY);
	}

	if (!(pl_added = add(&pl_temp)))
	{
		META_DEBUG(1, ("Couldn't add plugin '%s' to list; see log", pl_temp.desc));
		return NULL;
	}

	pl_added->action = PA_LOAD;
	if (!pl_added->load(now))
	{
		if (meta_errno == ME_NOTALLOWED || meta_errno == ME_DELAYED)
		{
			META_DEBUG(1, ("Plugin '%s' couldn't attach; only allowed %s", pl_added->desc, pl_added->str_loadable(SL_ALLOWED)));
			pl_added->clear();
		}
		else if (pl_added->status == PL_OPENED)
		{
			META_DEBUG(1, ("Opened plugin '%s', but failed to attach; see log", pl_added->desc));
		}
		else
		{
			META_DEBUG(1, ("Couldn't load plugin '%s'; see log", pl_added->desc));
		}
		return NULL;
	}

	META_DEBUG(1, ("Loaded plugin '%s' successfully", pl_added->desc));
	meta_errno = ME_NOERROR;

	return pl_added;
}

// Add a plugin to the list.
// meta_errno values:
//  - ME_MAXREACHED		reached max plugins
MPlugin* MPluginList::add(MPlugin* padd)
{
	int i;
	MPlugin* iplug;

	// Find either:
	//  - a slot in the list that's not being used
	//  - the end of the list
	for (i = 0; i < endlist && plist[i].status != PL_EMPTY; i++);

	// couldn't find a slot to use
	if (i == size)
	{
		META_ERROR("Couldn't add plugin '%s' to list; reached max plugins (%d)", padd->file, i);
		RETURN_ERRNO(NULL, ME_MAXREACHED);
	}

	// if we found the end of the list, advance end marker
	if (i == endlist)
		endlist++;

	iplug = &plist[i];

	// copy filename into this free slot
	Q_strncpy(iplug->filename, padd->filename, sizeof iplug->filename - 1);
	iplug->filename[sizeof iplug->filename - 1] = '\0';

	// Copy file offset ptr.
	// Can't just copy ptr, as it points to offset in padd, which will go
	// away; need to point to corresponding offset in iplug.
	iplug->file = iplug->filename + (padd->file - padd->filename);

	// copy description
	Q_strncpy(iplug->desc, padd->desc, sizeof iplug->desc - 1);
	iplug->desc[sizeof iplug->desc - 1] = '\0';

	// copy pathname
	Q_strncpy(iplug->pathname, padd->pathname, sizeof iplug->pathname - 1);
	iplug->pathname[sizeof iplug->pathname - 1] = '\0';

	// copy source
	iplug->source = padd->source;

	// copy status
	iplug->status = padd->status;

	//copy other things
	iplug->source_plugin_index = padd->source_plugin_index;

	return iplug;
}


// Read plugins.ini at server startup.
// meta_errno values:
//  - ME_NOFILE		ini file missing or empty
bool MPluginList::ini_startup()
{
	FILE *fp;
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin *pmatch;

	if (!valid_gamedir_file(inifile))
	{
		META_ERROR("ini: Metamod plugins file empty or missing: %s", inifile);
		RETURN_ERRNO(false, ME_NOFILE);
	}

	full_gamedir_path(inifile, inifile);
	fp = fopen(inifile, "r");
	if (!fp)
	{
		META_ERROR("ini: Unable to open plugins file '%s': %s", inifile, strerror(errno));
		RETURN_ERRNO(false, ME_NOFILE);
	}

	META_LOG("ini: Begin reading plugins list: %s", inifile);
	for (n = 0 , ln = 1; !feof(fp) && fgets(line, sizeof(line), fp) && n < size; ln++)
	{
		// Remove line terminations.
		char* cp;
		if ((cp = Q_strrchr(line, '\r')))
			*cp = '\0';

		if ((cp = Q_strrchr(line, '\n')))
			*cp = '\0';

		// Parse directly into next entry in array
		if (!plist[n].ini_parseline(line))
		{
			if (meta_errno == ME_FORMAT)
			{
				META_ERROR("ini: Skipping malformed line %d of %s", ln, inifile);
			}
			continue;
		}
		// Check for a duplicate - an existing entry with this pathname.
		if (find(plist[n].pathname))
		{
			// Should we check platform specific level here?
			META_INFO("ini: Skipping duplicate plugin, line %d of %s: %s", ln, inifile, plist[n].pathname);
			continue;
		}

		// Check for a matching platform with different platform specifics
		// level.
		if (NULL != (pmatch = find_match(&plist[n])))
		{
			if (pmatch->pfspecific >= plist[n].pfspecific)
			{
				META_DEBUG(1, ("ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)", ln, inifile, pmatch->pfspecific, plist[n].pfspecific));
				continue;
			}

			META_DEBUG(1, ("ini: Plugin in line %d overrides existing plugin with lower platform specific level %d, ours %d", ln, pmatch->pfspecific, plist[n].pfspecific));
			int _index = pmatch->index;
			Q_memset(pmatch, 0, sizeof(MPlugin));
			pmatch->index = _index;
		}
		plist[n].action = PA_LOAD;
		META_LOG("ini: Read plugin config for: %s", plist[n].desc);
		n++;
		endlist = n; // mark end of list
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins to load", inifile, n);

	fclose(fp);
	if (!n)
	{
		META_ERROR("ini: Warning; no plugins found to load?");
	}

	return true;
}

// Re-read plugins.ini looking for added/deleted/changed plugins.
// meta_errno values:
//  - ME_NOFILE		ini file missing or empty
bool MPluginList::ini_refresh()
{
	FILE *fp;
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added;

	fp = fopen(inifile, "r");
	if (!fp)
	{
		META_ERROR("ini: Unable to open plugins file '%s': %s", inifile, strerror(errno));
		RETURN_ERRNO(false, ME_NOFILE);
	}

	META_LOG("ini: Begin re-reading plugins list: %s", inifile);
	for (n = 0 , ln = 1; !feof(fp) && fgets(line, sizeof(line), fp) && n < size; ln++)
	{
		// Remove line terminations.
		char *cp;
		if ((cp = Q_strrchr(line, '\r')))
			*cp = '\0';

		if ((cp = Q_strrchr(line, '\n')))
			*cp = '\0';

		// Parse into a temp plugin
		Q_memset(&pl_temp, 0, sizeof(pl_temp));
		if (!pl_temp.ini_parseline(line))
		{
			if (meta_errno == ME_FORMAT)
			{
				META_ERROR("ini: Skipping malformed line %d of %s",ln, inifile);
			}
			continue;
		}
		// Try to find plugin with this pathname in the current list of
		// plugins.
		if (!(pl_found = find(pl_temp.pathname)))
		{
			// Check for a matching platform with higher platform specifics
			// level.
			if (NULL != (pl_found = find_match(&pl_temp)))
			{
				if (pl_found->pfspecific >= pl_temp.pfspecific)
				{
					META_DEBUG(1, ("ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)", ln, inifile, pl_found->pfspecific, pl_temp.pfspecific));
					continue;
				}
				if (PA_LOAD == pl_found->action)
				{
					META_DEBUG(1, ("ini: Plugin in line %d overrides loading of plugin with lower platform specific level %d, ours %d", ln, pl_found->pfspecific, pl_temp.pfspecific));
					int _index = pl_found->index;
					Q_memset(pl_found, 0, sizeof(MPlugin));
					pl_found->index = _index;
				}
				else
				{
					META_DEBUG(1, ("ini: Plugin in line %d should override existing plugin with lower platform specific level %d, ours %d. Unable to comply.", ln, pl_found->pfspecific, pl_temp.pfspecific));
					continue;
				}
			}
			// new plugin; add to list
			if ((pl_added = add(&pl_temp)))
			{
				// try to load this plugin at the next opportunity
				pl_added->action = PA_LOAD;
			}
			else
			// error details logged in add()
				continue;
		}
		else
		{
			// This plugin is already in the current list of plugins.
			// Pathname already matches.  Recopy desc, if specified in
			// plugins.ini.
			if (pl_temp.desc[0] != '<')
			{
				Q_strncpy(pl_found->desc, pl_temp.desc, sizeof pl_found->desc - 1);
				pl_found->desc[sizeof pl_found->desc - 1] = '\0';
			}

			// Check the file to see if it looks like it's been modified
			// since we last loaded it.
			if (!pl_found->newer_file())
			{
				if (meta_errno == ME_NOFILE)
				{
					META_ERROR("ini: Skipping plugin, couldn't stat file '%s': %s", pl_found->pathname, strerror(errno));
					continue;
				}
				else
				{
					// File hasn't been updated.
					// Keep plugin (don't let refresh() unload it).
					pl_found->action = PA_KEEP;
				}
			}
			// Newer file on disk.
			else if (pl_found->status >= PL_OPENED)
			{
				META_DEBUG(2, ("ini: Plugin '%s' has newer file on disk", pl_found->desc));
				pl_found->action = PA_RELOAD;
			}
			else
				META_ERROR("ini: Plugin '%s' has newer file, but unexpected status (%s)", pl_found->desc, pl_found->str_status());
		}

		if (pl_found)
		{
			META_LOG("ini: Read plugin config for: %s", pl_found->desc);
		}
		else
		{
			META_LOG("ini: Read plugin config for: %s", pl_temp.desc);
		}
		n++;
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins", inifile, n);

	fclose(fp);
	if (!n)
	{
		META_ERROR("ini: Warning; no plugins found to load?");
	}

	return true;
}

// Load a plugin from a console command.
// meta_errno values:
//  - errno's from cmd_parseline()
//  - errno's from resolve()
//  - ME_ALREADY	this plugin already loaded
//  - errno's from add()
//  - errno's from load()
bool MPluginList::cmd_addload(const char* args)
{
	MPlugin pl_temp = {};
	MPlugin *pl_found, *pl_added;

	if (pl_temp.cmd_parseline(args) != true)
	{
		META_CONS("Couldn't parse 'meta load' arguments: %s", args);
		// meta_errno should be already set in cmd_parseline()
		return false;
	}

	// resolve given path into a file; accepts various "shortcut"
	// pathnames.
	if (pl_temp.resolve() != true)
	{
		// Couldn't find a matching file on disk
		META_CONS("Couldn't resolve given path into a file: %s", pl_temp.file);
		// meta_errno should be already set in resolve()
		return false;
	}

	// Try to find plugin with this pathname in the current list of
	// plugins.
	if ((pl_found = find(pl_temp.pathname)))
	{
		// Already in list
		META_CONS("Plugin '%s' already in current list; file=%s desc='%s'", pl_temp.file, pl_found->file, pl_found->desc);
		RETURN_ERRNO(false, ME_ALREADY);
	}
	// new plugin; add to list
	if (!(pl_added = add(&pl_temp)))
	{
		META_CONS("Couldn't add plugin '%s' to list; see log", pl_temp.desc);
		// meta_errno should be already set in add()
		return false;
	}

	// try to load new plugin
	pl_added->action = PA_LOAD;
	if (!pl_added->load(PT_ANYTIME))
	{
		// load failed
		if (meta_errno == ME_DELAYED)
			META_CONS("Loaded plugin '%s', but will wait to become active, %s", pl_added->desc, pl_added->str_loadable(SL_ALLOWED));
		else if (meta_errno == ME_NOTALLOWED)
		{
			META_CONS("Plugin '%s' couldn't attach; only allowed %s", pl_added->desc, pl_added->str_loadable(SL_ALLOWED));
			pl_added->clear();
		}
		else if (pl_added->status == PL_OPENED)
			META_CONS("Opened plugin '%s', but failed to attach; see log", pl_added->desc);
		else
			META_CONS("Couldn't load plugin '%s'; see log", pl_added->desc);

		show(0);
		// meta_errno should be already set in load()
		return false;
	}

	META_CONS("Loaded plugin '%s' successfully", pl_added->desc);
	META_CONS("Rebuilding callbacks...");
	meta_rebuild_callbacks();
	show(0);

	return true;
}

// Load plugins at startup.
// meta_errno values:
//  - errno's from ini_startup()
bool MPluginList::load()
{
	int n = 0;
	if (!ini_startup())
	{
		META_ERROR("Problem loading plugins.ini: %s", inifile);
		// meta_errno should be already set in ini_startup()
		return false;
	}

	META_LOG("dll: Loading plugins...");
	for (int i = 0; i < endlist; i++)
	{
		if (plist[i].status < PL_VALID)
			continue;

		if (plist[i].load(PT_STARTUP) == true)
			n++;
		else
			// all plugins should be loadable at startup...
			META_ERROR("dll: Failed to load plugin '%s'", plist[i].file);
	}

	META_LOG("dll: Rebuilding callbacks...");
	meta_rebuild_callbacks();

	META_LOG("dll: Finished loading %d plugins", n);
	return true;
}

// Update list of loaded plugins from ini file, and load any new/changed plugins.
// meta_errno values:
//  - errno's from ini_refresh()
bool MPluginList::refresh(PLUG_LOADTIME now)
{
	int i, ndone = 0, nkept = 0, nloaded = 0, nunloaded = 0, nreloaded = 0, ndelayed = 0;
	MPlugin* iplug;

	if (!ini_refresh())
	{
		META_ERROR("dll: Problem reloading plugins.ini: %s", inifile);
		// meta_errno should be already set in ini_refresh()
		return false;
	}

	META_LOG("dll: Updating plugins...");
	for (i = 0; i < endlist; i++)
	{
		iplug = &plist[i];
		if (iplug->status < PL_VALID)
			continue;

		switch (iplug->action)
		{
		case PA_KEEP:
			META_DEBUG(1, ("Keeping plugin '%s'", iplug->desc));
			iplug->action = PA_NONE;
			nkept++;
			break;
		case PA_LOAD:
			META_DEBUG(1, ("Loading plugin '%s'", iplug->desc));
			if (iplug->load(now))
				nloaded++;
			else if (meta_errno == ME_DELAYED)
				ndelayed++;
			break;
		case PA_RELOAD:
			META_DEBUG(1, ("Reloading plugin '%s'", iplug->desc));
			if (iplug->reload(now, PNL_FILE_NEWER))
				nreloaded++;
			else if (meta_errno == ME_DELAYED)
				ndelayed++;
			break;
		case PA_NONE:
			// If previously loaded from ini, but apparently removed from new ini.
			if (iplug->source == PS_INI && iplug->status >= PL_RUNNING)
			{
				META_DEBUG(1, ("Unloading plugin '%s'", iplug->desc));
				iplug->action = PA_UNLOAD;
				if (iplug->unload(now, PNL_INI_DELETED, PNL_INI_DELETED))
					nunloaded++;
				else if (meta_errno == ME_DELAYED)
					ndelayed++;
			}
			break;
		case PA_ATTACH:
			// Previously requested attach, but was delayed?
			META_DEBUG(1, ("Retrying attach plugin '%s'", iplug->desc));
			if (iplug->retry(now, PNL_DELAYED))
				nloaded++;
			else if (meta_errno == ME_DELAYED)
				ndelayed++;
			break;
		case PA_UNLOAD:
			// Previously requested unload, but was delayed?
			META_DEBUG(1, ("Retrying unload plugin '%s'", iplug->desc));
			if (iplug->retry(now, PNL_DELAYED))
				nunloaded++;
			else if (meta_errno == ME_DELAYED)
				ndelayed++;
			break;
		case PA_NULL:
			META_ERROR("dll: Unexpected action for plugin '%s': '%s'", iplug->desc, iplug->str_action());
			break;
		default:
			META_ERROR("dll: Unrecognized action for plugin '%s': '%s'", iplug->desc, iplug->str_action());
			break;
		}
		ndone++;
	}

	META_LOG("dll: Rebuilding callbacks...");
	meta_rebuild_callbacks();

	META_LOG("dll: Finished updating %d plugins; kept %d, loaded %d, unloaded %d, reloaded %d, delayed %d", ndone, nkept, nloaded, nunloaded, nreloaded, ndelayed);
	return true;
}

// Re-enable any plugins currently paused.
// meta_errno values:
//  - none
void MPluginList::unpause_all(void)
{
	for (int i = 0; i < endlist; i++)
	{
		auto iplug = &plist[i];
		if (iplug->status == PL_PAUSED)
			iplug->unpause();
	}
}

// Retry any pending actions on plugins, for instance load/unload delayed
// until changelevel.
// meta_errno values:
//  - none
void MPluginList::retry_all(PLUG_LOADTIME now)
{
	for (int i = 0; i < endlist; i++)
	{
		auto iplug = &plist[i];
		if (iplug->action != PA_NONE)
			iplug->retry(now, PNL_DELAYED);
	}
}

// List plugins and information about them in a formatted table.
// meta_errno values:
//  - none
void MPluginList::show(int source_index)
{
	int n = 0, r = 0;
	MPlugin *pl;
	char desc[15 + 1], file[16 + 1], vers[7 + 1];		// plus 1 for term null

	if (source_index <= 0)
		META_CONS("Currently loaded plugins:");
	else
		META_CONS("Child plugins:");

	META_CONS("  %*s  %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s", WIDTH_MAX_PLUGINS, "", sizeof(desc) - 1, "description", "stat", "pend",
			sizeof(file) - 1, "file", sizeof(vers) - 1, "ers", 2 + WIDTH_MAX_PLUGINS, "src", "load ", "unlod");

	for (int i = 0; i < endlist; i++)
	{
		pl = &plist[i];
		if (pl->status < PL_VALID)
			continue;

		if (source_index > 0 && pl->source_plugin_index != source_index)
			continue;

		Q_strncpy(desc, pl->desc, sizeof(desc) - 1);
		desc[sizeof(desc) - 1] = '\0';

		Q_strncpy(file, pl->file, sizeof(file) - 1);
		file[sizeof(file) - 1] = '\0';

		if (pl->info && pl->info->version)
		{
			Q_strncpy(vers, pl->info->version, sizeof(vers) - 1);
			vers[sizeof(vers) - 1] = '\0';
		}
		else
		{
			Q_strncpy(vers, " -", sizeof(vers) - 1);
			vers[sizeof(vers) - 1] = '\0';
		}

		META_CONS(" [%*d] %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s", WIDTH_MAX_PLUGINS, pl->index,
			sizeof(desc) - 1, desc, pl->str_status(ST_SHOW), pl->str_action(SA_SHOW), sizeof(file) - 1, file, sizeof(vers) - 1, vers,
			2 + WIDTH_MAX_PLUGINS, pl->str_source(SO_SHOW), pl->str_loadable(SL_SHOW), pl->str_unloadable(SL_SHOW));

		if (pl->status == PL_RUNNING)
			r++;
		n++;
	}

	META_CONS("%d plugins, %d running", n, r);
}

// List plugins and information to Player/client entity.  Differs from the
// "meta list" console command in that:
//  - Shows only "running" plugins, skipping any failed or paused plugins.
//  - Limited info about each plugin, mostly the "public" info (name, author,
//    etc).
// meta_errno values:
//  - none
void MPluginList::show_client(edict_t *pEntity)
{
	int n = 0;
	MPlugin *pl;
	META_CLIENT(pEntity, "Currently running plugins:");

	for (int i = 0; i < endlist; i++)
	{
		pl = &plist[i];
		if (pl->status != PL_RUNNING || !pl->info)
			continue;

		n++;
		META_CLIENT(pEntity, " [%3d] %s, v%s, %s, by %s, see %s", n,
			pl->info->name ? pl->info->name : "<unknown>",
			pl->info->version ? pl->info->version : "<?>",
			pl->info->date ? pl->info->date : "<../../..>",
			pl->info->author ? pl->info->author : "<unknown>",
			pl->info->url ? pl->info->url : "<unknown>");
	}

	META_CLIENT(pEntity, "%d plugins", n);
}

bool MPluginList::found_child_plugins(int source_index) const
{
	if (source_index <= 0)
		return false;

	for (int i = 0; i < endlist; i++)
	{
		if (plist[i].status < PL_VALID)
			continue;

		if (plist[i].source_plugin_index == source_index)
			return true;
	}

	return false;
}

void MPluginList::clear_source_plugin_index(int source_index)
{
	if (source_index <= 0)
		return;

	for (int i = 0; i < endlist; i++)
	{
		if (plist[i].status < PL_VALID)
			continue;

		if (plist[i].source_plugin_index == source_index)
			plist[i].source_plugin_index = -1;
	}
}
