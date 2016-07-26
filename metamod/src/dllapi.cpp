// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dllapi.cpp - implementation of Half-Life DLL routines

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

#include "precompiled.h"

// Original DLL routines, functions returning "void".
#define META_DLLAPI_HANDLE_void(FN_TYPE, pfnName, pfn_args) \
	SETUP_API_CALLS_void(FN_TYPE, pfnName, dllapi_info); \
	CALL_PLUGIN_API_void(P_PRE, pfnName, pfn_args, dllapi_table); \
	CALL_GAME_API_void(pfnName, pfn_args, dllapi_table); \
	CALL_PLUGIN_API_void(P_POST, pfnName, pfn_args, dllapi_post_table);

// Original DLL routines, functions returning an actual value.
#define META_DLLAPI_HANDLE(ret_t, ret_init, FN_TYPE, pfnName, pfn_args) \
	SETUP_API_CALLS(ret_t, ret_init, FN_TYPE, pfnName, dllapi_info); \
	CALL_PLUGIN_API(P_PRE, ret_init, pfnName, pfn_args, MRES_SUPERCEDE, dllapi_table); \
	CALL_GAME_API(pfnName, pfn_args, dllapi_table); \
	CALL_PLUGIN_API(P_POST, ret_init, pfnName, pfn_args, MRES_OVERRIDE, dllapi_post_table);


// The "new" api routines (just 3 right now), functions returning "void".
#define META_NEWAPI_HANDLE_void(FN_TYPE, pfnName, pfn_args) \
	SETUP_API_CALLS_void(FN_TYPE, pfnName, newapi_info); \
	CALL_PLUGIN_API_void(P_PRE, pfnName, pfn_args, newapi_table); \
	CALL_GAME_API_void(pfnName, pfn_args, newapi_table); \
	CALL_PLUGIN_API_void(P_POST, pfnName, pfn_args, newapi_post_table);

// The "new" api routines (just 3 right now), functions returning an actual value.
#define META_NEWAPI_HANDLE(ret_t, ret_init, FN_TYPE, pfnName, pfn_args) \
	SETUP_API_CALLS(ret_t, ret_init, FN_TYPE, pfnName, newapi_info); \
	CALL_PLUGIN_API(P_PRE, ret_init, pfnName, pfn_args, MRES_SUPERCEDE, newapi_table); \
	CALL_GAME_API(pfnName, pfn_args, newapi_table); \
	CALL_PLUGIN_API(P_POST, ret_init, pfnName, pfn_args, MRES_OVERRIDE, newapi_post_table);


// From SDK dlls/game.cpp:
void mm_GameDLLInit(void) {
	META_DLLAPI_HANDLE_void(FN_GAMEINIT, pfnGameInit, ());
	RETURN_API_void();
}

// From SDK dlls/cbase.cpp:
int mm_DispatchSpawn(edict_t *pent) {
	// 0==Success, -1==Failure ?
	META_DLLAPI_HANDLE(int, 0, FN_DISPATCHSPAWN, pfnSpawn, (pent));
	RETURN_API();
}
void mm_DispatchThink(edict_t *pent) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHTHINK, pfnThink, (pent));
	RETURN_API_void();
}
void mm_DispatchUse(edict_t *pentUsed, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHUSE, pfnUse, (pentUsed, pentOther));
	RETURN_API_void();
}
void mm_DispatchTouch(edict_t *pentTouched, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHTOUCH, pfnTouch, (pentTouched, pentOther));
	RETURN_API_void();
}
void mm_DispatchBlocked(edict_t *pentBlocked, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHBLOCKED, pfnBlocked, (pentBlocked, pentOther));
	RETURN_API_void();
}
void mm_DispatchKeyValue(edict_t *pentKeyvalue, KeyValueData *pkvd) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHKEYVALUE, pfnKeyValue, (pentKeyvalue, pkvd));
	RETURN_API_void();
}
void mm_DispatchSave(edict_t *pent, SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHSAVE, pfnSave, (pent, pSaveData));
	RETURN_API_void();
}
int mm_DispatchRestore(edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity) {
	// 0==Success, -1==Failure ?
	META_DLLAPI_HANDLE(int, 0, FN_DISPATCHRESTORE, pfnRestore, (pent, pSaveData, globalEntity));
	RETURN_API();
}
void mm_DispatchObjectCollsionBox(edict_t *pent) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHOBJECTCOLLISIONBOX, pfnSetAbsBox, (pent));
	RETURN_API_void();
}
void mm_SaveWriteFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount) {
	META_DLLAPI_HANDLE_void(FN_SAVEWRITEFIELDS, pfnSaveWriteFields, (pSaveData, pname, pBaseData, pFields, fieldCount));
	RETURN_API_void();
}
void mm_SaveReadFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount) {
	META_DLLAPI_HANDLE_void(FN_SAVEREADFIELDS, pfnSaveReadFields, (pSaveData, pname, pBaseData, pFields, fieldCount));
	RETURN_API_void();
}

