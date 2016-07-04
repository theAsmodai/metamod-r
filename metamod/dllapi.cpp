// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dllapi.cpp - implementation of Half-Life DLL routines

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

#include <stddef.h>			// offsetof

#include <extdll.h>			// always

#include "dllapi.h"			// me
#include "metamod.h"		// SETUP_API_CALLS, etc
#include "api_info.h"		// dllapi_info, etc
#include "commands_meta.h"	// client_meta, etc
#include "log_meta.h"		// META_ERROR, etc
#include "api_hook.h"


// Original DLL routines, functions returning "void".
#define META_DLLAPI_HANDLE_void(FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	main_hook_function_void(offsetof(dllapi_info_t, pfnName), e_api_dllapi, offsetof(DLL_FUNCTIONS, pfnName), &packed_args); \
	API_END_TSC_TRACKING()

// Original DLL routines, functions returning an actual value.
#define META_DLLAPI_HANDLE(ret_t, ret_init, FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	class_ret_t ret_val(main_hook_function(class_ret_t((ret_t)ret_init), offsetof(dllapi_info_t, pfnName), e_api_dllapi, offsetof(DLL_FUNCTIONS, pfnName), &packed_args)); \
	API_END_TSC_TRACKING()

// The "new" api routines (just 3 right now), functions returning "void".
#define META_NEWAPI_HANDLE_void(FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	main_hook_function_void(offsetof(newapi_info_t, pfnName), e_api_newapi, offsetof(NEW_DLL_FUNCTIONS, pfnName), &packed_args); \
	API_END_TSC_TRACKING()

// The "new" api routines (just 3 right now), functions returning an actual value.
#define META_NEWAPI_HANDLE(ret_t, ret_init, FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	class_ret_t ret_val(main_hook_function(class_ret_t((ret_t)ret_init), offsetof(newapi_info_t, pfnName), e_api_newapi, offsetof(NEW_DLL_FUNCTIONS, pfnName), &packed_args)); \
	API_END_TSC_TRACKING()


// From SDK dlls/game.cpp:
static void mm_GameDLLInit(void) {
	META_DLLAPI_HANDLE_void(FN_GAMEINIT, pfnGameInit, void, (VOID_ARG));
	RETURN_API_void();
}

// From SDK dlls/cbase.cpp:
static int mm_DispatchSpawn(edict_t *pent) {
	// 0==Success, -1==Failure ?
	META_DLLAPI_HANDLE(int, 0, FN_DISPATCHSPAWN, pfnSpawn, p, (pent));
	RETURN_API(int);
}
static void mm_DispatchThink(edict_t *pent) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHTHINK, pfnThink, p, (pent));
	RETURN_API_void();
}
static void mm_DispatchUse(edict_t *pentUsed, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHUSE, pfnUse, 2p, (pentUsed, pentOther));
	RETURN_API_void();
}
static void mm_DispatchTouch(edict_t *pentTouched, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHTOUCH, pfnTouch, 2p, (pentTouched, pentOther));
	RETURN_API_void();
}
static void mm_DispatchBlocked(edict_t *pentBlocked, edict_t *pentOther) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHBLOCKED, pfnBlocked, 2p, (pentBlocked, pentOther));
	RETURN_API_void();
}
static void mm_DispatchKeyValue(edict_t *pentKeyvalue, KeyValueData *pkvd) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHKEYVALUE, pfnKeyValue, 2p, (pentKeyvalue, pkvd));
	RETURN_API_void();
}
static void mm_DispatchSave(edict_t *pent, SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHSAVE, pfnSave, 2p, (pent, pSaveData));
	RETURN_API_void();
}
static int mm_DispatchRestore(edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity) {
	// 0==Success, -1==Failure ?
	META_DLLAPI_HANDLE(int, 0, FN_DISPATCHRESTORE, pfnRestore, 2pi, (pent, pSaveData, globalEntity));
	RETURN_API(int);
}
static void mm_DispatchObjectCollsionBox(edict_t *pent) {
	META_DLLAPI_HANDLE_void(FN_DISPATCHOBJECTCOLLISIONBOX, pfnSetAbsBox, p, (pent));
	RETURN_API_void();
}
static void mm_SaveWriteFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount) {
	META_DLLAPI_HANDLE_void(FN_SAVEWRITEFIELDS, pfnSaveWriteFields, 4pi, (pSaveData, pname, pBaseData, pFields, fieldCount));
	RETURN_API_void();
}
static void mm_SaveReadFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount) {
	META_DLLAPI_HANDLE_void(FN_SAVEREADFIELDS, pfnSaveReadFields, 4pi, (pSaveData, pname, pBaseData, pFields, fieldCount));
	RETURN_API_void();
}

