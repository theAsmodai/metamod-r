#pragma once

#include "api_info.h"
#include "support_meta.h"

// Flags to indicate current "load" state of plugin.
// NOTE: order is important, as greater/less comparisons are made.
enum PLUG_STATUS {
	PL_EMPTY = 0,			// empty slot
	PL_VALID,			// has valid info in it
	PL_BADFILE,			// nonexistent file (open failed),
					//    or not a valid plugin file (query failed)
	PL_OPENED,			// dlopened and queried
	PL_FAILED,			// opened, but failed to attach or unattach
	PL_RUNNING,			// attached and running
	PL_PAUSED,			// attached but paused
};

// Action to take for plugin at next opportunity.
enum PLUG_ACTION {
	PA_NULL = 0,
	PA_NONE,			// no action needed right now
	PA_KEEP,			// keep, after ini refresh
	PA_LOAD,			// load (dlopen, query) and try to attach
	PA_ATTACH,			// attach
	PA_UNLOAD,			// unload (detach, dlclose)
	PA_RELOAD,			// unload and load again
};

// Flags to indicate from where the plugin was loaded.
enum PLOAD_SOURCE {
	PS_INI = 0,			// was loaded from the plugins.ini
	PS_CMD,				// was loaded via a server command
	PS_PLUGIN,			// was loaded by other plugin
};

// Flags for how to word description of plugin loadtime.
enum STR_LOADTIME {
	SL_SIMPLE = 0,			// single word
	SL_SHOW,			// for "show" output, 5 chars
	SL_ALLOWED,			// when plugin is allowed to load/unload
	SL_NOW,				// current situation
};

// Flags for how to format description of status.
enum STR_STATUS {
	ST_SIMPLE = 0,			// single word
	ST_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of action.
enum STR_ACTION {
	SA_SIMPLE = 0,			// single word
	SA_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of source.
enum STR_SOURCE {
	SO_SIMPLE = 0,			// two words
	SO_SHOW,			// for "list" output, 3 chars
};

// api table list
struct api_tables_t {
	enginefuncs_t *engine;
	DLL_FUNCTIONS *dllapi;
	NEW_DLL_FUNCTIONS *newapi;
};

// An individual plugin.
class MPlugin: public class_metamod_new {
public:
	mBOOL ini_parseline(const char *line);		// parse line from inifile
	mBOOL cmd_parseline(const char *line);		// parse from console command
	mBOOL plugin_parseline(const char *fname, int loader_index); // parse from plugin
	mBOOL check_input();

	mBOOL resolve();				// find a matching file on disk
	char *resolve_dirs(const char *path);
	char *resolve_prefix(const char *path);
	char *resolve_suffix(const char *path);
	static mBOOL is_platform_postfix(const char *pf);

	mBOOL platform_match(MPlugin* plugin);

	mBOOL load(PLUG_LOADTIME now);
	mBOOL unload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason, PL_UNLOAD_REASON real_reason);
	mBOOL reload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
	mBOOL pause();
	mBOOL unpause();
	mBOOL retry(PLUG_LOADTIME now, PL_UNLOAD_REASON reason); // if previously failed
	void free_api_pointers();
	mBOOL clear();
	mBOOL plugin_unload(plid_t plid, PLUG_LOADTIME now, PL_UNLOAD_REASON reason); // other plugin unloading
	void show();				// print info about plugin to console

	mBOOL newer_file();			// check for newer file on disk

	const char *str_status(STR_STATUS fmt);
	const char *str_action(STR_ACTION fmt);
	const char *str_source(STR_SOURCE fmt);

	const char *str_reason(PL_UNLOAD_REASON preason, PL_UNLOAD_REASON preal_reason);
	const char *str_loadtime(PLUG_LOADTIME pallow, STR_LOADTIME fmt);

	inline const char *str_status()		{ return str_status(ST_SIMPLE); };
	inline const char *str_action()		{ return str_action(SA_SIMPLE); };
	inline const char *str_source()		{ return str_source(SO_SIMPLE); };

	inline const char *str_loadable() {
		return info ? str_loadtime(info->loadable, SL_SIMPLE) : " -";
	};
	inline const char *str_unloadable() {
		return info ? str_loadtime(info->unloadable, SL_SIMPLE) : " -";
	};
	inline const char *str_loadable(STR_LOADTIME fmt) {
		return info ? str_loadtime(info->loadable, fmt) : " -";
	};
	inline const char *str_unloadable(STR_LOADTIME fmt) {
		return info ? str_loadtime(info->unloadable, fmt) : " -";
	};

public:
	// reordered for faster api_hook.cpp functions
	PLUG_STATUS status;				// current status of plugin (loaded, etc)
	api_tables_t tables;
	api_tables_t post_tables;

	inline void *get_api_table(enum_api_t api) {
		return ((void **)&tables)[api];
	}
	inline void *get_api_post_table(enum_api_t api) {
		return ((void **)&post_tables)[api];
	}

	int index;					// 1-based
	int pfspecific;                  		// level of specific platform affinity, used during load time
	PLUG_ACTION action;				// what to do with plugin (load, unload, etc)
	PLOAD_SOURCE source;				// source of the request to load the plugin
	int source_plugin_index;			// index of plugin that loaded this plugin. -1 means source plugin has been unloaded.
	int unloader_index;
	mBOOL is_unloader;				// fix to prevent other plugins unload active unloader.

	DLHANDLE handle;				// handle for dlopen, dlsym, etc
	plugin_info_t *info;				// information plugin provides about itself
	time_t time_loaded;				// when plugin was loaded

	char filename[PATH_MAX];			// ie "dlls/mm_test_i386.so", from inifile
	char *file;					// ie "mm_test_i386.so", ptr from filename
	char desc[MAX_DESC_LEN];			// ie "Test metamod plugin", from inifile
	char pathname[PATH_MAX];			// UNIQUE, ie "/home/willday/half-life/cstrike/dlls/mm_test_i386.so", built with GameDLL.gamedir

private:
	mBOOL query();
	mBOOL attach(PLUG_LOADTIME now);
	mBOOL detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);

	gamedll_funcs_t gamedll_funcs;
	mutil_funcs_t mutil_funcs;
};

// Macros used by MPlugin::show(), to list the functions that the plugin
// catches.
#define SHOW_DEF_API(api_info, api_table, pre_str, post_str) \
	n=0; \
	{ \
		const api_info_t * ainfo = (const api_info_t *)&api_info; \
		const void ** table = (const void **)api_table; \
		for (int i = 0; &ainfo[i] < &api_info.END; i++) { \
			if (table[i]) { \
				META_CONS("%s%s%s", pre_str, ainfo[i].name, post_str); \
				n++; \
			} \
		} \
	}

#define SHOW_DEF_DLLAPI(api_table, pre_str, post_str) \
	SHOW_DEF_API(dllapi_info, api_table, pre_str, post_str)

#define SHOW_DEF_NEWAPI(api_table, pre_str, post_str) \
	SHOW_DEF_API(newapi_info, api_table, pre_str, post_str)

#define SHOW_DEF_ENGINE(api_table, pre_str, post_str) \
	SHOW_DEF_API(engine_info, api_table, pre_str, post_str)