// From SDK dlls/world.cpp:
void mm_SaveGlobalState(SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_SAVEGLOBALSTATE, pfnSaveGlobalState, (pSaveData));
	RETURN_API_void();
}
void mm_RestoreGlobalState(SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_RESTOREGLOBALSTATE, pfnRestoreGlobalState, (pSaveData));
	RETURN_API_void();
}
void mm_ResetGlobalState(void) {
	META_DLLAPI_HANDLE_void(FN_RESETGLOBALSTATE, pfnResetGlobalState, ());
	RETURN_API_void();
}

// From SDK dlls/client.cpp:
BOOL mm_ClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] ) {
	g_Players.clear_player_cvar_query(pEntity);
	META_DLLAPI_HANDLE(BOOL, TRUE, FN_CLIENTCONNECT, pfnClientConnect, (pEntity, pszName, pszAddress, szRejectReason));
	RETURN_API();
}
void mm_ClientDisconnect(edict_t *pEntity) {
	g_Players.clear_player_cvar_query(pEntity);
	META_DLLAPI_HANDLE_void(FN_CLIENTDISCONNECT, pfnClientDisconnect, (pEntity));
	RETURN_API_void();
}
void mm_ClientKill(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_CLIENTKILL, pfnClientKill, (pEntity));
	RETURN_API_void();
}
void mm_ClientPutInServer(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_CLIENTPUTINSERVER, pfnClientPutInServer, (pEntity));
	RETURN_API_void();
}
void mm_ClientCommand(edict_t *pEntity) {
	if(!strcmp(CMD_ARGV(0), "meta")) {
		client_meta(pEntity);
	}
	META_DLLAPI_HANDLE_void(FN_CLIENTCOMMAND, pfnClientCommand, (pEntity));
	RETURN_API_void();
}
void mm_ClientUserInfoChanged(edict_t *pEntity, char *infobuffer) {
	META_DLLAPI_HANDLE_void(FN_CLIENTUSERINFOCHANGED, pfnClientUserInfoChanged, (pEntity, infobuffer));
	RETURN_API_void();
}
void mm_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
	META_DLLAPI_HANDLE_void(FN_SERVERACTIVATE, pfnServerActivate, (pEdictList, edictCount, clientMax));
	RETURN_API_void();
}
void mm_ServerDeactivate(void) {
	META_DLLAPI_HANDLE_void(FN_SERVERDEACTIVATE, pfnServerDeactivate, ());
	// Update loaded plugins.  Look for new plugins in inifile, as well as
	// any plugins waiting for a changelevel to load.  
	//
	// This is done in ServerDeactivate rather than Activate, as the latter
	// isn't actually the first routine to be called on a new map.  In
	// particular DispatchKeyValue and DispatchSpawn are called before
	// Activate, and we want any newly loaded plugins to be able to catch
	// these.
	//
	// So, we do this from Deactivate, which is the _last_ routine called
	// from the previous map.  It's also called right before shutdown,
	// which means whenever hlds quits, it'll reload the plugins just
	// before it exits, which is rather silly, but oh well.
	g_plugins->refresh(PT_CHANGELEVEL);
	g_plugins->unpause_all();
	// g_plugins->retry_all(PT_CHANGELEVEL);
	g_Players.clear_all_cvar_queries();
	requestid_counter = 0;
	RETURN_API_void();
}
void mm_PlayerPreThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_PLAYERPRETHINK, pfnPlayerPreThink, (pEntity));
	RETURN_API_void();
}
void mm_PlayerPostThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_PLAYERPOSTTHINK, pfnPlayerPostThink, (pEntity));
	RETURN_API_void();
}
void mm_StartFrame(void) {
	META_DLLAPI_HANDLE_void(FN_STARTFRAME, pfnStartFrame, ());
	RETURN_API_void();
}
void mm_ParmsNewLevel(void) {
	META_DLLAPI_HANDLE_void(FN_PARMSNEWLEVEL, pfnParmsNewLevel, ());
	RETURN_API_void();
}
void mm_ParmsChangeLevel(void) {
	META_DLLAPI_HANDLE_void(FN_PARMSCHANGELEVEL, pfnParmsChangeLevel, ());
	RETURN_API_void();
}
const char *mm_GetGameDescription(void) {
	META_DLLAPI_HANDLE(const char *, NULL, FN_GETGAMEDESCRIPTION, pfnGetGameDescription, ());
	RETURN_API();
}
void mm_PlayerCustomization(edict_t *pEntity, customization_t *pCust) {
	META_DLLAPI_HANDLE_void(FN_PLAYERCUSTOMIZATION, pfnPlayerCustomization, (pEntity, pCust));
	RETURN_API_void();
}
void mm_SpectatorConnect(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORCONNECT, pfnSpectatorConnect, (pEntity));
	RETURN_API_void();
}
void mm_SpectatorDisconnect(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORDISCONNECT, pfnSpectatorDisconnect, (pEntity));
	RETURN_API_void();
}
void mm_SpectatorThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORTHINK, pfnSpectatorThink, (pEntity));
	RETURN_API_void();
}
void mm_Sys_Error(const char *error_string) {
	META_DLLAPI_HANDLE_void(FN_SYS_ERROR, pfnSys_Error, (error_string));
	RETURN_API_void();
}

