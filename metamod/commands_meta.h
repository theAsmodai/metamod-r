// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// commands_meta.h - prototypes for console commands

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

#ifndef COMMANDS_META_H
#define COMMANDS_META_H

#include "types_meta.h"			// mBOOL
#include "comp_dep.h"

// Flags to use for meta_cmd_doplug(), to operate on existing plugins; note
// "load" operates on a non-existing plugin thus isn't included here.
typedef enum {
	PC_NULL = 0,
	PC_PAUSE,		// pause the plugin
	PC_UNPAUSE,		// unpause the plugin
	PC_UNLOAD,		// unload the plugin
	PC_RELOAD,		// unload the plugin and load it again
	PC_RETRY,		// retry a failed operation (usually load/attach)
	PC_INFO,		// show all info about the plugin
	PC_CLEAR,		// remove a failed plugin from the list
	PC_FORCE_UNLOAD,	// forcibly unload the plugin
	PC_REQUIRE,		// require that this plugin is loaded/running
} PLUG_CMD;

void DLLINTERNAL meta_register_cmdcvar();

void DLLHIDDEN svr_meta(void); // only hidden because called from outside!

void DLLINTERNAL cmd_meta_usage(void);
void DLLINTERNAL cmd_meta_version(void);
void DLLINTERNAL cmd_meta_gpl(void);

void DLLINTERNAL cmd_meta_game(void);
void DLLINTERNAL cmd_meta_refresh(void);
void DLLINTERNAL cmd_meta_load(void);

void DLLINTERNAL cmd_meta_pluginlist(void);
void DLLINTERNAL cmd_meta_cmdlist(void);
void DLLINTERNAL cmd_meta_cvarlist(void);
void DLLINTERNAL cmd_meta_config(void);

void DLLINTERNAL cmd_doplug(PLUG_CMD pcmd);

void DLLINTERNAL client_meta(edict_t *pEntity);
void DLLINTERNAL client_meta_usage(edict_t *pEntity);
void DLLINTERNAL client_meta_version(edict_t *pEntity);
void DLLINTERNAL client_meta_pluginlist(edict_t *pEntity);
void DLLINTERNAL client_meta_aybabtu(edict_t *pEntity);

#endif /* COMMANDS_META_H */
