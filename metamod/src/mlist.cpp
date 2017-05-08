#include "precompiled.h"

// Constructor
MPluginList::MPluginList(const char* ifile) : m_max_loaded_count(0)
{
	// store filename of ini file
	Q_strncpy(m_inifile, ifile, sizeof m_inifile - 1);
	m_inifile[sizeof m_inifile - 1] = '\0';

	// initialize array
	Q_memset(m_plist, 0, sizeof m_plist);
	for (int i = 0; i < MAX_PLUGINS; i++)
	{
		m_plist[i].m_index = i + 1; // 1-based
	}

	m_max_loaded_count = 0;
}

MPlugin* MPluginList::getlist()
{
	return m_plist;
}

int MPluginList::getmaxcount() const
{
	return m_max_loaded_count;
}

// Find a plugin based on the plugin handle.
MPlugin *MPluginList::find(module_handle_t handle)
{
	if (!handle)
		return nullptr;

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		if (m_plist[i].m_status < PL_VALID)
			continue;
		if (handle == m_plist[i].m_sys_module.gethandle())
			return &m_plist[i];
	}

	return nullptr;
}

// Find a plugin based on the plugin index #.
MPlugin *MPluginList::find(int pindex)
{
	if (pindex <= 0)
		return nullptr;

	auto pfound = &m_plist[pindex - 1];
	if (pfound->m_status < PL_VALID)
		return nullptr;
	
	return pfound;
}

// Find a plugin with the given plid.
MPlugin *MPluginList::find(plid_t id)
{
	if (!id)
		return nullptr;

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		if (m_plist[i].m_status < PL_VALID)
			continue;

		if (m_plist[i].m_info == id)
			return &m_plist[i];
	}

	return nullptr;
}

// Find a plugin with the given pathname.
MPlugin *MPluginList::find(const char* findpath)
{
	if (!findpath)
		return nullptr;

	META_DEBUG(8, "Looking for loaded plugin with path: %s", findpath);

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		META_DEBUG(9, "Looking at: plugin %s loadedpath: %s", m_plist[i].m_file, m_plist[i].m_pathname);

		if (m_plist[i].m_status < PL_VALID)
			continue;

		if (!Q_strcmp(m_plist[i].m_pathname, findpath))
		{
			META_DEBUG(8, "Found loaded plugin %s", m_plist[i].m_file);
			return &m_plist[i];
		}
	}

	META_DEBUG(8, "No loaded plugin found with path: %s", findpath);
	return nullptr;
}

// Find a plugin that uses the given memory location.
MPlugin *MPluginList::find_memloc(void *memptr)
{
	for (int i = 0; i < m_max_loaded_count; i++) {
		auto iplug = &m_plist[i];

		if (iplug->m_sys_module.contain(memptr))
			return iplug;
	}

	return nullptr;
}

// Find a plugin with non-ambiguous prefix string matching desc, file,
// name, or logtag.
MPlugin *MPluginList::find_match(const char *prefix, bool& unique)
{
	if (!prefix) {
		return nullptr;
	}

	MPlugin* pfound = nullptr;
	size_t len = Q_strlen(prefix);

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto plug = &m_plist[i];
		if (plug->m_status < PL_VALID)
			continue;

		if (plug->m_info && !Q_strnicmp(plug->m_info->name, prefix, len)
			|| !Q_strnicmp(plug->m_desc, prefix, len)
			|| !Q_strnicmp(plug->m_file, prefix, len)
			|| plug->m_info && !Q_strnicmp(plug->m_info->logtag, prefix, len))
		{
			if (pfound) {
				unique = false;
				break;
			}

			pfound = plug;
			unique = true;
		}
	}

	return pfound;
}

// Find a plugin with same file, logtag, desc or significant
// prefix of file. Uses the platform_match() method of MPlugin.
MPlugin *MPluginList::find_match(MPlugin* pmatch)
{
	if (!pmatch) {
		return nullptr;
	}

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto plug = &m_plist[i];

		if (pmatch->platform_match(plug)) {
			return plug;
		}
	}

	return nullptr;
}

