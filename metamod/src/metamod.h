// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// metamod.h - (main) description of metamod operations

/*
 * Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
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
 *    link the code of this program with the Half-Life Game g_engine ("HL
 *    g_engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL g_engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef METAMOD_H
#define METAMOD_H

#include "meta_api.h"			// META_RES, etc
#include "mlist.h"				// MPluginList, etc
#include "mreg.h"				// MRegCmdList, etc
#include "conf_meta.h"			// MConfig
#include "osdep.h"				// NAME_MAX, etc
#include "types_meta.h"			// mBOOL
#include "mplayer.h"			// MPlayerList
#include "meta_eiface.h"        // HL_enginefuncs_t, meta_enginefuncs_t
#include "engine_t.h"           // engine_t, g_engine

// file that lists plugins to load at startup
#define PLUGINS_INI			"addons/metamod/plugins.ini"
#define OLD_PLUGINS_INI		"metamod.ini"

// file that contains commands to metamod plugins at startup
#define EXEC_CFG			"addons/metamod/exec.cfg"
#define OLD_EXEC_CFG		"metaexec.cfg"

// previously, file that contained path for an override-gamedll
#define OLD_GAMEDLL_TXT		"metagame.ini"

// generic config file
#define CONFIG_INI			"addons/metamod/config.ini"


// cvar to contain version
extern cvar_t meta_version;

// Info about the game dll/mod.
typedef struct gamedll_s {
	char name[NAME_MAX];		// ie "cstrike" (from gamedir)
	const char *desc;			// ie "Counter-Strike"
	char gamedir[PATH_MAX];		// ie "/home/willday/half-life/cstrike"
	char pathname[PATH_MAX];	// ie "/home/willday/half-life/cstrike/dlls/cs_i386.so"
	char const *file;			// ie "cs_i386.so"
	char real_pathname[PATH_MAX];	// in case pathname overridden by bot, etc
	DLHANDLE handle;
	gamedll_funcs_t funcs;		// dllapi_table, newapi_table
} gamedll_t;
extern gamedll_t GameDLL;

// SDK variables for storing engine funcs and globals.
extern HL_enginefuncs_t g_engfuncs;
extern globalvars_t  *gpGlobals;

// Our modified version of the engine funcs, to give to plugins.
extern meta_enginefuncs_t g_plugin_engfuncs;

// g_config structure.
extern MConfig *g_config;

// List of plugins loaded/opened/running.
extern MPluginList *g_plugins;

// List of command functions registered by plugins.
extern MRegCmdList *g_regCmds;

// List of cvar structures registered by plugins.
extern MRegCvarList *g_regCvars;

// List of user messages registered by gamedll.
extern MRegMsgList *g_regMsgs;

#ifdef UNFINISHED
// List of event/logline hooks requested by plugins.
extern MHookList *Hooks;
#endif /* UNFINISHED */

// Data provided to plugins.
// Separate copies to prevent plugins from modifying "readable" parts.
// See meta_api.h for meta_globals_t structure.
extern meta_globals_t PublicMetaGlobals;
extern meta_globals_t PrivateMetaGlobals;

// hook function tables
extern DLL_FUNCTIONS *pHookedDllFunctions;
extern NEW_DLL_FUNCTIONS *pHookedNewDllFunctions;

// (patch by hullu)
// Safety check for metamod-bot-plugin bugfix.
//  engine_api->pfnRunPlayerMove calls dllapi-functions before it returns.
//  This causes problems with bots running as metamod plugins, because
//  metamod assumed that PublicMetaGlobals is free to be used.
//  With call_count we can fix this by backuping up PublicMetaGlobals if 
//  it's already being used.
extern unsigned int CALL_API_count;

// stores previous requestid counter
extern int requestid_counter;

// (patch by BAILOPAN)
// Holds cached player info, right now only things for querying cvars
// Max players is always 32, small enough that we can use a static array
extern MPlayerList g_Players;

void metamod_startup(void);

mBOOL meta_init_gamedll(void);
mBOOL meta_load_gamedll(void);

// ===== lotsa macros... ======================================================

