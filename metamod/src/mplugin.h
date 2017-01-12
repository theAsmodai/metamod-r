#pragma once

#include "api_info.h"
#include "support_meta.h"

// Flags to indicate current "load" state of plugin.
// NOTE: order is important, as greater/less comparisons are made.
enum PLUG_STATUS : uint8
{
	PL_EMPTY = 0,		// empty slot
	PL_VALID,			// has valid info in it
	PL_BADFILE,			// nonexistent file (open failed), or not a valid plugin file (query failed)
	PL_OPENED,			// dlopened and queried
	PL_FAILED,			// opened, but failed to attach or unattach
	PL_RUNNING,			// attached and running
	PL_PAUSED,			// attached but paused
};

// Action to take for plugin at next opportunity.
enum PLUG_ACTION : uint8
{
	PA_NULL = 0,
	PA_NONE,			// no action needed right now
	PA_KEEP,			// keep, after ini refresh
	PA_LOAD,			// load (dlopen, query) and try to attach
	PA_ATTACH,			// attach
	PA_UNLOAD,			// unload (detach, dlclose)
	PA_RELOAD,			// unload and load again
};

// Flags to indicate from where the plugin was loaded.
enum PLOAD_SOURCE : uint8
{
	PS_INI = 0,			// was loaded from the plugins.ini
	PS_CMD,				// was loaded via a server command
	PS_PLUGIN,			// was loaded by other plugin
};

// Flags for how to word description of plugin loadtime.
enum STR_LOADTIME : uint8
{
	SL_SIMPLE = 0,			// single word
	SL_SHOW,			// for "show" output, 5 chars
	SL_ALLOWED,			// when plugin is allowed to load/unload
	SL_NOW,				// current situation
};

// Flags for how to format description of status.
enum STR_STATUS : uint8
{
	ST_SIMPLE = 0,			// single word
	ST_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of action.
enum STR_ACTION : uint8
{
	SA_SIMPLE = 0,			// single word
	SA_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of source.
enum STR_SOURCE : uint8
{
	SO_SIMPLE = 0,			// two words
	SO_SHOW,			// for "list" output, 3 chars
};

// An individual plugin.
class MPlugin {
public:
	PLUG_STATUS m_status;				// current status of plugin (loaded, etc)
	PLUG_ACTION m_action;				// what to do with plugin (load, unload, etc)
	PLOAD_SOURCE m_source;				// source of the request to load the plugin
	int m_index;					// 1-based
	plugin_info_t *m_info;				// information plugin provides about itself
	CSysModule m_sys_module;
	time_t m_time_loaded;				// when plugin was loaded
	int m_source_plugin_index;			// who loaded this plugin
	int m_unloader_index;
	bool m_is_unloader;				// fix to prevent other plugins unload active unloader.

	DLL_FUNCTIONS *m_dllapi_table;
	DLL_FUNCTIONS *m_dllapi_post_table;
	NEW_DLL_FUNCTIONS *m_newapi_table;
	NEW_DLL_FUNCTIONS *m_newapi_post_table;
	enginefuncs_t *m_engine_table;
	enginefuncs_t *m_engine_post_table;

	gamedll_funcs_t m_gamedll_funcs;
	mutil_funcs_t m_mutil_funcs;

	char m_filename[PATH_MAX];			// ie "dlls/mm_test_i386.so", from inifile
	char *m_file;					// ie "mm_test_i386.so", ptr from filename
	char m_desc[MAX_DESC_LEN];			// ie "Test metamod plugin", from inifile
	char m_pathname[PATH_MAX];			// UNIQUE, ie "/home/willday/half-life/cstrike/dlls/mm_test_i386.so", built with GameDLL.gamedir
	int m_pfspecific;					// level of specific platform affinity, used during load time

	bool ini_parseline(char *line);				// parse line from inifile
	bool cmd_parseline(const char *line);				// parse from console command
	bool plugin_parseline(const char *fname, int loader_index); 	// parse from plugin
	bool check_input();

	bool resolve();						// find a matching file on disk
	char *resolve_dirs(char *path) const;
	char *resolve_prefix(char *path) const;
	char *resolve_suffix(char *path) const;

	bool platform_match(MPlugin* plugin) const;

	bool load(PLUG_LOADTIME now);
	bool unload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason, PL_UNLOAD_REASON real_reason);
	bool reload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
	bool pause();
	bool unpause();
	bool retry(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);			// if previously failed
	bool clear();
	bool plugin_unload(plid_t plid, PLUG_LOADTIME now, PL_UNLOAD_REASON reason);	// other plugin unloading
	void show();									// print info about plugin to console

	bool newer_file() const;								// check for newer file on disk

	const char *str_status(STR_STATUS fmt) const;
	const char *str_action(STR_ACTION fmt) const;
	const char *str_source(STR_SOURCE fmt) const;

	const char *str_reason(PL_UNLOAD_REASON preason, PL_UNLOAD_REASON preal_reason) const;
	static const char *str_loadtime(PLUG_LOADTIME pallow, STR_LOADTIME fmt);

	const char *str_status()		{ return str_status(ST_SIMPLE); };
	const char *str_action()		{ return str_action(SA_SIMPLE); };
	const char *str_source()		{ return str_source(SO_SIMPLE); };

	const char *str_loadable() {
		if (m_info) return str_loadtime(m_info->loadable, SL_SIMPLE);
		else return " -";
	};
	const char *str_unloadable() {
		if (m_info) return str_loadtime(m_info->unloadable, SL_SIMPLE);
		else return " -";
	};
	const char *str_loadable(STR_LOADTIME fmt) {
		if (m_info) return str_loadtime(m_info->loadable, fmt);
		else return " -";
	};
	const char *str_unloadable(STR_LOADTIME fmt) {
		if (m_info) return str_loadtime(m_info->unloadable, fmt);
		else return " -";
	};

private:
	bool query();
	bool attach(PLUG_LOADTIME now);
	bool detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
};