// From SDK pm_shared/pm_shared.c:
void mm_PM_Move (struct playermove_s *ppmove, int server) {
	META_DLLAPI_HANDLE_void(FN_PM_MOVE, pfnPM_Move, (ppmove, server));
	RETURN_API_void();
}
void mm_PM_Init(struct playermove_s *ppmove) {
	META_DLLAPI_HANDLE_void(FN_PM_INIT, pfnPM_Init, (ppmove));
	RETURN_API_void();
}
char mm_PM_FindTextureType(char *name) {
	META_DLLAPI_HANDLE(char, '\0', FN_PM_FINDTEXTURETYPE, pfnPM_FindTextureType, (name));
	RETURN_API();
}

// From SDK dlls/client.cpp:
void mm_SetupVisibility(edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas) {
	META_DLLAPI_HANDLE_void(FN_SETUPVISIBILITY, pfnSetupVisibility, (pViewEntity, pClient, pvs, pas));
	RETURN_API_void();
}
void mm_UpdateClientData (const struct edict_s *ent, int sendweapons, struct clientdata_s *cd) {
	META_DLLAPI_HANDLE_void(FN_UPDATECLIENTDATA, pfnUpdateClientData, (ent, sendweapons, cd));
	RETURN_API_void();
}
int mm_AddToFullPack(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet) {
	META_DLLAPI_HANDLE(int, 0, FN_ADDTOFULLPACK, pfnAddToFullPack, (state, e, ent, host, hostflags, player, pSet));
	RETURN_API();
}
void mm_CreateBaseline(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs) {
	META_DLLAPI_HANDLE_void(FN_CREATEBASELINE, pfnCreateBaseline, (player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs));
	RETURN_API_void();
}
void mm_RegisterEncoders(void) {
	META_DLLAPI_HANDLE_void(FN_REGISTERENCODERS, pfnRegisterEncoders, ());
	RETURN_API_void();
}
int mm_GetWeaponData(struct edict_s *player, struct weapon_data_s *info) {
	META_DLLAPI_HANDLE(int, 0, FN_GETWEAPONDATA, pfnGetWeaponData, (player, info));
	RETURN_API();
}
void mm_CmdStart(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed) {
	META_DLLAPI_HANDLE_void(FN_CMDSTART, pfnCmdStart, (player, cmd, random_seed));
	RETURN_API_void();
}
void mm_CmdEnd (const edict_t *player) {
	META_DLLAPI_HANDLE_void(FN_CMDEND, pfnCmdEnd, (player));
	RETURN_API_void();
}
int mm_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size) {
	META_DLLAPI_HANDLE(int, 0, FN_CONNECTIONLESSPACKET, pfnConnectionlessPacket, (net_from, args, response_buffer, response_buffer_size));
	RETURN_API();
}
int mm_GetHullBounds(int hullnumber, float *mins, float *maxs) {
	META_DLLAPI_HANDLE(int, 0, FN_GETHULLBOUNDS, pfnGetHullBounds, (hullnumber, mins, maxs));
	RETURN_API();
}
void mm_CreateInstancedBaselines (void) {
	META_DLLAPI_HANDLE_void(FN_CREATEINSTANCEDBASELINES, pfnCreateInstancedBaselines, ());
	RETURN_API_void();
}
int mm_InconsistentFile(const edict_t *player, const char *filename, char *disconnect_message) {
	META_DLLAPI_HANDLE(int, 0, FN_INCONSISTENTFILE, pfnInconsistentFile, (player, filename, disconnect_message));
	RETURN_API();
}
int mm_AllowLagCompensation(void) {
	META_DLLAPI_HANDLE(int, 0, FN_ALLOWLAGCOMPENSATION, pfnAllowLagCompensation, ());
	RETURN_API();
}