// These are the meat of the metamod processing, and are as ugly as (or
// uglier) than they look.  This is done via macros, because of the varying
// parameter types (int, void, edict_t*, etc) as well as varying
// function-pointer types and different api tables (dllapi, newapi,
// engine), which just can't be passed to a function.  And, since the
// operation is similar for each api call, I didn't want to keep
// duplicating code all over the place.  Thus the ugly macros.
//
// The basic operation is, for each api call:
//  - iterate through list of plugins
//  - for each plugin, if it provides this api call, then call the
//    function in the plugin
//  - call the "real" function (in the game dll, or from the engine)
//  - for each plugin, check for a "post" version of the function, and call
//    if present
//
//
// Also, for any api call, each plugin has the opportunity to replace the
// real routine, in two ways:
//  - prevent the real routine from being called ("supercede")
//  - allow the real routine to be called, but change the value that's
//    returned ("override")
//
// Thus after each plugin is called, its META_RETURN flag is checked, and
// action taken appropriately.  Note that supercede/override only affects
// the _real_ routine; other plugins will still be called.
//
// In addition to the SUPERCEDE and OVERRIDE flags, there are two
// additional flags a plugin can return:
//  - HANDLED ("I did something here")
//  - IGNORED ("I didn't really do anything")
//
// These aren't used by metamod itself, but could be used by plugins to
// get an idea if a previous plugin did anything.
//
//
// The 5 basic macros are:
//   SETUP
//   CALL_PLUGIN
//   CALL_GAME  and  CALL_ENGINE
//   RETURN
//
// These 5 are actually used to build second level macros for each api type
// (dllapi, newapi, engine), with the CALL_PLUGIN macro being used twice
// (before and after).  Altogether, they end up expanding to approx 150
// lines of code for _each_ api call.  Ack, ugly indeed.
//
// However, due to some functions returning 'void', and others returning an
// actual value, I had to have separate macros for the two, since I
// couldn't seem to generalize the two into a form that the compiler would
// accept.  Thus there are "_void" versions of the 5 macros; these are
// listed first.

// ===== macros for void-returning functions ==================================

// declare/init some variables
#define SETUP_API_CALLS_void(FN_TYPE, pfnName, api_info_table) \
	int i; \
	META_RES mres=MRES_UNSET, status=MRES_UNSET, prev_mres=MRES_UNSET; \
	MPlugin *iplug; \
	FN_TYPE pfn_routine=NULL; \
	int loglevel=api_info_table.pfnName.loglevel; \
	const char *pfn_string=api_info_table.pfnName.name; \
	meta_globals_t backup_meta_globals; \
	/* fix bug with metamod-bot-plugins (hullu)*/ \
	if (CALL_API_count++>0) \
		/* backup publicmetaglobals */ \
		backup_meta_globals = PublicMetaGlobals;

// call each plugin
#define CALL_PLUGIN_API_void(post, pfnName, pfn_args, api_table) \
	prev_mres=MRES_UNSET; \
	for(i=0; i < g_plugins->endlist; i++) { \
		iplug=&g_plugins->plist[i]; \
		if (iplug->status != PL_RUNNING) \
			continue; \
		if(iplug->api_table && (pfn_routine=iplug->api_table->pfnName)); \
		else \
			/* plugin doesn't provide this function */  \
			continue; \
		/* initialize PublicMetaGlobals */ \
		PublicMetaGlobals.mres = MRES_UNSET; \
		PublicMetaGlobals.prev_mres = prev_mres; \
		PublicMetaGlobals.status = status; \
		/* call plugin */ \
		META_DEBUG(loglevel, ("Calling %s:%s%s()", iplug->file, pfn_string, (post?"_Post":""))); \
		pfn_routine pfn_args; \
		/* plugin's result code */ \
		mres=PublicMetaGlobals.mres; \
		if(mres > status) \
			status = mres; \
		/* save this for successive plugins to see */ \
		prev_mres = mres; \
		if(mres==MRES_UNSET) \
			META_ERROR("Plugin didn't set meta_result: %s:%s%s()", iplug->file, pfn_string, (post?"_Post":"")); \
		if(post && mres==MRES_SUPERCEDE) \
			META_ERROR("MRES_SUPERCEDE not valid in Post functions: %s:%s%s()", iplug->file, pfn_string, (post?"_Post":"")); \
	}

