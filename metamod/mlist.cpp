// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mlist.cpp - functions for list of plugins (class MPluginList)

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

#include <errno.h>				// errno, etc

#include <extdll.h>				// always

#include "mlist.h"				// me
#include "mplugin.h"			// class MPlugin
#include "plinfo.h"				// plid_t, etc
#include "commands_meta.h"		// cmd_meta_pluginlist, etc
#include "metamod.h"			// GameDLL, etc
#include "types_meta.h"			// mBOOL
#include "log_meta.h"			// META_LOG, etc
#include "osdep.h"				// win32 snprintf, normalize_pathname,
#include "osdep_p.h"

// Constructor
MPluginList::MPluginList(const char *ifile) 
	: size(MAX_PLUGINS), endlist(0)
{
	int i;
	// store filename of ini file
	STRNCPY(inifile, ifile, sizeof(inifile));
	// initialize array
	for(i=0; i < size; i++) {
		//reset to empty
		plist[i].index=i+1;
		reset_plugin(&plist[i]);
	}
	endlist=0;
}

// Resets plugin to empty
void DLLINTERNAL MPluginList::reset_plugin(MPlugin *pl_find) {
	int i;
	
	//calculate index
	i = pl_find - &plist[0];
	
	//free any pointers first
	pl_find->free_api_pointers();
	
	//set zero
	memset(pl_find, 0, sizeof(*pl_find));
	
	pl_find->index=i+1;		// 1-based
}

// Find a plugin based on the plugin index #.
// meta_errno values:
//  - ME_ARGUMENT	invalid pindex
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin * DLLINTERNAL MPluginList::find(int pindex) {
	MPlugin *pfound;
	if(pindex <= 0)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	pfound=&plist[pindex-1];
	if(pfound->status < PL_VALID)
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	else
		return(pfound);
}

