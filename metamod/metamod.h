// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// metamod.h - (main) description of metamod operations

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

#ifndef METAMOD_H
#define METAMOD_H

#include "comp_dep.h"
#include "meta_api.h"			// META_RES, etc
#include "mlist.h"				// MPluginList, etc
#include "mreg.h"				// MRegCmdList, etc
#include "conf_meta.h"			// MConfig
#include "osdep.h"				// NAME_MAX, etc
#include "types_meta.h"			// mBOOL
#include "mplayer.h"                    // MPlayerList
#include "meta_eiface.h"        // HL_enginefuncs_t, meta_enginefuncs_t
#include "engine_t.h"           // engine_t, Engine

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

// metamod module handle
extern DLHANDLE metamod_handle DLLHIDDEN;

// cvar to contain version
extern cvar_t meta_version DLLHIDDEN;

// Info about the game dll/mod.
typedef struct gamedll_s {
	char name[NAME_MAX];		// ie "cstrike" (from gamedir)
	const char *desc;				// ie "Counter-Strike"
	char gamedir[PATH_MAX];		// ie "/home/willday/half-life/cstrike"
	char pathname[PATH_MAX];	// ie "/home/willday/half-life/cstrike/dlls/cs_i386.so"
	char const *file;			// ie "cs_i386.so"
	char real_pathname[PATH_MAX];	// in case pathname overridden by bot, etc
	DLHANDLE handle;
	gamedll_funcs_t funcs;		// dllapi_table, newapi_table
} gamedll_t;
extern gamedll_t GameDLL DLLHIDDEN;

// SDK variables for storing engine funcs and globals.
extern HL_enginefuncs_t g_engfuncs DLLHIDDEN;
extern globalvars_t *gpGlobals DLLHIDDEN;

// Our modified version of the engine funcs, to give to plugins.
extern meta_enginefuncs_t g_plugin_engfuncs DLLHIDDEN;

// Config structure.
extern MConfig *Config DLLHIDDEN;

// List of plugins loaded/opened/running.
extern MPluginList *Plugins DLLHIDDEN;

// List of command functions registered by plugins.
extern MRegCmdList *RegCmds DLLHIDDEN;

// List of cvar structures registered by plugins.
extern MRegCvarList *RegCvars DLLHIDDEN;

// List of user messages registered by gamedll.
extern MRegMsgList *RegMsgs DLLHIDDEN;

// Data provided to plugins.
// Separate copies to prevent plugins from modifying "readable" parts.
// See meta_api.h for meta_globals_t structure.
extern meta_globals_t PublicMetaGlobals DLLHIDDEN;
extern meta_globals_t PrivateMetaGlobals DLLHIDDEN;

// hook function tables
extern DLL_FUNCTIONS *g_pHookedDllFunctions DLLHIDDEN;
extern NEW_DLL_FUNCTIONS *g_pHookedNewDllFunctions DLLHIDDEN;

extern int metamod_not_loaded DLLHIDDEN;

// Holds cached player info, right now only things for querying cvars
// Max players is always 32, small enough that we can use a static array
extern MPlayerList g_Players DLLHIDDEN;

extern int requestid_counter DLLHIDDEN;

int DLLINTERNAL metamod_startup(void);

mBOOL DLLINTERNAL meta_init_gamedll(void);
mBOOL DLLINTERNAL meta_load_gamedll(void);

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

// return (void)
#define RETURN_API_void() \
	return;

// ===== macros for type-returning functions ==================================

// return a value
#define RETURN_API(ret_t) \
	{return(GET_RET_CLASS(ret_val, ret_t));}

// ===== end macros ===========================================================

#ifdef META_PERFMON

// ============================================================================
// Api-hook performance monitoring
// ============================================================================

extern long double total_tsc DLLHIDDEN;
extern unsigned long long count_tsc DLLHIDDEN;
extern unsigned long long active_tsc DLLHIDDEN;
extern unsigned long long min_tsc DLLHIDDEN;

inline unsigned long long DLLINTERNAL GET_TSC(void) {
	union { struct { unsigned int eax, edx;	} split; unsigned long long full; } tsc;
#ifdef __GNUC__
	__asm__ __volatile__("rdtsc":"=a"(tsc.split.eax), "=d"(tsc.split.edx));	
#else
	__asm
	{
		rdtsc
		mov tsc.split.eax, eax
		mov tsc.split.edx, edx
	}
#endif
	return(tsc.full);
}

#define API_START_TSC_TRACKING() \
	active_tsc = GET_TSC()

#define API_PAUSE_TSC_TRACKING() \
	total_tsc += GET_TSC() - active_tsc

#define API_UNPAUSE_TSC_TRACKING() \
	active_tsc = GET_TSC()

#define API_END_TSC_TRACKING() { \
		unsigned long long run_tsc = GET_TSC() - active_tsc; \
		total_tsc += run_tsc; \
		count_tsc++; \
		if(min_tsc == 0 || run_tsc < min_tsc) \
			min_tsc = run_tsc; \
	}

// ===== end ==================================================================

#else

// ===== performance monitor disabled =========================================

#define API_START_TSC_TRACKING()
#define API_PAUSE_TSC_TRACKING()
#define API_UNPAUSE_TSC_TRACKING()
#define API_END_TSC_TRACKING()

// ===== end ==================================================================

#endif /*META_PERFMON*/

#endif /* METAMOD_H */