MPlugin* MPluginList::plugin_addload(plid_t plid, const char* fname, PLUG_LOADTIME now)
{
	auto pl_loader = find(plid);
	if (!pl_loader) {
		META_DEBUG(1, "Couldn't find plugin that gave this loading request!");
		return nullptr;
	}

	MPlugin pl_temp = {};
	if (!pl_temp.plugin_parseline(fname, pl_loader->m_index)) {
		return nullptr;
	}

	if (pl_temp.resolve() != true) {
		META_DEBUG(1, "Couldn't resolve given path into a file: %s", pl_temp.m_file);
		return nullptr;
	}

	auto pl_found = find(pl_temp.m_pathname);
	if (pl_found)
	{
		META_DEBUG(1, "Plugin '%s' already in current list; file=%s desc='%s'", pl_temp.m_file, pl_found->m_file, pl_found->m_desc);
		return pl_found;
	}

	auto pl_added = add(&pl_temp);
	if (!pl_added)
	{
		META_DEBUG(1, "Couldn't add plugin '%s' to list; see log", pl_temp.m_desc);
		return nullptr;
	}

	pl_added->m_action = PA_LOAD;
	if (!pl_added->load(now))
	{
		if (pl_added->m_status == PL_OPENED)
		{
			META_DEBUG(1, "Opened plugin '%s', but failed to attach; see log", pl_added->m_desc);
		}
		else
		{
			META_DEBUG(1, "Couldn't load plugin '%s'; see log", pl_added->m_desc);
		}
		return nullptr;
	}

	META_DEBUG(1, "Loaded plugin '%s' successfully", pl_added->m_desc);

	return pl_added;
}

MPlugin* MPluginList::find_empty_slot()
{
	for (int i = 0; i < MAX_PLUGINS; i++) {
		if (m_plist[i].m_status == PL_EMPTY) {
			if (i >= m_max_loaded_count)
				m_max_loaded_count = i + 1;

			return &m_plist[i];
		}
	}

	return nullptr;
}

// Add a plugin to the list.
MPlugin* MPluginList::add(MPlugin* padd)
{
	auto iplug = find_empty_slot();

	// couldn't find a slot to use
	if (!iplug)
	{
		META_ERROR("Couldn't add plugin '%s' to list; reached max plugins (%d)", padd->m_file, MAX_PLUGINS);
		return nullptr;
	}

	// copy filename into this free slot
	Q_strncpy(iplug->m_filename, padd->m_filename, sizeof iplug->m_filename - 1);
	iplug->m_filename[sizeof iplug->m_filename - 1] = '\0';

	// Copy file offset ptr.
	// Can't just copy ptr, as it points to offset in padd, which will go
	// away; need to point to corresponding offset in iplug.
	iplug->m_file = iplug->m_filename + (padd->m_file - padd->m_filename);

	// copy description
	Q_strncpy(iplug->m_desc, padd->m_desc, sizeof iplug->m_desc - 1);
	iplug->m_desc[sizeof iplug->m_desc - 1] = '\0';

	// copy pathname
	Q_strncpy(iplug->m_pathname, padd->m_pathname, sizeof iplug->m_pathname - 1);
	iplug->m_pathname[sizeof iplug->m_pathname - 1] = '\0';
	NormalizePath(iplug->m_pathname);

	iplug->m_source = padd->m_source;
	iplug->m_status = padd->m_status;
	iplug->m_source_plugin_index = padd->m_source_plugin_index;

	return iplug;
}


