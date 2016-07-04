// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// sdk_util.h - wrapper & extension of util.h from HL SDK

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

// Wrap util.h from SDK with ifndef/endif, to avoid problems from multiple
// inclusions.  Dunno why Valve didn't do that in util.h themselves..

#ifndef SDK_UTIL_H
#define SDK_UTIL_H

// We're not including the DBG_EntOfVars and DBG_AssertFunction routines
// mentioned in the SDK util.h, so we're going to unset DEBUG here so that
// we don't get "unresolved symbol" errors.
#ifdef DEBUG
#undef DEBUG
#endif /* DEBUG */

#include "enginecallbacks.h"
#include "comp_dep.h"
#include <util.h>


// Also, create some additional macros for engine callback functions, which
// weren't in SDK dlls/enginecallbacks.h but probably should have been.

#define GET_INFOKEYBUFFER	(*g_engfuncs.pfnGetInfoKeyBuffer)
#define INFOKEY_VALUE		(*g_engfuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEYVALUE	(*g_engfuncs.pfnSetClientKeyValue)
#define REG_SVR_COMMAND		(*g_engfuncs.pfnAddServerCommand)
#define SERVER_PRINT		(*g_engfuncs.pfnServerPrint)
#define SET_SERVER_KEYVALUE	(*g_engfuncs.pfnSetKeyValue)
#define QUERY_CLIENT_CVAR_VALUE	(*g_engfuncs.pfnQueryClientCvarValue)
#define QUERY_CLIENT_CVAR_VALUE2 (*g_engfuncs.pfnQueryClientCvarValue2)

// Add overloaded ENTINDEX() version for const edict_t pointer.
// The pfnIndexOfEdict() function takes a const edict_t pointer
// as parameter anyway, so there is no reason why ENTINDEX()
// shouldn't.
inline int ENTINDEX(const edict_t *pEdict) { 
	return((*g_engfuncs.pfnIndexOfEdict)(pEdict));
}


// Also, create some nice inlines for engine callback combos.

// Get a setinfo value from a player entity.
inline char * DLLINTERNAL ENTITY_KEYVALUE(edict_t *entity, char *key) {
	return(INFOKEY_VALUE(GET_INFOKEYBUFFER(entity), key));
}

// Set a setinfo value for a player entity.
inline void DLLINTERNAL ENTITY_SET_KEYVALUE(edict_t *entity, char *key, char *value) {
	SET_CLIENT_KEYVALUE(ENTINDEX(entity), GET_INFOKEYBUFFER(entity), key, value);
}

// Get a "serverinfo" value.
inline char * DLLINTERNAL SERVERINFO(char *key) {
	return(ENTITY_KEYVALUE(INDEXENT(0), key));
}

// Set a "serverinfo" value.
inline void DLLINTERNAL SET_SERVERINFO(char *key, char *value) {
	SET_SERVER_KEYVALUE(GET_INFOKEYBUFFER(INDEXENT(0)), key, value);
}

// Get a "localinfo" value.
inline char * DLLINTERNAL LOCALINFO(char *key) {
	return(ENTITY_KEYVALUE(NULL, key));
}

// Set a "localinfo" value.
inline void DLLINTERNAL SET_LOCALINFO(char *key, char *value) {
	SET_SERVER_KEYVALUE(GET_INFOKEYBUFFER(NULL), key, value);
}

inline int DLLINTERNAL fast_FNullEnt(const edict_t* pent) {
	return(!pent || !(*g_engfuncs.pfnEntOffsetOfPEntity)(pent));
}

// Our slightly modified version, using an edict_t pointer instead of a
// CBaseEntity pointer. (was in 1.17p1, included in 1.17.1)
void DLLINTERNAL META_UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, const char *pMessage);

const char * DLLINTERNAL META_UTIL_VarArgs(const char *format, ...);

short DLLINTERNAL FixedSigned16(float value, float scale);
unsigned short DLLINTERNAL FixedUnsigned16(float value, float scale);

#endif /* SDK_UTIL_H */
