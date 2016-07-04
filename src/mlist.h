#ifndef MLIST_H
#define MLIST_H

#include "types_meta.h"			// mBOOL
#include "mplugin.h"			// class MPlugin
#include "plinfo.h"			// plid_t, etc
#include "new_baseclass.h"

// Max number of plugins we can manage.  This is an arbitrary, fixed number,
// for convenience.  It would probably be better to dynamically grow the
// list as needed, but we do this for now.
#define MAX_PLUGINS 50
// Width required to printf above MAX, for show() functions.
#define WIDTH_MAX_PLUGINS	2

// A list of plugins.
class MPluginList : public class_metamod_new {
	public:
	// data:
		MPlugin plist[MAX_PLUGINS];			// array of plugins
		int size;					// size of list, ie MAX_PLUGINS
		int endlist;					// index of last used entry
		char inifile[PATH_MAX];				// full pathname
	// constructor:
		MPluginList(const char *ifile) DLLINTERNAL;
	// functions:
		void DLLINTERNAL reset_plugin(MPlugin *pl_find);
		MPlugin * DLLINTERNAL find(int pindex);			// find by index
		MPlugin * DLLINTERNAL find(const char *findpath); 	// find by pathname
		MPlugin * DLLINTERNAL find(plid_t id);			// find by plid_t
		MPlugin * DLLINTERNAL find(DLHANDLE handle);		// find by handle
		MPlugin * DLLINTERNAL find_memloc(void *memptr);	// find by memory location
		MPlugin * DLLINTERNAL find_match(const char *prefix);	// find by partial prefix match
		MPlugin * DLLINTERNAL find_match(MPlugin *pmatch);	// find by platform_match()
		MPlugin * DLLINTERNAL add(MPlugin *padd);
		mBOOL DLLINTERNAL found_child_plugins(int source_index);
		void DLLINTERNAL clear_source_plugin_index(int source_index);
		void DLLINTERNAL trim_list(void);
		mBOOL DLLINTERNAL ini_startup(void);			// read inifile at startup
		mBOOL DLLINTERNAL ini_refresh(void);			// re-read inifile
		mBOOL DLLINTERNAL cmd_addload(const char *args);	// load from console command
		MPlugin * DLLINTERNAL plugin_addload(plid_t plid, const char *fname, PLUG_LOADTIME now); //load from plugin
		mBOOL DLLINTERNAL load(void);				// load the list, at startup
		mBOOL DLLINTERNAL refresh(PLUG_LOADTIME now);		// update from re-read inifile
		void DLLINTERNAL unpause_all(void);			// unpause any paused plugins
		void DLLINTERNAL retry_all(PLUG_LOADTIME now);		// retry any pending plugin actions
		void DLLINTERNAL show(int source_index);		// list plugins to console
		void DLLINTERNAL show(void) { show(-1); };		// list plugins to console
		void DLLINTERNAL show_client(edict_t *pEntity);		// list plugins to player client
};

#endif /* MLIST_H */