// From SDK dlls/world.cpp:
static void mm_SaveGlobalState(SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_SAVEGLOBALSTATE, pfnSaveGlobalState, p, (pSaveData));
	RETURN_API_void();
}
static void mm_RestoreGlobalState(SAVERESTOREDATA *pSaveData) {
	META_DLLAPI_HANDLE_void(FN_RESTOREGLOBALSTATE, pfnRestoreGlobalState, p, (pSaveData));
	RETURN_API_void();
}
static void mm_ResetGlobalState(void) {
	META_DLLAPI_HANDLE_void(FN_RESETGLOBALSTATE, pfnResetGlobalState, void, (VOID_ARG));
	RETURN_API_void();
}

// From SDK dlls/client.cpp:
static qboolean mm_ClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]) {
	g_Players.clear_player_cvar_query(pEntity);
	META_DLLAPI_HANDLE(qboolean, TRUE, FN_CLIENTCONNECT, pfnClientConnect, 4p, (pEntity, pszName, pszAddress, szRejectReason));
	RETURN_API(qboolean);
}
static void mm_ClientDisconnect(edict_t *pEntity) {
	g_Players.clear_player_cvar_query(pEntity);
	META_DLLAPI_HANDLE_void(FN_CLIENTDISCONNECT, pfnClientDisconnect, p, (pEntity));
	RETURN_API_void();
}
static void mm_ClientKill(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_CLIENTKILL, pfnClientKill, p, (pEntity));
	RETURN_API_void();
}
static void mm_ClientPutInServer(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_CLIENTPUTINSERVER, pfnClientPutInServer, p, (pEntity));
	RETURN_API_void();
}
static void mm_ClientCommand(edict_t *pEntity) {
	if(Config->clientmeta && strmatch(CMD_ARGV(0), "meta")) {
		client_meta(pEntity);
	}
	META_DLLAPI_HANDLE_void(FN_CLIENTCOMMAND, pfnClientCommand, p, (pEntity));
	RETURN_API_void();
}
static void mm_ClientUserInfoChanged(edict_t *pEntity, char *infobuffer) {
	META_DLLAPI_HANDLE_void(FN_CLIENTUSERINFOCHANGED, pfnClientUserInfoChanged, 2p, (pEntity, infobuffer));
	RETURN_API_void();
}
static void mm_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
	META_DLLAPI_HANDLE_void(FN_SERVERACTIVATE, pfnServerActivate, p2i, (pEdictList, edictCount, clientMax));
	RETURN_API_void();
}
static void mm_ServerDeactivate(void) {
	META_DLLAPI_HANDLE_void(FN_SERVERDEACTIVATE, pfnServerDeactivate, void, (VOID_ARG));
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
	Plugins->refresh(PT_CHANGELEVEL);
	Plugins->unpause_all();
	// Plugins->retry_all(PT_CHANGELEVEL);
	g_Players.clear_all_cvar_queries();
	requestid_counter = 0;
	RETURN_API_void();
}
static void mm_PlayerPreThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_PLAYERPRETHINK, pfnPlayerPreThink, p, (pEntity));
	RETURN_API_void();
}
static void mm_PlayerPostThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_PLAYERPOSTTHINK, pfnPlayerPostThink, p, (pEntity));
	RETURN_API_void();
}
static void mm_StartFrame(void) {
	meta_debug_value = (int)meta_debug.value;

	META_DLLAPI_HANDLE_void(FN_STARTFRAME, pfnStartFrame, void, (VOID_ARG));
	RETURN_API_void();
}
static void mm_ParmsNewLevel(void) {
	META_DLLAPI_HANDLE_void(FN_PARMSNEWLEVEL, pfnParmsNewLevel, void, (VOID_ARG));
	RETURN_API_void();
}
static void mm_ParmsChangeLevel(void) {
	META_DLLAPI_HANDLE_void(FN_PARMSCHANGELEVEL, pfnParmsChangeLevel, void, (VOID_ARG));
	RETURN_API_void();
}
static const char *mm_GetGameDescription(void) {
	META_DLLAPI_HANDLE(const char *, NULL, FN_GETGAMEDESCRIPTION, pfnGetGameDescription, void, (VOID_ARG));
	RETURN_API(const char *);
}
static void mm_PlayerCustomization(edict_t *pEntity, customization_t *pCust) {
	META_DLLAPI_HANDLE_void(FN_PLAYERCUSTOMIZATION, pfnPlayerCustomization, 2p, (pEntity, pCust));
	RETURN_API_void();
}
static void mm_SpectatorConnect(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORCONNECT, pfnSpectatorConnect, p, (pEntity));
	RETURN_API_void();
}
static void mm_SpectatorDisconnect(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORDISCONNECT, pfnSpectatorDisconnect, p, (pEntity));
	RETURN_API_void();
}
static void mm_SpectatorThink(edict_t *pEntity) {
	META_DLLAPI_HANDLE_void(FN_SPECTATORTHINK, pfnSpectatorThink, p, (pEntity));
	RETURN_API_void();
}
static void mm_Sys_Error(const char *error_string) {
	META_DLLAPI_HANDLE_void(FN_SYS_ERROR, pfnSys_Error, p, (error_string));
	RETURN_API_void();
}

