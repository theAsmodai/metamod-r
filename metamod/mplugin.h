// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mplugin.h - class and types to describe an individual plugin

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
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

#ifndef MPLUGIN_H
#define MPLUGIN_H

#include <time.h>			// time_t, etc
#include <malloc.h>			// malloc, etc

#include <eiface.h>			// DLL_FUNCTIONS, etc

#include "types_meta.h"			// mBOOL
#include "meta_api.h"			// GETENTITYAPI_FN, etc
#include "api_info.h"			// dllapi_info, etc
#include "support_meta.h"		// MAX_DESC_LEN
#include "osdep.h"
#include "new_baseclass.h"


// Flags to indicate current "load" state of plugin.
// NOTE: order is important, as greater/less comparisons are made.
typedef enum {
	PL_EMPTY = 0,			// empty slot
	PL_VALID,			// has valid info in it
	PL_BADFILE,			// nonexistent file (open failed), 
					//    or not a valid plugin file (query failed)
	PL_OPENED,			// dlopened and queried
	PL_FAILED,			// opened, but failed to attach or unattach
	PL_RUNNING,			// attached and running
	PL_PAUSED,			// attached but paused
} PLUG_STATUS;

// Action to take for plugin at next opportunity.
typedef enum {
	PA_NULL = 0,
	PA_NONE,			// no action needed right now
	PA_KEEP,			// keep, after ini refresh
	PA_LOAD,			// load (dlopen, query) and try to attach
	PA_ATTACH,			// attach
	PA_UNLOAD,			// unload (detach, dlclose)
	PA_RELOAD,			// unload and load again
} PLUG_ACTION;

// Flags to indicate from where the plugin was loaded.
typedef enum {
	PS_INI = 0,			// was loaded from the plugins.ini
	PS_CMD,				// was loaded via a server command
	PS_PLUGIN,			// was loaded by other plugin
} PLOAD_SOURCE;

// Flags for how to word description of plugin loadtime.
typedef enum {
	SL_SIMPLE = 0,			// single word
	SL_SHOW,			// for "show" output, 5 chars
	SL_ALLOWED,			// when plugin is allowed to load/unload
	SL_NOW,				// current situation
} STR_LOADTIME;

// Flags for how to format description of status.
typedef enum {
	ST_SIMPLE = 0,			// single word
	ST_SHOW,			// for "show" output, 4 chars
} STR_STATUS;

// Flags for how to format description of action.
typedef enum {
	SA_SIMPLE = 0,			// single word
	SA_SHOW,			// for "show" output, 4 chars
} STR_ACTION;

// Flags for how to format description of source.
typedef enum {
	SO_SIMPLE = 0,			// two words
	SO_SHOW,			// for "list" output, 3 chars
} STR_SOURCE;

// api table list
typedef struct {
	enginefuncs_t *engine;
	DLL_FUNCTIONS *dllapi;
	NEW_DLL_FUNCTIONS *newapi;
} api_tables_t;

// An individual plugin.
class MPlugin : public class_metamod_new {
	public:
	// data:
		// reordered for faster api_hook.cpp functions
		PLUG_STATUS status;				// current status of plugin (loaded, etc)
		api_tables_t tables;
		api_tables_t post_tables;
		
		inline DLLINTERNAL void * get_api_table(enum_api_t api) {
			return(((void**)&tables)[api]);
		}
		inline DLLINTERNAL void * get_api_post_table(enum_api_t api) {
			return(((void**)&post_tables)[api]);
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
		
	// functions:		
		mBOOL DLLINTERNAL ini_parseline(const char *line);		// parse line from inifile
		mBOOL DLLINTERNAL cmd_parseline(const char *line);		// parse from console command
		mBOOL DLLINTERNAL plugin_parseline(const char *fname, int loader_index); // parse from plugin
		mBOOL DLLINTERNAL check_input(void);
		
		mBOOL DLLINTERNAL resolve(void);				// find a matching file on disk
		char * DLLINTERNAL resolve_dirs(const char *path);
		char * DLLINTERNAL resolve_prefix(const char *path);
		char * DLLINTERNAL resolve_suffix(const char *path);
		static mBOOL DLLINTERNAL is_platform_postfix(const char *pf);

		mBOOL DLLINTERNAL platform_match(MPlugin* plugin);
		
		mBOOL DLLINTERNAL load(PLUG_LOADTIME now);
		mBOOL DLLINTERNAL unload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason, PL_UNLOAD_REASON real_reason);
		mBOOL DLLINTERNAL reload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
		mBOOL DLLINTERNAL pause(void);
		mBOOL DLLINTERNAL unpause(void);
		mBOOL DLLINTERNAL retry(PLUG_LOADTIME now, PL_UNLOAD_REASON reason); // if previously failed
		void DLLINTERNAL free_api_pointers(void);
		mBOOL DLLINTERNAL clear(void);
		mBOOL DLLINTERNAL plugin_unload(plid_t plid, PLUG_LOADTIME now, PL_UNLOAD_REASON reason); // other plugin unloading
		void DLLINTERNAL show(void);				// print info about plugin to console

		mBOOL DLLINTERNAL newer_file(void);			// check for newer file on disk

	// output string functions
		const char * DLLINTERNAL str_status(STR_STATUS fmt);
		const char * DLLINTERNAL str_action(STR_ACTION fmt);
		const char * DLLINTERNAL str_source(STR_SOURCE fmt);

		const char * DLLINTERNAL str_reason(PL_UNLOAD_REASON preason, PL_UNLOAD_REASON preal_reason);
		const char * DLLINTERNAL str_loadtime(PLUG_LOADTIME pallow, STR_LOADTIME fmt);

		inline const char * DLLINTERNAL str_status(void)		{ return(str_status(ST_SIMPLE)); };
		inline const char * DLLINTERNAL str_action(void)		{ return(str_action(SA_SIMPLE)); };
		inline const char * DLLINTERNAL str_source(void)		{ return(str_source(SO_SIMPLE)); };

		inline const char * DLLINTERNAL str_loadable(void) { 
			return(info?str_loadtime(info->loadable, SL_SIMPLE):" -");
		};
		inline const char * DLLINTERNAL str_unloadable(void) { 
			return(info?str_loadtime(info->unloadable, SL_SIMPLE):" -");
		};
		inline const char * DLLINTERNAL str_loadable(STR_LOADTIME fmt) { 
			return(info?str_loadtime(info->loadable, fmt):" -");
		};
		inline const char * DLLINTERNAL str_unloadable(STR_LOADTIME fmt) { 
			return(info?str_loadtime(info->unloadable, fmt):" -");
		};
	private:
		mBOOL DLLINTERNAL query(void);
		mBOOL DLLINTERNAL attach(PLUG_LOADTIME now);
		mBOOL DLLINTERNAL detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
		
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
		for(int i = 0; &ainfo[i] < &api_info.END; i++) { \
			if(table[i]) { \
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

#endif /* MPLUGIN_H */