// Find a plugin based on the plugin handle.
// meta_errno values:
//  - ME_ARGUMENT	invalid pindex
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin * DLLINTERNAL MPluginList::find(DLHANDLE handle) {
	int i;

	if(!handle)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	for(i=0; i < endlist; i++) {
		if(plist[i].status < PL_VALID)
			continue;
		if(plist[i].handle == handle)
			return(&plist[i]);
	}
	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Clear source_plugin_index on all matching plugins
void DLLINTERNAL MPluginList::clear_source_plugin_index(int source_index) {
	int i;
	
	if(source_index <= 0)
		return;
	
	for(i=0; i < endlist; i++) {
		if(plist[i].status < PL_VALID)
			continue;
		if(plist[i].source_plugin_index == source_index)
			plist[i].source_plugin_index = -1;
	}
}

// Find if any plugin has been loaded by plugin 'source_index'
mBOOL DLLINTERNAL MPluginList::found_child_plugins(int source_index) {
	int i;
	
	if(source_index <= 0)
		return(mFALSE);
	
	for(i=0; i < endlist; i++) {
		if(plist[i].status < PL_VALID)
			continue;
		if(plist[i].source_plugin_index == source_index)
			return(mTRUE);
	}
	
	return(mFALSE);
}

// Try make endlist lower (called after plugin unload)
void DLLINTERNAL MPluginList::trim_list(void) {
	int i,n;
	
	if(endlist <= 0)
		return;
	
	for(i=0,n=0; i < endlist; i++) {
		if(plist[i].status == PL_EMPTY)
			continue;
		n=i+1;
	}
	
	if(n < endlist)
		endlist=n;
}

// Find a plugin with the given plid.
// meta_errno values:
//  - ME_ARGUMENT	null plid_t
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin * DLLINTERNAL MPluginList::find(plid_t id) {
	int i;

	if(!id)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	for(i=0; i < endlist; i++) {
		if(plist[i].status < PL_VALID)
			continue;
		if(plist[i].info == id)
			return(&plist[i]);
	}
	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin with the given pathname.
// meta_errno values:
//  - ME_ARGUMENT	null path
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin * DLLINTERNAL MPluginList::find(const char *findpath) {
	int i;

	if(!findpath)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	META_DEBUG(8, ("Looking for loaded plugin with dlfnamepath: %s", findpath));
	for(i=0; i < endlist; i++) {
		META_DEBUG(9, ("Looking at: plugin %s loadedpath: %s", plist[i].file, plist[i].pathname));
		if(plist[i].status < PL_VALID)
			continue;
		if(strmatch(plist[i].pathname, findpath)) {
			META_DEBUG(8, ("Found loaded plugin %s", plist[i].file));
			return(&plist[i]);
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
MPlugin * DLLINTERNAL MPluginList::find_memloc(void *memptr) {
#ifdef linux
	const char *dlfile;

	if(!memptr)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	if(!(dlfile=DLFNAME(memptr))) {
		META_DEBUG(8, ("DLFNAME failed to find memloc %d", memptr));
		// meta_errno should be already set in DLFNAME
		return(NULL);
	}
	
	return(find(dlfile));
#else
	DLHANDLE dlhandle;
	
	if(!memptr)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	if(!(dlhandle=get_module_handle_of_memptr(memptr))) {
		META_DEBUG(8, ("DLFNAME failed to find memloc %d", memptr));
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}
	
	return(find(dlhandle));
#endif
}

// Find a plugin with non-ambiguous prefix string matching desc, file, 
// name, or logtag.
// meta_errno values:
//  - ME_ARGUMENT	null prefix
//  - ME_NOTFOUND	couldn't find a matching plugin
//  - ME_NOTUNIQ	found multiple matches; no unique match
MPlugin * DLLINTERNAL MPluginList::find_match(const char *prefix) {
	int i, len;
	MPlugin *iplug, *pfound;
	char buf[NAME_MAX];

	if(!prefix)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	pfound=NULL;
	len=strlen(prefix);
	safevoid_snprintf(buf, sizeof(buf), "mm_%s", prefix);
	for(i=0; i < endlist; i++) {
		iplug=&plist[i];
		if(iplug->status < PL_VALID)
			continue;
		if(iplug->info && strncasecmp(iplug->info->name, prefix, len) == 0) {
			if(pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);
			pfound=iplug;
			continue;
		}
		else if(strncasecmp(iplug->desc, prefix, len) == 0) {
			if(pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);
			pfound=iplug;
			continue;
		}
		else if(strncasecmp(iplug->file, prefix, len) == 0) {
			if(pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);
			pfound=iplug;
			continue;
		}
		else if(strncasecmp(iplug->file, buf, strlen(buf)) == 0) {
			if(pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);
			pfound=iplug;
			continue;
		}
		else if(iplug->info
				&& strncasecmp(iplug->info->logtag, prefix, len) == 0)
		{
			if(pfound)
				RETURN_ERRNO(NULL, ME_NOTUNIQ);
			pfound=iplug;
			continue;
		}
	}
	
	if(pfound)
		return(pfound);

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Find a plugin with same file, logtag, desc or significant 
// prefix of file. Uses the platform_match() method of MPlugin.
// meta_errno values:
//  - ME_ARGUMENT	null prefix
//  - ME_NOTFOUND	couldn't find a matching plugin
MPlugin * DLLINTERNAL MPluginList::find_match(MPlugin *pmatch) {
	int i;
	MPlugin *iplug, *pfound;
	if(!pmatch)
		RETURN_ERRNO(NULL, ME_ARGUMENT);
	pfound=NULL;
	for(i=0; i < endlist; i++) {
		iplug=&plist[i];
		if(pmatch->platform_match(iplug)) {
			pfound=iplug;
			break;
		}
	}

	if(pfound)
		return(pfound);
	
	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Add a plugin to the list.
// meta_errno values:
//  - ME_MAXREACHED		reached max plugins
MPlugin * DLLINTERNAL MPluginList::add(MPlugin *padd) {
	int i;
	MPlugin *iplug;

	// Find either:
	//  - a slot in the list that's not being used
	//  - the end of the list
	for(i=0; i < endlist && plist[i].status != PL_EMPTY; i++);

	// couldn't find a slot to use
	if(i==size) {
		META_WARNING("Couldn't add plugin '%s' to list; reached max plugins (%d)", 
				padd->file, i);
		RETURN_ERRNO(NULL, ME_MAXREACHED);
	}

	// if we found the end of the list, advance end marker
	if(i==endlist)
		endlist++;
	iplug = &plist[i];

	// copy filename into this free slot
	STRNCPY(iplug->filename, padd->filename, sizeof(iplug->filename));
	// Copy file offset ptr.
	// Can't just copy ptr, as it points to offset in padd, which will go
	// away; need to point to corresponding offset in iplug.
	iplug->file = iplug->filename + (padd->file - padd->filename);
	// copy description
	STRNCPY(iplug->desc, padd->desc, sizeof(iplug->desc));
	// copy pathname
	STRNCPY(iplug->pathname, padd->pathname, sizeof(iplug->pathname));
	// copy source
	iplug->source=padd->source;
	// copy loader-plugin
	iplug->source_plugin_index=padd->source_plugin_index;
	// copy status
	iplug->status=padd->status;

	return(iplug);
}


// Read plugins.ini at server startup.
// meta_errno values:
//  - ME_NOFILE		ini file missing or empty
mBOOL DLLINTERNAL MPluginList::ini_startup() {
	FILE *fp;
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin *pmatch;

	if(!valid_gamedir_file(inifile)) {
		META_WARNING("ini: Metamod plugins file empty or missing: %s", inifile);
		RETURN_ERRNO(mFALSE, ME_NOFILE);
	}
	full_gamedir_path(inifile, inifile);

	fp=fopen(inifile, "r");
	if(!fp) {
		META_WARNING("ini: Unable to open plugins file '%s': %s", inifile, 
				strerror(errno));
		RETURN_ERRNO(mFALSE, ME_NOFILE);
	}

	META_LOG("ini: Begin reading plugins list: %s", inifile);
	for(n=0, ln=1; !feof(fp) && fgets(line, sizeof(line), fp) && n < size; ln++) {
		// Remove line terminations.
		char *cp;
		if((cp=strrchr(line, '\r')))
			*cp='\0';
		if((cp=strrchr(line, '\n')))
			*cp='\0';
		// Parse directly into next entry in array
		if(!plist[n].ini_parseline(line)) {
			if(meta_errno==ME_FORMAT)
				META_WARNING("ini: Skipping malformed line %d of %s", ln, 
						inifile);
			continue;
		}
		// Check for a duplicate - an existing entry with this pathname.
		if(find(plist[n].pathname)) {
			// Should we check platform specific level here?
			META_INFO("ini: Skipping duplicate plugin, line %d of %s: %s", 
					ln, inifile, plist[n].pathname);
			continue;
		}
		// Check for a matching platform with different platform specifics
		// level.
		if(NULL != (pmatch=find_match(&plist[n]))) {
			if(pmatch->pfspecific >= plist[n].pfspecific) {
				META_DEBUG(1, ("ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)",
                         ln, inifile, pmatch->pfspecific, plist[n].pfspecific)); 
				continue;
			}
			META_DEBUG(1, ("ini: Plugin in line %d overrides existing plugin with lower platform specific level %d, ours %d",
					ln, pmatch->pfspecific, plist[n].pfspecific));
			//reset to empty
			reset_plugin(pmatch);
		}
		plist[n].action=PA_LOAD;
		META_LOG("ini: Read plugin config for: %s", plist[n].desc);
		n++;
		endlist=n;		// mark end of list
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins to load",
			inifile, n);

	fclose(fp);
	if(!n) {
		META_WARNING("ini: Warning; no plugins found to load?");
	}
	return(mTRUE);
}

// Re-read plugins.ini looking for added/deleted/changed plugins.
// meta_errno values:
//  - ME_NOFILE		ini file missing or empty
mBOOL DLLINTERNAL MPluginList::ini_refresh() {
	FILE *fp;
	char line[MAX_STRBUF_LEN];
	int n, ln;
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added;

	fp=fopen(inifile, "r");
	if(!fp) {
		META_WARNING("ini: Unable to open plugins file '%s': %s", inifile, 
				strerror(errno));
		RETURN_ERRNO(mFALSE, ME_NOFILE);
	}

	META_LOG("ini: Begin re-reading plugins list: %s", inifile);
	for(n=0, ln=1; !feof(fp) && fgets(line, sizeof(line), fp) && n < size; ln++) 
	{
		// Remove line terminations.
		char *cp;
		if((cp=strrchr(line, '\r')))
			*cp='\0';
		if((cp=strrchr(line, '\n')))
			*cp='\0';
		// Parse into a temp plugin
		memset(&pl_temp, 0, sizeof(pl_temp));
		if(!pl_temp.ini_parseline(line)) {
			if(meta_errno==ME_FORMAT)
				META_WARNING("ini: Skipping malformed line %d of %s", 
						ln, inifile);
			continue;
		}
		// Try to find plugin with this pathname in the current list of
		// plugins.
		if(!(pl_found=find(pl_temp.pathname))) {
			// Check for a matching platform with higher platform specifics
			// level.
			if(NULL != (pl_found=find_match(&pl_temp))) {
				if(pl_found->pfspecific >= pl_temp.pfspecific) {
					META_DEBUG(1, ("ini: Skipping plugin, line %d of %s: plugin with higher platform specific level already exists. (%d >= %d)",
                       			 ln, inifile, pl_found->pfspecific, pl_temp.pfspecific)); 
					continue;
				}
				if(PA_LOAD == pl_found->action) {
					META_DEBUG(1, ("ini: Plugin in line %d overrides loading of plugin with lower platform specific level %d, ours %d",
								ln, pl_found->pfspecific, pl_temp.pfspecific));
					//reset to empty
					reset_plugin(pl_found);
				}
				else {
					META_DEBUG(1, ("ini: Plugin in line %d should override existing plugin with lower platform specific level %d, ours %d. Unable to comply.",
								ln, pl_found->pfspecific, pl_temp.pfspecific));
					continue;
				}
			}
			// new plugin; add to list
			if((pl_added=add(&pl_temp))) {
				// try to load this plugin at the next opportunity
				pl_added->action=PA_LOAD;
			}
			else
				// error details logged in add()
				continue;
		}
		else {
			// This plugin is already in the current list of plugins.
			// Pathname already matches.  Recopy desc, if specified in
			// plugins.ini.
			if(pl_temp.desc[0] != '<')
				STRNCPY(pl_found->desc, pl_temp.desc, sizeof(pl_found->desc));

			// Check the file to see if it looks like it's been modified
			// since we last loaded it.
			if(!pl_found->newer_file()) {
				if(meta_errno==ME_NOFILE) {
					META_WARNING("ini: Skipping plugin, couldn't stat file '%s': %s",
							pl_found->pathname, strerror(errno));
					continue;
				}
				else {
					// File hasn't been updated.
					// Keep plugin (don't let refresh() unload it).
					pl_found->action=PA_KEEP;
				}
			}
			// Newer file on disk.
			else if(pl_found->status >= PL_OPENED) {
				META_DEBUG(2, ("ini: Plugin '%s' has newer file on disk", pl_found->desc));
				pl_found->action=PA_RELOAD;
			}
			else
				META_WARNING("ini: Plugin '%s' has newer file, but unexpected status (%s)",
						pl_found->desc, pl_found->str_status());
		}
		if(NULL != pl_found) {
			META_LOG("ini: Read plugin config for: %s", pl_found->desc);
		}
		else {
			META_LOG("ini: Read plugin config for: %s", pl_temp.desc);
		}
		n++;
	}
	META_LOG("ini: Finished reading plugins list: %s; Found %d plugins", inifile, n);

	fclose(fp);
	if(!n) {
		META_WARNING("ini: Warning; no plugins found to load?");
	}
	return(mTRUE);
}

// Load a plugin from plugin request.
// meta_errno values:
//  - errno's from resolve()
//  - ME_ALREADY	this plugin already loaded
//  - errno's from add()
//  - errno's from load()
MPlugin * DLLINTERNAL MPluginList::plugin_addload(plid_t plid, const char *fname, PLUG_LOADTIME now) {
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added, *pl_loader;
	
	// Find loader plugin
	if(!(pl_loader=find(plid))) {
		// Couldn't find a matching file on disk
		META_DEBUG(1, ("Couldn't find plugin that gave this loading request!"));
		// meta_errno should be already set in resolve()
		RETURN_ERRNO(NULL, ME_BADREQ);
	}
	
	memset(&pl_temp, 0, sizeof(pl_temp));
	
	// copy filename
	if(!pl_temp.plugin_parseline(fname, pl_loader->index)) {
		// parse_plugin_load doesn't return mFALSE.
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}
	
	// resolve given path into a file; accepts various "shortcut"
	// pathnames.
	if(pl_temp.resolve() != mTRUE) {
		// Couldn't find a matching file on disk
		META_DEBUG(1, ("Couldn't resolve given path into a file: %s", pl_temp.file));
		// meta_errno should be already set in resolve()
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}

	// Try to find plugin with this pathname in the current list of
	// plugins.
	if((pl_found=find(pl_temp.pathname))) {
		// Already in list
		META_DEBUG(1, ("Plugin '%s' already in current list; file=%s desc='%s'", 
				pl_temp.file, pl_found->file, pl_found->desc));
		RETURN_ERRNO(NULL, ME_ALREADY);
	}
	// new plugin; add to list
	if(!(pl_added=add(&pl_temp))) {
		META_DEBUG(1, ("Couldn't add plugin '%s' to list; see log", pl_temp.desc));
		// meta_errno should be already set in add()
		return(NULL);
	}

	// try to load new plugin (setting 'must load now')
	pl_added->action=PA_LOAD;
	if(!pl_added->load(now)) {
		// load failed
		if(meta_errno==ME_NOTALLOWED || meta_errno==ME_DELAYED) {
			META_DEBUG(1, ("Plugin '%s' couldn't attach; only allowed %s", 
					pl_added->desc, pl_added->str_loadable(SL_ALLOWED)));
			pl_added->clear();
		}
		else if(pl_added->status == PL_OPENED)
			META_DEBUG(1, ("Opened plugin '%s', but failed to attach; see log", pl_added->desc));
		else
			META_DEBUG(1, ("Couldn't load plugin '%s'; see log", pl_added->desc));
		// meta_errno should be already set in load()
		return(NULL);
	}
	META_DEBUG(1, ("Loaded plugin '%s' successfully", pl_added->desc));
	meta_errno = ME_NOERROR;
	return(pl_added);
}

// Load a plugin from a console command.
// meta_errno values:
//  - errno's from cmd_parseline()
//  - errno's from resolve()
//  - ME_ALREADY	this plugin already loaded
//  - errno's from add()
//  - errno's from load()
mBOOL DLLINTERNAL MPluginList::cmd_addload(const char *args) {
	MPlugin pl_temp;
	MPlugin *pl_found, *pl_added;
	
	memset(&pl_temp, 0, sizeof(pl_temp));
	
	// XXX move back to comands_meta ?

	// parse into a temp plugin
	if(pl_temp.cmd_parseline(args) != mTRUE) {
		META_CONS("Couldn't parse 'meta load' arguments: %s", args);
		// meta_errno should be already set in cmd_parseline()
		return(mFALSE);
	}

	// resolve given path into a file; accepts various "shortcut"
	// pathnames.
	if(pl_temp.resolve() != mTRUE) {
		// Couldn't find a matching file on disk
		META_CONS("Couldn't resolve given path into a file: %s", 
				pl_temp.file);
		// meta_errno should be already set in resolve()
		return(mFALSE);
	}

	// Try to find plugin with this pathname in the current list of
	// plugins.
	if((pl_found=find(pl_temp.pathname))) {
		// Already in list
		META_CONS("Plugin '%s' already in current list; file=%s desc='%s'", 
				pl_temp.file, pl_found->file, pl_found->desc);
		RETURN_ERRNO(mFALSE, ME_ALREADY);
	}
	// new plugin; add to list
	if(!(pl_added=add(&pl_temp))) {
		META_CONS("Couldn't add plugin '%s' to list; see log", pl_temp.desc);
		// meta_errno should be already set in add()
		return(mFALSE);
	}

	// try to load new plugin
	pl_added->action=PA_LOAD;
	if(!pl_added->load(PT_ANYTIME)) {
		// load failed
		if(meta_errno==ME_DELAYED) 
			META_CONS("Loaded plugin '%s', but will wait to become active, %s", 
					pl_added->desc, pl_added->str_loadable(SL_ALLOWED));
		else if(meta_errno==ME_NOTALLOWED) {
			META_CONS("Plugin '%s' couldn't attach; only allowed %s", 
					pl_added->desc, pl_added->str_loadable(SL_ALLOWED));
			pl_added->clear();
		}
		else if(pl_added->status == PL_OPENED)
			META_CONS("Opened plugin '%s', but failed to attach; see log", pl_added->desc);
		else
			META_CONS("Couldn't load plugin '%s'; see log", pl_added->desc);
		show();
		// meta_errno should be already set in load()
		return(mFALSE);
	}
	META_CONS("Loaded plugin '%s' successfully", pl_added->desc);
	show();
	return(mTRUE);
}

// Load plugins at startup.
// meta_errno values:
//  - errno's from ini_startup()
mBOOL DLLINTERNAL MPluginList::load() {
	int i, n;

	if(!ini_startup()) {
		META_WARNING("Problem loading plugins.ini: %s", inifile);
		// meta_errno should be already set in ini_startup()
		return(mFALSE);
	}

	META_LOG("dll: Loading plugins...");
	for(i=0, n=0; i < endlist; i++) {
		if(plist[i].status < PL_VALID)
			continue;
		if(plist[i].load(PT_STARTUP) == mTRUE)
			n++;
		else
			// all plugins should be loadable at startup...
			META_WARNING("dll: Failed to load plugin '%s'", plist[i].file);
	}
	META_LOG("dll: Finished loading %d plugins", n);
	return(mTRUE);
}

// Update list of loaded plugins from ini file, and load any new/changed plugins.
// meta_errno values:
//  - errno's from ini_refresh()
mBOOL DLLINTERNAL MPluginList::refresh(PLUG_LOADTIME now) {
	int i, ndone=0, nkept=0, nloaded=0, nunloaded=0, nreloaded=0, ndelayed=0;
	MPlugin *iplug;

	if(!ini_refresh()) {
		META_WARNING("dll: Problem reloading plugins.ini: %s", inifile);
		// meta_errno should be already set in ini_refresh()
		return(mFALSE);
	}

	META_LOG("dll: Updating plugins...");
	for(i=0; i < endlist; i++) {
		iplug=&plist[i];
		if(iplug->status < PL_VALID)
			continue;
		switch(iplug->action) {
			case PA_KEEP:
				META_DEBUG(1, ("Keeping plugin '%s'", iplug->desc));
				iplug->action=PA_NONE;
				nkept++;
				break;
			case PA_LOAD:
				META_DEBUG(1, ("Loading plugin '%s'", iplug->desc));
				if(iplug->load(now))
					nloaded++;
				else if(meta_errno==ME_DELAYED)
					ndelayed++;
				break;
			case PA_RELOAD:
				META_DEBUG(1, ("Reloading plugin '%s'", iplug->desc));
				if(iplug->reload(now, PNL_FILE_NEWER))
					nreloaded++;
				else if(meta_errno==ME_DELAYED)
					ndelayed++;
				break;
			case PA_NONE:
				// If previously loaded from ini, but apparently removed from new ini.
				if(iplug->source==PS_INI && iplug->status >= PL_RUNNING) {
					META_DEBUG(1, ("Unloading plugin '%s'", iplug->desc));
					iplug->action=PA_UNLOAD;
					if(iplug->unload(now, PNL_INI_DELETED, PNL_INI_DELETED))
						nunloaded++;
					else if(meta_errno==ME_DELAYED)
						ndelayed++;
				}
				break;
			case PA_ATTACH:
				// Previously requested attach, but was delayed?
				META_DEBUG(1, ("Retrying attach plugin '%s'", iplug->desc));
				if(iplug->retry(now, PNL_DELAYED))
					nloaded++;
				else if(meta_errno==ME_DELAYED)
					ndelayed++;
				break;
			case PA_UNLOAD:
				// Previously requested unload, but was delayed?
				META_DEBUG(1, ("Retrying unload plugin '%s'", iplug->desc));
				if(iplug->retry(now, PNL_DELAYED))
					nunloaded++;
				else if(meta_errno==ME_DELAYED)
					ndelayed++;
				break;
			case PA_NULL:
				META_WARNING("dll: Unexpected action for plugin '%s': '%s'", iplug->desc, iplug->str_action());
				break;
			default:
				META_WARNING("dll: Unrecognized action for plugin '%s': '%s'", iplug->desc, iplug->str_action());
				break;
		}
		ndone++;
	}
	META_LOG("dll: Finished updating %d plugins; kept %d, loaded %d, unloaded %d, reloaded %d, delayed %d", 
			ndone, nkept, nloaded, nunloaded, nreloaded, ndelayed);
	return(mTRUE);
}

// Re-enable any plugins currently paused.
// meta_errno values:
//  - none
void DLLINTERNAL MPluginList::unpause_all(void) {
	int i;
	MPlugin *iplug;
	for(i=0; i < endlist; i++) {
		iplug=&plist[i];
		if(iplug->status==PL_PAUSED)
			iplug->unpause();
	}
}

// Retry any pending actions on plugins, for instance load/unload delayed
// until changelevel.
// meta_errno values:
//  - none
void DLLINTERNAL MPluginList::retry_all(PLUG_LOADTIME now) {
	int i;
	MPlugin *iplug;
	for(i=0; i < endlist; i++) {
		iplug=&plist[i];
		if(iplug->action != PA_NONE)
			iplug->retry(now, PNL_DELAYED);
	}
}

// List plugins and information about them in a formatted table.
// meta_errno values:
//  - none
void DLLINTERNAL MPluginList::show(int source_index) {
	int i, n=0, r=0;
	MPlugin *pl;
	char desc[15+1], file[16+1], vers[7+1];		// plus 1 for term null
	
	if(source_index <= 0)
		META_CONS("Currently loaded plugins:");
	else
		META_CONS("Child plugins:");
	
	META_CONS("  %*s  %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s",
			WIDTH_MAX_PLUGINS, "",
			sizeof(desc)-1, "description",
			"stat", "pend",
			sizeof(file)-1, "file", sizeof(vers)-1, "ers",
			2+WIDTH_MAX_PLUGINS, "src", 
			"load ", "unlod");
	
	for(i=0; i < endlist; i++) {
		pl=&plist[i];
		if(pl->status < PL_VALID)
			continue;
		if(source_index > 0 && pl->source_plugin_index != source_index)
			continue;
		STRNCPY(desc, pl->desc, sizeof(desc));
		STRNCPY(file, pl->file, sizeof(file));
		if(pl->info && pl->info->version)
			STRNCPY(vers, pl->info->version, sizeof(vers));
		else 
			STRNCPY(vers, " -", sizeof(vers));
		META_CONS(" [%*d] %-*s  %-4s %-4s  %-*s  v%-*s  %-*s  %-5s %-5s",
				WIDTH_MAX_PLUGINS, pl->index, 
				sizeof(desc)-1, desc,
				pl->str_status(ST_SHOW), pl->str_action(SA_SHOW),
				sizeof(file)-1, file, sizeof(vers)-1, vers,
				2+WIDTH_MAX_PLUGINS, pl->str_source(SO_SHOW),
				pl->str_loadable(SL_SHOW), pl->str_unloadable(SL_SHOW));
		if(pl->status == PL_RUNNING)
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
void DLLINTERNAL MPluginList::show_client(edict_t *pEntity) {
	int i, n=0;
	MPlugin *pl;
	META_CLIENT(pEntity, "Currently running plugins:");
	for(i=0; i < endlist; i++) {
		pl=&plist[i];
		if(pl->status != PL_RUNNING || !pl->info)
			continue;
		n++;
		META_CLIENT(pEntity, " [%3d] %s, v%s, %s, by %s, see %s",
				n, 
				pl->info->name ? pl->info->name : "<unknown>",
				pl->info->version ? pl->info->version : "<?>",
				pl->info->date ? pl->info->date : "<../../..>",
				pl->info->author ? pl->info->author : "<unknown>",
				pl->info->url ? pl->info->url : "<unknown>");
	}
	META_CLIENT(pEntity, "%d plugins", n);
}