// call "real" function, from gamedll
#define CALL_GAME_API_void(pfnName, pfn_args, api_table) \
	CALL_API_count--; \
	if(status==MRES_SUPERCEDE) { \
		META_DEBUG(loglevel, ("Skipped (supercede) %s:%s()", GameDLL.file, pfn_string)); \
		/* don't return here; superceded game routine, but still allow \
		 * _post routines to run. \
		 */ \
	} \
	else if(GameDLL.funcs.api_table) { \
		pfn_routine=GameDLL.funcs.api_table->pfnName; \
		if(pfn_routine) { \
			META_DEBUG(loglevel, ("Calling %s:%s()", GameDLL.file, pfn_string)); \
			pfn_routine pfn_args; \
		} \
		/* don't complain for NULL routines in NEW_DLL_FUNCTIONS  */ \
		else if((void*) GameDLL.funcs.api_table != (void*) GameDLL.funcs.newapi_table) { \
			META_ERROR("Couldn't find api call: %s:%s", GameDLL.file, pfn_string); \
			status=MRES_UNSET; \
		} \
	} \
	else { \
		META_DEBUG(loglevel, ("No api table defined for api call: %s:%s", GameDLL.file, pfn_string)); \
	} \
	CALL_API_count++;

// call "real" function, from engine
#define CALL_ENGINE_API_void(pfnName, pfn_args) \
	CALL_API_count--; \
	if(status==MRES_SUPERCEDE) { \
		META_DEBUG(loglevel, ("Skipped (supercede) engine:%s()", pfn_string)); \
		/* don't return here; superceded game routine, but still allow \
		 * _post routines to run. \
		 */ \
	} \
	else { \
		pfn_routine=g_engine.funcs->pfnName; \
		if(pfn_routine) { \
			META_DEBUG(loglevel, ("Calling engine:%s()", pfn_string)); \
			pfn_routine pfn_args; \
		} \
		else { \
			META_ERROR("Couldn't find api call: engine:%s", pfn_string); \
			status=MRES_UNSET; \
		} \
	} \
	CALL_API_count++;

// return (void)
#define RETURN_API_void() \
	if (--CALL_API_count>0) \
		/*restore backup*/ \
		PublicMetaGlobals = backup_meta_globals; \
	return;


// ===== macros for type-returning functions ==================================

// declare/init some variables
#define SETUP_API_CALLS(ret_t, ret_init, FN_TYPE, pfnName, api_info_table) \
	int i; \
	ret_t dllret=ret_init; \
	ret_t override_ret=ret_init; \
	ret_t pub_override_ret=ret_init; \
	ret_t orig_ret=ret_init; \
	ret_t pub_orig_ret=ret_init; \
	META_RES mres=MRES_UNSET, status=MRES_UNSET, prev_mres=MRES_UNSET; \
	MPlugin *iplug; \
	FN_TYPE pfn_routine=NULL; \
	int loglevel=api_info_table.pfnName.loglevel; \
	const char *pfn_string=api_info_table.pfnName.name; \
	meta_globals_t backup_meta_globals; \
	/*Fix bug with metamod-bot-plugins*/ \
	if (CALL_API_count++>0) \
		/*Backup PublicMetaGlobals*/ \
		backup_meta_globals = PublicMetaGlobals;