// New API functions
// From SDK ?
void mm_OnFreeEntPrivateData(edict_t *pEnt) {
	META_NEWAPI_HANDLE_void(FN_ONFREEENTPRIVATEDATA, pfnOnFreeEntPrivateData, (pEnt));
	RETURN_API_void();
}
void mm_GameShutdown(void) {
	META_NEWAPI_HANDLE_void(FN_GAMESHUTDOWN, pfnGameShutdown, ());
	RETURN_API_void();
}
int mm_ShouldCollide(edict_t *pentTouched, edict_t *pentOther) {
	META_NEWAPI_HANDLE(int, 1, FN_SHOULDCOLLIDE, pfnShouldCollide, (pentTouched, pentOther));
	RETURN_API();
}
// Added 2005-08-11 (no SDK update)
void mm_CvarValue(const edict_t *pEdict, const char *value) {
	g_Players.clear_player_cvar_query(pEdict);
	META_NEWAPI_HANDLE_void(FN_CVARVALUE, pfnCvarValue, (pEdict, value));
	RETURN_API_void();
}

//Added 2005-11-22 (no SDK update)
void mm_CvarValue2(const edict_t *pEdict, int requestID, const char *cvarName, const char *value) {
	META_NEWAPI_HANDLE_void(FN_CVARVALUE2, pfnCvarValue2, (pEdict, requestID, cvarName, value));
	RETURN_API_void();
}