// From SDK pm_shared/pm_shared.c:
static void mm_PM_Move (struct playermove_s *ppmove, int server) {
	META_DLLAPI_HANDLE_void(FN_PM_MOVE, pfnPM_Move, pi, (ppmove, server));
	RETURN_API_void();
}
static void mm_PM_Init(struct playermove_s *ppmove) {
	META_DLLAPI_HANDLE_void(FN_PM_INIT, pfnPM_Init, p, (ppmove));
	RETURN_API_void();
}
static char mm_PM_FindTextureType(char *name) {
	META_DLLAPI_HANDLE(char, '\0', FN_PM_FINDTEXTURETYPE, pfnPM_FindTextureType, p, (name));
	RETURN_API(char);
}

// From SDK dlls/client.cpp:
static void mm_SetupVisibility(edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas) {
	META_DLLAPI_HANDLE_void(FN_SETUPVISIBILITY, pfnSetupVisibility, 4p, (pViewEntity, pClient, pvs, pas));
	RETURN_API_void();
}
static void mm_UpdateClientData (const struct edict_s *ent, int sendweapons, struct clientdata_s *cd) {
	META_DLLAPI_HANDLE_void(FN_UPDATECLIENTDATA, pfnUpdateClientData, pip, (ent, sendweapons, cd));
	RETURN_API_void();
}
static int mm_AddToFullPack(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet) {
	META_DLLAPI_HANDLE(int, 0, FN_ADDTOFULLPACK, pfnAddToFullPack, pi2p2ip, (state, e, ent, host, hostflags, player, pSet));
	RETURN_API(int);
}
static void mm_CreateBaseline(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs) {
	META_DLLAPI_HANDLE_void(FN_CREATEBASELINE, pfnCreateBaseline, 2i2pi2p, (player, eindex, baseline, entity, playermodelindex, (float*)player_mins, (float*)player_maxs));
	RETURN_API_void();
}
static void mm_RegisterEncoders(void) {
	META_DLLAPI_HANDLE_void(FN_REGISTERENCODERS, pfnRegisterEncoders, void, (VOID_ARG));
	RETURN_API_void();
}
static int mm_GetWeaponData(struct edict_s *player, struct weapon_data_s *info) {
	META_DLLAPI_HANDLE(int, 0, FN_GETWEAPONDATA, pfnGetWeaponData, 2p, (player, info));
	RETURN_API(int);
}
static void mm_CmdStart(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed) {
	META_DLLAPI_HANDLE_void(FN_CMDSTART, pfnCmdStart, 2pui, (player, cmd, random_seed));
	RETURN_API_void();
}
static void mm_CmdEnd (const edict_t *player) {
	META_DLLAPI_HANDLE_void(FN_CMDEND, pfnCmdEnd, p, (player));
	RETURN_API_void();
}
static int mm_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size) {
	META_DLLAPI_HANDLE(int, 0, FN_CONNECTIONLESSPACKET, pfnConnectionlessPacket, 4p, (net_from, args, response_buffer, response_buffer_size));
	RETURN_API(int);
}
static int mm_GetHullBounds(int hullnumber, float *mins, float *maxs) {
	META_DLLAPI_HANDLE(int, 0, FN_GETHULLBOUNDS, pfnGetHullBounds, i2p, (hullnumber, mins, maxs));
	RETURN_API(int);
}
static void mm_CreateInstancedBaselines (void) {
	META_DLLAPI_HANDLE_void(FN_CREATEINSTANCEDBASELINES, pfnCreateInstancedBaselines, void, (VOID_ARG));
	RETURN_API_void();
}
static int mm_InconsistentFile(const edict_t *player, const char *filename, char *disconnect_message) {
	META_DLLAPI_HANDLE(int, 0, FN_INCONSISTENTFILE, pfnInconsistentFile, 3p, (player, filename, disconnect_message));
	RETURN_API(int);
}
static int mm_AllowLagCompensation(void) {
	META_DLLAPI_HANDLE(int, 0, FN_ALLOWLAGCOMPENSATION, pfnAllowLagCompensation, void, (VOID_ARG));
	RETURN_API(int);
}