// call each plugin
#define CALL_PLUGIN_API(post, ret_init, pfnName, pfn_args, MRES_TYPE, api_table) \
	override_ret=ret_init; \
	prev_mres=MRES_UNSET; \
	for(i=0; i < g_plugins->endlist; i++) { \
		if (g_plugins->plist[i].status != PL_RUNNING) \
			continue; \
		iplug=&g_plugins->plist[i]; \
		if(iplug->api_table && (pfn_routine=iplug->api_table->pfnName)); \
		else \
			/* plugin doesn't provide this function */  \
			continue; \
		/* initialize PublicMetaGlobals */ \
		PublicMetaGlobals.mres = MRES_UNSET; \
		PublicMetaGlobals.prev_mres = prev_mres; \
		PublicMetaGlobals.status = status; \
		pub_orig_ret = orig_ret; \
		PublicMetaGlobals.orig_ret = &pub_orig_ret; \
		if(status==MRES_TYPE) { \
			pub_override_ret = override_ret; \
			PublicMetaGlobals.override_ret = &pub_override_ret; \
		} \
		/* call plugin */ \
		META_DEBUG(loglevel, ("Calling %s:%s%s()", iplug->file, pfn_string, (post?"_Post":""))); \
		dllret=pfn_routine pfn_args; \
		/* plugin's result code */ \
		mres=PublicMetaGlobals.mres; \
		if(mres > status) \
			status = mres; \
		/* save this for successive plugins to see */ \
		prev_mres = mres; \
		if(mres==MRES_TYPE) \
			override_ret = pub_override_ret = dllret; \
		else if(mres==MRES_UNSET) \
			META_ERROR("Plugin didn't set meta_result: %s:%s%s()", iplug->file, pfn_string, (post?"_Post":"")); \
		else if(post && mres==MRES_SUPERCEDE) \
			META_ERROR("MRES_SUPERCEDE not valid in Post functions: %s:%s%s()", iplug->file, pfn_string, (post?"_Post":"")); \
	}

// call "real" function, from gamedll
#define CALL_GAME_API(pfnName, pfn_args, api_table) \
	CALL_API_count--; \
	if(status==MRES_SUPERCEDE) { \
		META_DEBUG(loglevel, ("Skipped (supercede) %s:%s()", GameDLL.file, pfn_string)); \
		orig_ret = pub_orig_ret = override_ret; \
		PublicMetaGlobals.orig_ret = &pub_orig_ret; \
		/* don't return here; superceded game routine, but still allow \
		 * _post routines to run. \
		 */ \
	} \
	else if(GameDLL.funcs.api_table) { \
		pfn_routine=GameDLL.funcs.api_table->pfnName; \
		if(pfn_routine) { \
			META_DEBUG(loglevel, ("Calling %s:%s()", GameDLL.file, pfn_string)); \
			dllret=pfn_routine pfn_args; \
			orig_ret = dllret; \
		} \
		/* don't complain for NULL routines in NEW_DLL_FUNCTIONS  */ \
		else if((void*) GameDLL.funcs.api_table != (void*) GameDLL.funcs.newapi_table) { \
			META_ERROR("Couldn't find api call: %s:%s", GameDLL.file, pfn_string); \
			status=MRES_UNSET; \
		} \
	} \
	else { \
		META_DEBUG(loglevel, ("No api table defined for api call: %s:%s", GameDLL.file, pfn_string)); \
	} \
	CALL_API_count++;

// call "real" function, from engine
#define CALL_ENGINE_API(pfnName, pfn_args) \
	CALL_API_count--; \
	if(status==MRES_SUPERCEDE) { \
		META_DEBUG(loglevel, ("Skipped (supercede) engine:%s()", pfn_string)); \
		orig_ret = pub_orig_ret = override_ret; \
		PublicMetaGlobals.orig_ret = &pub_orig_ret; \
		/* don't return here; superceded game routine, but still allow \
		 * _post routines to run. \
		 */ \
	} \
	else { \
		pfn_routine=g_engine.funcs->pfnName; \
		if(pfn_routine) { \
			META_DEBUG(loglevel, ("Calling engine:%s()", pfn_string)); \
			dllret=pfn_routine pfn_args; \
			orig_ret = dllret; \
		} \
		else { \
			META_ERROR("Couldn't find api call: engine:%s", pfn_string); \
			status=MRES_UNSET; \
		} \
	} \
	CALL_API_count++;

// return a value
#define RETURN_API() \
	if (--CALL_API_count>0) \
		/*Restore backup*/ \
		PublicMetaGlobals = backup_meta_globals; \
	if(status==MRES_OVERRIDE) { \
		META_DEBUG(loglevel, ("Returning (override) %s()", pfn_string)); \
		return(override_ret); \
	} \
	else \
		return(orig_ret);

// ===== end macros ===========================================================


#endif /* METAMOD_H */