// Read plugins.ini at server startup.
bool MPluginList::ini_startup()
{
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin *pmatch;

	if (!FileExistsInGameDir(m_inifile))
	{
		META_ERROR("ini: Metamod plugins file empty or missing: %s", m_inifile);
		return false;
	}

	full_gamedir_path(m_inifile, m_inifile);
	FILE* fp = fopen(m_inifile, "r");
	if (!fp)
	{
		META_ERROR("ini: Unable to open plugins file '%s': %s", m_inifile, strerror(errno));
		return false;
	}

	META_LOG("ini: Begin reading plugins list: %s", m_inifile);
	for (n = 0, ln = 1; !feof(fp) && fgets(line, sizeof line, fp) && n < MAX_PLUGINS; ln++)
	{
		// Remove line terminations.
		char* cp;
		if ((cp = Q_strrchr(line, '\r')))
			*cp = '\0';

		if ((cp = Q_strrchr(line, '\n')))
			*cp = '\0';

		// Parse directly into next entry in array
		if (!m_plist[n].ini_parseline(line))
		{
			continue;
		}
		
		// Check for a duplicate - an existing entry with this pathname.
		if (find(m_plist[n].m_pathname))
		{
			// Should we check platform specific level here?
			META_INFO("ini: Skipping duplicate plugin, line %d of %s: %s", ln, m_inifile, m_plist[n].m_pathname);
			continue;
		}

		// Check for a matching platform with different platform specifics
		// level.
		if (nullptr != (pmatch = find_match(&m_plist[n])))
		{
			if (pmatch->m_pfspecific >= m_plist[n].m_pfspecific)
			{
				META_DEBUG(1, "ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)", ln, m_inifile, pmatch->m_pfspecific, m_plist[n].m_pfspecific);
				continue;
			}

			META_DEBUG(1, "ini: Plugin in line %d overrides existing plugin with lower platform specific level %d, ours %d", ln, pmatch->m_pfspecific, m_plist[n].m_pfspecific);
			int _index = pmatch->m_index;
			Q_memset(pmatch, 0, sizeof(MPlugin));
			pmatch->m_index = _index;
		}
		m_plist[n].m_action = PA_LOAD;
		META_LOG("ini: Read plugin config for: %s", m_plist[n].m_desc);
		n++;
		m_max_loaded_count = n; // mark end of list
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins to load", m_inifile, n);

	fclose(fp);
	if (!n)
	{
		META_ERROR("ini: Warning; no plugins found to load?");
	}

	return true;
}

// Re-read plugins.ini looking for added/deleted/changed plugins.
bool MPluginList::ini_refresh()
{
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added;

	FILE* fp = fopen(m_inifile, "r");
	if (!fp)
	{
		META_ERROR("ini: Unable to open plugins file '%s': %s", m_inifile, strerror(errno));
		return false;
	}

	META_LOG("ini: Begin re-reading plugins list: %s", m_inifile);
	for (n = 0 , ln = 1; !feof(fp) && fgets(line, sizeof line, fp) && n < MAX_PLUGINS; ln++)
	{
		// Remove line terminations.
		char *cp;
		if ((cp = Q_strrchr(line, '\r')))
			*cp = '\0';

		if ((cp = Q_strrchr(line, '\n')))
			*cp = '\0';

		// Parse into a temp plugin
		Q_memset(&pl_temp, 0, sizeof pl_temp);
		if (!pl_temp.ini_parseline(line))
		{
			META_ERROR("ini: Skipping malformed line %d of %s",ln, m_inifile);
			continue;
		}
		// Try to find plugin with this pathname in the current list of
		// plugins.
		if (!(pl_found = find(pl_temp.m_pathname)))
		{
			// Check for a matching platform with higher platform specifics
			// level.
			if (nullptr != (pl_found = find_match(&pl_temp)))
			{
				if (pl_found->m_pfspecific >= pl_temp.m_pfspecific)
				{
					META_DEBUG(1, "ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)", ln, m_inifile, pl_found->m_pfspecific, pl_temp.m_pfspecific);
					continue;
				}
				if (PA_LOAD == pl_found->m_action)
				{
					META_DEBUG(1, "ini: Plugin in line %d overrides loading of plugin with lower platform specific level %d, ours %d", ln, pl_found->m_pfspecific, pl_temp.m_pfspecific);
					int _index = pl_found->m_index;
					Q_memset(pl_found, 0, sizeof(MPlugin));
					pl_found->m_index = _index;
				}
				else
				{
					META_DEBUG(1, "ini: Plugin in line %d should override existing plugin with lower platform specific level %d, ours %d. Unable to comply.", ln, pl_found->m_pfspecific, pl_temp.m_pfspecific);
					continue;
				}
			}
			// new plugin; add to list
			if ((pl_added = add(&pl_temp)))
			{
				// try to load this plugin at the next opportunity
				pl_added->m_action = PA_LOAD;
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
			if (pl_temp.m_desc[0] != '<')
			{
				Q_strncpy(pl_found->m_desc, pl_temp.m_desc, sizeof pl_found->m_desc - 1);
				pl_found->m_desc[sizeof pl_found->m_desc - 1] = '\0';
			}

			// Check the file to see if it looks like it's been modified
			// since we last loaded it.
			if (!pl_found->newer_file())
			{
				pl_found->m_action = PA_KEEP;
			}
			// Newer file on disk.
			else if (pl_found->m_status >= PL_OPENED)
			{
				META_DEBUG(2, "ini: Plugin '%s' has newer file on disk", pl_found->m_desc);
				pl_found->m_action = PA_RELOAD;
			}
			else
				META_ERROR("ini: Plugin '%s' has newer file, but unexpected status (%s)", pl_found->m_desc, pl_found->str_status());
		}

		if (pl_found)
		{
			META_LOG("ini: Read plugin config for: %s", pl_found->m_desc);
		}
		else
		{
			META_LOG("ini: Read plugin config for: %s", pl_temp.m_desc);
		}
		n++;
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins", m_inifile, n);

	fclose(fp);
	if (!n)
	{
		META_ERROR("ini: Warning; no plugins found to load?");
	}

	return true;
}

// Load a plugin from a console command.
bool MPluginList::cmd_addload(const char* args)
{
	MPlugin pl_temp = {};
	MPlugin *pl_found, *pl_added;

	if (pl_temp.cmd_parseline(args) != true)
	{
		META_CONS("Couldn't parse 'meta load' arguments: %s", args);
		return false;
	}

	// resolve given path into a file; accepts various "shortcut"
	// pathnames.
	if (pl_temp.resolve() != true)
	{
		// Couldn't find a matching file on disk
		META_CONS("Couldn't resolve given path into a file: %s", pl_temp.m_file);
		return false;
	}

	// Try to find plugin with this pathname in the current list of
	// plugins.
	if ((pl_found = find(pl_temp.m_pathname)))
	{
		// Already in list
		META_CONS("Plugin '%s' already in current list; file=%s desc='%s'", pl_temp.m_file, pl_found->m_file, pl_found->m_desc);
		return false;
	}
	// new plugin; add to list
	if (!(pl_added = add(&pl_temp)))
	{
		META_CONS("Couldn't add plugin '%s' to list; see log", pl_temp.m_desc);
		return false;
	}

	// try to load new plugin
	pl_added->m_action = PA_LOAD;
	if (!pl_added->load(PT_ANYTIME))
	{
		// load failed
		if (pl_added->m_status == PL_OPENED)
			META_CONS("Opened plugin '%s', but failed to attach; see log", pl_added->m_desc);
		else
			META_CONS("Couldn't load plugin '%s'; see log", pl_added->m_desc);

		show(0);
		return false;
	}

	META_CONS("Loaded plugin '%s' successfully", pl_added->m_desc);
	META_CONS("Rebuilding callbacks...");
	meta_rebuild_callbacks();
	show(0);

	return true;
}

// Load plugins at startup.
bool MPluginList::load()
{
	int n = 0;
	if (!ini_startup())
	{
		META_ERROR("Problem loading plugins.ini: %s", m_inifile);
		return false;
	}

	META_LOG("dll: Loading plugins...");
	for (int i = 0; i < m_max_loaded_count; i++)
	{
		if (m_plist[i].m_status < PL_VALID)
			continue;

		if (m_plist[i].load(PT_STARTUP) == true)
			n++;
		else
			// all plugins should be loadable at startup...
			META_ERROR("dll: Failed to load plugin '%s'", m_plist[i].m_file);
	}

	META_LOG("dll: Rebuilding callbacks...");
	meta_rebuild_callbacks();

	META_LOG("dll: Finished loading %d plugins", n);
	return true;
}

// Update list of loaded plugins from ini file, and load any new/changed plugins.
bool MPluginList::refresh(PLUG_LOADTIME now)
{
	int ndone = 0, nkept = 0, nloaded = 0, nunloaded = 0, nreloaded = 0, ndelayed = 0;

	if (!ini_refresh())
	{
		META_ERROR("dll: Problem reloading plugins.ini: %s", m_inifile);
		return false;
	}

	META_LOG("dll: Updating plugins...");
	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto iplug = &m_plist[i];
		if (iplug->m_status < PL_VALID)
			continue;

		switch (iplug->m_action)
		{
		case PA_KEEP:
			META_DEBUG(1, "Keeping plugin '%s'", iplug->m_desc);
			iplug->m_action = PA_NONE;
			nkept++;
			break;
		case PA_LOAD:
			META_DEBUG(1, "Loading plugin '%s'", iplug->m_desc);
			if (iplug->load(now))
				nloaded++;
			/*else if (meta_errno == ME_DELAYED) TODO
				ndelayed++;*/
			break;
		case PA_RELOAD:
			META_DEBUG(1, "Reloading plugin '%s'", iplug->m_desc);
			if (iplug->reload(now, PNL_FILE_NEWER))
				nreloaded++;
			/*else if (meta_errno == ME_DELAYED) TODO
				ndelayed++;*/
			break;
		case PA_NONE:
			// If previously loaded from ini, but apparently removed from new ini.
			if (iplug->m_source == PS_INI && iplug->m_status >= PL_RUNNING)
			{
				META_DEBUG(1, "Unloading plugin '%s'", iplug->m_desc);
				iplug->m_action = PA_UNLOAD;
				if (iplug->unload(now, PNL_INI_DELETED, PNL_INI_DELETED))
					nunloaded++;
				/*else if (meta_errno == ME_DELAYED) TODO
					ndelayed++;*/
			}
			break;
		case PA_ATTACH:
			// Previously requested attach, but was delayed?
			META_DEBUG(1, "Retrying attach plugin '%s'", iplug->m_desc);
			if (iplug->retry(now, PNL_DELAYED))
				nloaded++;
			/*else if (meta_errno == ME_DELAYED) TODO
				ndelayed++;*/
			break;
		case PA_UNLOAD:
			// Previously requested unload, but was delayed?
			META_DEBUG(1, "Retrying unload plugin '%s'", iplug->m_desc);
			if (iplug->retry(now, PNL_DELAYED))
				nunloaded++;
			/*else if (meta_errno == ME_DELAYED) TODO
				ndelayed++;*/
			break;
		case PA_NULL:
			META_ERROR("dll: Unexpected action for plugin '%s': '%s'", iplug->m_desc, iplug->str_action());
			break;
		default:
			META_ERROR("dll: Unrecognized action for plugin '%s': '%s'", iplug->m_desc, iplug->str_action());
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
void MPluginList::unpause_all()
{
	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto iplug = &m_plist[i];
		if (iplug->m_status == PL_PAUSED)
			iplug->unpause();
	}
}

// Retry any pending actions on plugins, for instance load/unload delayed
// until changelevel.
void MPluginList::retry_all(PLUG_LOADTIME now)
{
	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto iplug = &m_plist[i];
		if (iplug->m_action != PA_NONE)
			iplug->retry(now, PNL_DELAYED);
	}
}

// List plugins and information about them in a formatted table.
void MPluginList::show(int source_index)
{
	int n = 0, r = 0;
	char desc[15 + 1], file[16 + 1], vers[7 + 1];		// plus 1 for term null

	if (source_index <= 0)
		META_CONS("Currently loaded plugins:");
	else
		META_CONS("Child plugins:");

	META_CONS("  %*s  %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s", WIDTH_MAX_PLUGINS, "", sizeof desc - 1, "description", "stat", "pend",
			sizeof file - 1, "file", sizeof vers - 1, "ers", 2 + WIDTH_MAX_PLUGINS, "src", "load ", "unlod");

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto pl = &m_plist[i];
		if (pl->m_status < PL_VALID)
			continue;

		if (source_index > 0 && pl->m_source_plugin_index != source_index)
			continue;

		Q_strncpy(desc, pl->m_desc, sizeof desc - 1);
		desc[sizeof desc - 1] = '\0';

		Q_strncpy(file, pl->m_file, sizeof file - 1);
		file[sizeof file - 1] = '\0';

		if (pl->m_info && pl->m_info->version)
		{
			Q_strncpy(vers, pl->m_info->version, sizeof vers - 1);
			vers[sizeof vers - 1] = '\0';
		}
		else
		{
			Q_strncpy(vers, " -", sizeof vers - 1);
			vers[sizeof vers - 1] = '\0';
		}

		META_CONS(" [%*d] %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s", WIDTH_MAX_PLUGINS, pl->m_index,
			sizeof desc - 1, desc, pl->str_status(ST_SHOW), pl->str_action(SA_SHOW), sizeof file - 1, file, sizeof vers - 1, vers,
			2 + WIDTH_MAX_PLUGINS, pl->str_source(SO_SHOW), pl->str_loadable(SL_SHOW), pl->str_unloadable(SL_SHOW));

		if (pl->m_status == PL_RUNNING)
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
void MPluginList::show_client(edict_t *pEntity)
{
	int n = 0;
	META_CLIENT(pEntity, "Currently running plugins:");

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		auto pl = &m_plist[i];
		if (pl->m_status != PL_RUNNING || !pl->m_info)
			continue;

		n++;
		META_CLIENT(pEntity, " [%3d] %s, v%s, %s, by %s, see %s", n,
			pl->m_info->name ? pl->m_info->name : "<unknown>",
			pl->m_info->version ? pl->m_info->version : "<?>",
			pl->m_info->date ? pl->m_info->date : "<../../..>",
			pl->m_info->author ? pl->m_info->author : "<unknown>",
			pl->m_info->url ? pl->m_info->url : "<unknown>");
	}

	META_CLIENT(pEntity, "%d plugins", n);
}

bool MPluginList::found_child_plugins(int source_index) const
{
	if (source_index <= 0)
		return false;

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		if (m_plist[i].m_status < PL_VALID)
			continue;

		if (m_plist[i].m_source_plugin_index == source_index)
			return true;
	}

	return false;
}

void MPluginList::clear_source_plugin_index(int source_index)
{
	if (source_index <= 0)
		return;

	for (int i = 0; i < m_max_loaded_count; i++)
	{
		if (m_plist[i].m_status < PL_VALID)
			continue;

		if (m_plist[i].m_source_plugin_index == source_index)
			m_plist[i].m_source_plugin_index = -1;
	}
}
