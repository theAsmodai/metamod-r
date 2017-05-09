#pragma once

#include "mplugin.h"		// class MPlugin
#include "plinfo.h"		// plid_t, etc

// Max number of plugins we can manage.  This is an arbitrary, fixed number,
// for convenience.  It would probably be better to dynamically grow the
// list as needed, but we do this for now.
#define MAX_PLUGINS 50

// Width required to printf above MAX, for show() functions.
#define WIDTH_MAX_PLUGINS	2

// A list of plugins.
class MPluginList
{
public:
	MPluginList(const char *ifile);

	MPlugin *getlist();
	int getmaxcount() const;

	MPlugin *find(int pindex);			// find by index
	MPlugin *find(const char *findpath);		// find by pathname
	MPlugin *find(plid_t id);			// find by plid_t
	MPlugin *find_memloc(void *memptr);		// find by memory location
	MPlugin *find_match(const char *prefix, bool& unique);	// find by partial prefix match
	MPlugin *find_match(MPlugin *pmatch);		// find by platform_match()
	MPlugin *find(module_handle_t handle);			// find by handle
	MPlugin *find_empty_slot();
	MPlugin *add(MPlugin *padd);

	bool found_child_plugins(int source_index) const;

	bool ini_startup();								// read inifile at startup
	bool ini_refresh();								// re-read inifile
	bool cmd_addload(const char *args);							// load from console command
	MPlugin *plugin_addload(plid_t plid, const char *fname, PLUG_LOADTIME now);		//load from plugin

	bool load();				// load the list, at startup
	bool refresh(PLUG_LOADTIME now);		// update from re-read inifile
	void unpause_all();				// unpause any paused plugins
	void retry_all(PLUG_LOADTIME now);		// retry any pending plugin actions
	void show(int source_index = 0);		// list plugins to console
	void show_client(edict_t *pEntity);		// list plugins to player client
	void clear_source_plugin_index(int source_index);

private:
	int m_max_loaded_count;					// index of last used entry
	MPlugin m_plist[MAX_PLUGINS];			// array of plugins
	char m_inifile[PATH_MAX];				// full pathname
};