// From SDK dlls/cbase.cpp:
// "(wd)" indicates my comments on the functions
static DLL_FUNCTIONS sFunctionTable = 
{
	mm_GameDLLInit,					//! pfnGameInit()				Initialize the game (one-time call after loading of game .dll)
	mm_DispatchSpawn,				//! pfnSpawn()
	mm_DispatchThink,				//! pfnThink()
	mm_DispatchUse,					//! pfnUse()
	mm_DispatchTouch,				//! pfnTouch()
	mm_DispatchBlocked,				//! pfnBlocked()
	mm_DispatchKeyValue,			//! pfnKeyValue()
	mm_DispatchSave,				//! pfnSave()
	mm_DispatchRestore,				//! pfnRestore()
	mm_DispatchObjectCollsionBox,	//! pfnSetAbsBox()

	mm_SaveWriteFields,				//! pfnSaveWriteFields()
	mm_SaveReadFields,				//! pfnSaveReadFields()

	mm_SaveGlobalState,				//! pfnSaveGlobalState()
	mm_RestoreGlobalState,			//! pfnRestoreGlobalState()
	mm_ResetGlobalState,			//! pfnResetGlobalState()

	mm_ClientConnect,				//! pfnClientConnect()			(wd) Client has connected
	mm_ClientDisconnect,			//! pfnClientDisconnect()		(wd) Player has left the game
	mm_ClientKill,					//! pfnClientKill()				(wd) Player has typed "kill"
	mm_ClientPutInServer,			//! pfnClientPutInServer()		(wd) Client is entering the game
	mm_ClientCommand,				//! pfnClientCommand()			(wd) Player has sent a command (typed, or from a bind)
	mm_ClientUserInfoChanged,		//! pfnClientUserInfoChanged()	(wd) Client has updated their setinfo structure
	mm_ServerActivate,				//! pfnServerActivate()			(wd) Server is starting a new map
	mm_ServerDeactivate,			//! pfnServerDeactivate()		(wd) Server is leaving the map (shutdown, or changelevel); SDK2

	mm_PlayerPreThink,				//! pfnPlayerPreThink()
	mm_PlayerPostThink,				//! pfnPlayerPostThink()

	mm_StartFrame,					//! pfnStartFrame()
	mm_ParmsNewLevel,				//! pfnParmsNewLevel()
	mm_ParmsChangeLevel,			//! pfnParmsChangeLevel()

	mm_GetGameDescription,			//! pfnGetGameDescription()		Returns string describing current .dll.  E.g. "TeamFotrress 2", "Half-Life"
	mm_PlayerCustomization,			//! pfnPlayerCustomization()	Notifies .dll of new customization for player.

	mm_SpectatorConnect,			//! pfnSpectatorConnect()		Called when spectator joins server
	mm_SpectatorDisconnect,			//! pfnSpectatorDisconnect()	Called when spectator leaves the server
	mm_SpectatorThink,				//! pfnSpectatorThink()			Called when spectator sends a command packet (usercmd_t)

	mm_Sys_Error,					//! pfnSys_Error()				Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.  SDK2

	mm_PM_Move,						//! pfnPM_Move()				(wd) SDK2
	mm_PM_Init,						//! pfnPM_Init()				Server version of player movement initialization; (wd) SDK2
	mm_PM_FindTextureType,			//! pfnPM_FindTextureType()		(wd) SDK2

	mm_SetupVisibility,				//! pfnSetupVisibility()		Set up PVS and PAS for networking for this client; (wd) SDK2
	mm_UpdateClientData,			//! pfnUpdateClientData()		Set up data sent only to specific client; (wd) SDK2
	mm_AddToFullPack,				//! pfnAddToFullPack()			(wd) SDK2
	mm_CreateBaseline,				//! pfnCreateBaseline()			Tweak entity baseline for network encoding, allows setup of player baselines, too.; (wd) SDK2
	mm_RegisterEncoders,			//! pfnRegisterEncoders()		Callbacks for network encoding; (wd) SDK2
	mm_GetWeaponData,				//! pfnGetWeaponData()			(wd) SDK2
	mm_CmdStart,					//! pfnCmdStart()				(wd) SDK2
	mm_CmdEnd,						//! pfnCmdEnd()					(wd) SDK2
	mm_ConnectionlessPacket,		//! pfnConnectionlessPacket()	(wd) SDK2
	mm_GetHullBounds,				//! pfnGetHullBounds()			(wd) SDK2
	mm_CreateInstancedBaselines,	//! pfnCreateInstancedBaselines()	(wd) SDK2
	mm_InconsistentFile,			//! pfnInconsistentFile()		(wd) SDK2
	mm_AllowLagCompensation,		//! pfnAllowLagCompensation()	(wd) SDK2
};