// New API functions
// From SDK ?
static void mm_OnFreeEntPrivateData(edict_t *pEnt) {
	META_NEWAPI_HANDLE_void(FN_ONFREEENTPRIVATEDATA, pfnOnFreeEntPrivateData, p, (pEnt));
	RETURN_API_void();
}
static void mm_GameShutdown(void) {
	META_NEWAPI_HANDLE_void(FN_GAMESHUTDOWN, pfnGameShutdown, void, (VOID_ARG));
	RETURN_API_void();
}
static int mm_ShouldCollide(edict_t *pentTouched, edict_t *pentOther) {
	META_NEWAPI_HANDLE(int, 1, FN_SHOULDCOLLIDE, pfnShouldCollide, 2p, (pentTouched, pentOther));
	RETURN_API(int);
}
// Added 2005/08/11 (no SDK update):
static void mm_CvarValue(const edict_t *pEnt, const char *value) {
	g_Players.clear_player_cvar_query(pEnt);
	META_NEWAPI_HANDLE_void(FN_CVARVALUE, pfnCvarValue, 2p, (pEnt, value));
	
	RETURN_API_void();
}
// Added 2005/11/21 (no SDK update):
static void mm_CvarValue2(const edict_t *pEnt, int requestID, const char *cvarName, const char *value) {
	META_NEWAPI_HANDLE_void(FN_CVARVALUE2, pfnCvarValue2, pi2p, (pEnt, requestID, cvarName, value));
	
	RETURN_API_void();
}


// From SDK dlls/cbase.cpp:
// "(wd)" indicates my comments on the functions
static DLL_FUNCTIONS gFunctionTable = 
{
	mm_GameDLLInit,				//! pfnGameInit()				Initialize the game (one-time call after loading of game .dll)
	mm_DispatchSpawn,				//! pfnSpawn()
	mm_DispatchThink,				//! pfnThink()
	mm_DispatchUse,				//! pfnUse()
	mm_DispatchTouch,				//! pfnTouch()
	mm_DispatchBlocked,			//! pfnBlocked()
	mm_DispatchKeyValue,			//! pfnKeyValue()
	mm_DispatchSave,				//! pfnSave()
	mm_DispatchRestore,			//! pfnRestore()
	mm_DispatchObjectCollsionBox,	//! pfnSetAbsBox()

	mm_SaveWriteFields,			//! pfnSaveWriteFields()
	mm_SaveReadFields,				//! pfnSaveReadFields()

	mm_SaveGlobalState,			//! pfnSaveGlobalState()
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
	mm_PlayerPostThink,			//! pfnPlayerPostThink()

	mm_StartFrame,					//! pfnStartFrame()
	mm_ParmsNewLevel,				//! pfnParmsNewLevel()
	mm_ParmsChangeLevel,			//! pfnParmsChangeLevel()

	mm_GetGameDescription,			//! pfnGetGameDescription()		Returns string describing current .dll.  E.g. "TeamFotrress 2", "Half-Life"
	mm_PlayerCustomization,		//! pfnPlayerCustomization()	Notifies .dll of new customization for player.

	mm_SpectatorConnect,			//! pfnSpectatorConnect()		Called when spectator joins server
	mm_SpectatorDisconnect,		//! pfnSpectatorDisconnect()	Called when spectator leaves the server
	mm_SpectatorThink,				//! pfnSpectatorThink()			Called when spectator sends a command packet (usercmd_t)

	mm_Sys_Error,					//! pfnSys_Error()				Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.  SDK2

	mm_PM_Move,					//! pfnPM_Move()				(wd) SDK2
	mm_PM_Init,					//! pfnPM_Init()				Server version of player movement initialization; (wd) SDK2
	mm_PM_FindTextureType,			//! pfnPM_FindTextureType()		(wd) SDK2

	mm_SetupVisibility,			//! pfnSetupVisibility()		Set up PVS and PAS for networking for this client; (wd) SDK2
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

DLL_FUNCTIONS *g_pHookedDllFunctions = &gFunctionTable;

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
	if(!pFunctionTable || metamod_not_loaded) {
		META_WARNING("GetEntityAPI called with null pFunctionTable");
		return(FALSE);
	}
	else if(interfaceVersion != INTERFACE_VERSION) {
		META_WARNING("GetEntityAPI version mismatch; requested=%d ours=%d", interfaceVersion, INTERFACE_VERSION);
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return(TRUE);
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	META_DEBUG(3, ("called: GetEntityAPI2; version=%d", *interfaceVersion));
	if(!pFunctionTable || metamod_not_loaded) {
		META_WARNING("GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		META_WARNING("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
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

static meta_new_dll_functions_t sNewFunctionTable(
	&mm_OnFreeEntPrivateData,		//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
	&mm_GameShutdown,				//! pfnGameShutdown()
	&mm_ShouldCollide,				//! pfnShouldCollide()
	// Added 2005/08/11 (no SDK update):
	&mm_CvarValue,				//! pfnCvarValue()
	// Added 2005/11/21 (no SDK update):
	&mm_CvarValue2				//! pfnCvarValue2()
);

NEW_DLL_FUNCTIONS *g_pHookedNewDllFunctions = &sNewFunctionTable;

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