DLL_FUNCTIONS *pHookedDllFunctions = &sFunctionTable;

// It's not clear what the difference is between GetAPI and GetAPI2; they
// both appear to return the exact same function table.  
//
// Only one of them appears to be ever called, though.  If the DLL provides
// GetAPI2, the engine/hlds will call that, and will not call GetAPI.  If
// the engine couldn't find GetAPI2 in the DLL, it appears to fall back to
// GetAPI.
//
// So, GetAPI2 appears to replace GetAPI, and appears to have been added
// with SDK 2.0.  My best guess is that, with the new SDK, interface
// version checking became important, and without the int ptr used in
// GetAPI2, the engine can't find out the version of the DLL via GetAPI.
//
// It's unclear whether a DLL coded under SDK2 needs to provide the older
// GetAPI or not..

C_DLLEXPORT int GetEntityAPI(DLL_FUNCTIONS *pFunctionTable, int interfaceVersion)
{
	META_DEBUG(3, ("called: GetEntityAPI; version=%d", interfaceVersion));
	if(!pFunctionTable) {
		META_ERROR("GetEntityAPI called with null pFunctionTable");
		return(FALSE);
	}
	else if(interfaceVersion != INTERFACE_VERSION) {
		META_ERROR("GetEntityAPI version mismatch; requested=%d ours=%d", interfaceVersion, INTERFACE_VERSION);
		return(FALSE);
	}

	
	memcpy(pFunctionTable, &sFunctionTable, sizeof(DLL_FUNCTIONS));


	return(TRUE);
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	META_DEBUG(3, ("called: GetEntityAPI2; version=%d", *interfaceVersion));

	if(!pFunctionTable) {
		META_ERROR("GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		META_ERROR("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}

	
	memcpy(pFunctionTable, &sFunctionTable, sizeof(DLL_FUNCTIONS));


	return(TRUE);
}


// I could find _no_ documentation or examples for the intended use of
// NEW_DLL_FUNCTIONS.  I wouldn't have even _known_ about the
// GetNewDLLFunctions() function except for the reference in Adminmod..  It
// appears to be new with SDK 2.0.
//
// Obviously, it seems to provide additional functions to the engine, but
// it's unclear why a new table and interface were added, rather than
// appending new functions to the GetAPI table/interface.
//
// Interestingly, it appears to be called by the engine _before_ GetAPI.

static meta_new_dll_functions_t sNewFunctionTable (
	&mm_OnFreeEntPrivateData,		//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
	&mm_GameShutdown,				//! pfnGameShutdown()
	&mm_ShouldCollide,				//! pfnShouldCollide()
	// Added 2005-08-11 (no SDK update)
	&mm_CvarValue,					//! pfnCvarValue()      (fz) Obsolete!  Use mm_CvarValue2 instead
	// Added 2005-11-22 (no SDK update)
	&mm_CvarValue2					//! pfnCvarValue2()     (fz) When pfnQueryClientCvarValue2() completes it will call
									//!                     pfnCvarValue2() with the request ID supplied earlier, the name of
									//!                     the cvar requested and the value of that cvar.
);


NEW_DLL_FUNCTIONS *pHookedNewDllFunctions = &sNewFunctionTable;

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion) 
{
	META_DEBUG(6, ("called: GetNewDLLFunctions; version=%d", *interfaceVersion));
#if 0 // ~dvander - but then you can't use cvar querying on many mods...
	// Don't provide these functions to engine if gamedll doesn't provide
	// them.  Otherwise, we're in the position of having to provide answers
	// we can't necessarily provide (for instance, ShouldCollide())...
	if(!GameDLL.funcs.newapi_table)
		return(FALSE);
#endif

	if(!pNewFunctionTable) {
		META_ERROR("GetNewDLLFunctions called with null pNewFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		META_ERROR("GetNewDLLFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return(FALSE);
	}

	sNewFunctionTable.copy_to(pNewFunctionTable);


	return(TRUE);
}

