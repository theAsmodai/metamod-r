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

#include <extdll.h>
#include <sdk_util.h>

#include <dllapi.h>
#include <meta_api.h>
#include <sdk_util.h>

#include "trace_api.h"
#include "log_plugin.h"

// from SDK dlls/game.cpp:
void GameDLLInit( void ) {
	DLL_TRACE(pfnGameInit, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

// from SDK dlls/cbase.cpp:
int DispatchSpawn( edict_t *pent ) {
	edict_t *ed=pent;
	DLL_TRACE(pfnSpawn, P_PRE, ("classname=%s",
				ed ? STRING(ed->v.classname) : "nil"));
	// 0==Success, -1==Failure ?
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DispatchThink( edict_t *pent ) {
	DLL_TRACE(pfnThink, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void DispatchUse( edict_t *pentUsed, edict_t *pentOther ) {
	DLL_TRACE(pfnUse, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void DispatchTouch( edict_t *pentTouched, edict_t *pentOther ) {
	DLL_TRACE(pfnTouch, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther ) {
	DLL_TRACE(pfnBlocked, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd ) {
	DLL_TRACE(pfnKeyValue, P_PRE, ("classname=%s keyname=%s value=%s", P_PRE,
			pkvd->szClassName, pkvd->szKeyName, pkvd->szValue));
	RETURN_META(MRES_IGNORED);
}
void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData ) {
	DLL_TRACE(pfnSave, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity ) {
	DLL_TRACE(pfnRestore, P_PRE, (""));
	// 0==Success, -1==Failure ?
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DispatchObjectCollsionBox( edict_t *pent ) {
	edict_t *ed=pent;
	DLL_TRACE(pfnSetAbsBox, P_PRE, ("classname=%s netname=%s",
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META(MRES_IGNORED);
}
void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount ) {
	DLL_TRACE(pfnSaveWriteFields, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount ) {
	DLL_TRACE(pfnSaveReadFields, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

// from SDK dlls/world.cpp:
void SaveGlobalState( SAVERESTOREDATA *pSaveData ) {
	DLL_TRACE(pfnSaveGlobalState, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void RestoreGlobalState( SAVERESTOREDATA *pSaveData ) {
	DLL_TRACE(pfnRestoreGlobalState, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ResetGlobalState( void ) {
	DLL_TRACE(pfnResetGlobalState, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

//! from SDK dlls/client.cpp:
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  ) {
	DLL_TRACE(pfnClientConnect, P_PRE, ("name=%s, addr=%s", pszName, pszAddress));
	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}
void ClientDisconnect( edict_t *pEntity ) {
	// DLL_TRACE(pfnClientDisconnect, P_PRE, ("name=%s", ENTITY_KEYVALUE(pEntity, "name")));
	DLL_TRACE(pfnClientDisconnect, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void ClientKill( edict_t *pEntity ) {
	DLL_TRACE(pfnClientKill, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void ClientPutInServer( edict_t *pEntity ) {
	DLL_TRACE(pfnClientPutInServer, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void ClientCommand( edict_t *pEntity ) {
	DLL_TRACE(pfnClientCommand, P_PRE, ("name=%s, cmd='%s %s'", STRING(pEntity->v.netname), CMD_ARGV(0), 
				CMD_ARGC() >= 1 ? CMD_ARGS() : ""));
	RETURN_META(MRES_IGNORED);
}
void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer ) {
	DLL_TRACE(pfnClientUserInfoChanged, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax ) {
	DLL_TRACE(pfnServerActivate, P_PRE, ("count=%d, max=%d", edictCount, clientMax));
	RETURN_META(MRES_IGNORED);
}
void ServerDeactivate( void ) {
	DLL_TRACE(pfnServerDeactivate, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void PlayerPreThink( edict_t *pEntity ) {
	DLL_TRACE(pfnPlayerPreThink, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void PlayerPostThink( edict_t *pEntity ) {
	DLL_TRACE(pfnPlayerPostThink, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void StartFrame( void ) {
	DLL_TRACE(pfnStartFrame, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ParmsNewLevel( void ) {
	DLL_TRACE(pfnParmsNewLevel, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ParmsChangeLevel( void ) {
	DLL_TRACE(pfnParmsChangeLevel, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
const char *GetGameDescription( void ) {
	DLL_TRACE(pfnGetGameDescription, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, "");
}
void PlayerCustomization( edict_t *pEntity, customization_t *pCust ) {
	DLL_TRACE(pfnPlayerCustomization, P_PRE, ("name=%s", STRING(pEntity->v.netname)));
	RETURN_META(MRES_IGNORED);
}
void SpectatorConnect( edict_t *pEntity ) {
	DLL_TRACE(pfnSpectatorConnect, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void SpectatorDisconnect( edict_t *pEntity ) {
	DLL_TRACE(pfnSpectatorDisconnect, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void SpectatorThink( edict_t *pEntity ) {
	DLL_TRACE(pfnSpectatorThink, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void Sys_Error( const char *error_string ) {
	DLL_TRACE(pfnSys_Error, P_PRE, ("string=%s", error_string));
	RETURN_META(MRES_IGNORED);
}

// from SDK pm_shared/pm_shared.c:
void PM_Move ( struct playermove_s *ppmove, int server ) {
	DLL_TRACE(pfnPM_Move, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void PM_Init( struct playermove_s *ppmove ) {
	DLL_TRACE(pfnPM_Init, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
char PM_FindTextureType( char *name ) {
	DLL_TRACE(pfnPM_FindTextureType, P_PRE, ("name=%s", name));
	RETURN_META_VALUE(MRES_IGNORED, '\0');
}

// from SDK dlls/client.cpp:
void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas ) {
	DLL_TRACE(pfnSetupVisibility, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd ) {
	DLL_TRACE(pfnUpdateClientData, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet ) {
	DLL_TRACE(pfnAddToFullPack, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs ) {
	DLL_TRACE(pfnCreateBaseline, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void RegisterEncoders( void ) {
	DLL_TRACE(pfnRegisterEncoders, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int GetWeaponData( struct edict_s *player, struct weapon_data_s *info ) {
	DLL_TRACE(pfnGetWeaponData, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 1);
}
void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed ) {
	DLL_TRACE(pfnCmdStart, P_PRE, ("name=%s, rand=%d", STRING(player->v.netname), random_seed));
	RETURN_META(MRES_IGNORED);
}
void CmdEnd ( const edict_t *player ) {
	DLL_TRACE(pfnCmdEnd, P_PRE, ("name=%s", STRING(player->v.netname)));
	RETURN_META(MRES_IGNORED);
}
int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size ) {
	DLL_TRACE(pfnConnectionlessPacket, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int GetHullBounds( int hullnumber, float *mins, float *maxs ) {
	DLL_TRACE(pfnGetHullBounds, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void CreateInstancedBaselines ( void ) {
	DLL_TRACE(pfnCreateInstancedBaselines, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message ) {
	DLL_TRACE(pfnInconsistentFile, P_PRE, ("name=%s, file=%s", STRING(player->v.netname), filename));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int AllowLagCompensation( void ) {
	DLL_TRACE(pfnAllowLagCompensation, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 1);
}


// from SDK ?
void OnFreeEntPrivateData(edict_t *pEnt) {
	NEWDLL_TRACE(pfnOnFreeEntPrivateData, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void GameShutdown(void) {
	NEWDLL_TRACE(pfnGameShutdown, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int ShouldCollide(edict_t *pentTouched, edict_t *pentOther) {
	NEWDLL_TRACE(pfnShouldCollide, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 1);
}

// Added 2005-08-11 (no SDK update)
void CvarValue(const edict_t *pEdict, const char *szValue) {
	NEWDLL_TRACE(pfnCvarValue, P_PRE, ("player=%s, value=%s", STRING(pEdict->v.netname), szValue?szValue:"nil"));
	RETURN_META(MRES_IGNORED);
}

// Added 2005-11-22 (no SDK update)
void CvarValue2(const edict_t *pEdict, int requestID, const char *cvarName, const char *value) {
	NEWDLL_TRACE(pfnCvarValue2, P_PRE, ("player=%s, requestID=%d, cvar=%s, value=%s", 
										STRING(pEdict->v.netname), requestID, cvarName?cvarName:"nil", value?value:"nil"));
	RETURN_META(MRES_IGNORED);
}


// from SDK dlls/cbase.cpp:
// (wd) indicates my comments on the functions
static DLL_FUNCTIONS gFunctionTable = 
{
	GameDLLInit,				//! pfnGameInit()				Initialize the game (one-time call after loading of game .dll)
	DispatchSpawn,				//! pfnSpawn()
	DispatchThink,				//! pfnThink()
	DispatchUse,				//! pfnUse()
	DispatchTouch,				//! pfnTouch()
	DispatchBlocked,			//! pfnBlocked()
	DispatchKeyValue,			//! pfnKeyValue()
	DispatchSave,				//! pfnSave()
	DispatchRestore,			//! pfnRestore()
	DispatchObjectCollsionBox,	//! pfnSetAbsBox()

	SaveWriteFields,			//! pfnSaveWriteFields()
	SaveReadFields,				//! pfnSaveReadFields()

	SaveGlobalState,			//! pfnSaveGlobalState()
	RestoreGlobalState,			//! pfnRestoreGlobalState()
	ResetGlobalState,			//! pfnResetGlobalState()

	ClientConnect,				//! pfnClientConnect()			(wd) Client has connected
	ClientDisconnect,			//! pfnClientDisconnect()		(wd) Player has left the game
	ClientKill,					//! pfnClientKill()				(wd) Player has typed "kill"
	ClientPutInServer,			//! pfnClientPutInServer()		(wd) Client is entering the game
	ClientCommand,				//! pfnClientCommand()			(wd) Player has sent a command (typed, or from a bind)
	ClientUserInfoChanged,		//! pfnClientUserInfoChanged()	(wd) Client has updated their setinfo structure
	ServerActivate,				//! pfnServerActivate()			(wd) Server is starting a new map
	ServerDeactivate,			//! pfnServerDeactivate()		(wd) Server is leaving the map (shutdown, or changelevel); SDK2

	PlayerPreThink,				//! pfnPlayerPreThink()
	PlayerPostThink,			//! pfnPlayerPostThink()

	StartFrame,					//! pfnStartFrame()
	ParmsNewLevel,				//! pfnParmsNewLevel()
	ParmsChangeLevel,			//! pfnParmsChangeLevel()

	GetGameDescription,			//! pfnGetGameDescription()		Returns string describing current .dll.  E.g. "TeamFotrress 2", "Half-Life"
	PlayerCustomization,		//! pfnPlayerCustomization()	Notifies .dll of new customization for player.

	SpectatorConnect,			//! pfnSpectatorConnect()		Called when spectator joins server
	SpectatorDisconnect,		//! pfnSpectatorDisconnect()	Called when spectator leaves the server
	SpectatorThink,				//! pfnSpectatorThink()			Called when spectator sends a command packet (usercmd_t)
	
	Sys_Error,					//! pfnSys_Error()				Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.  SDK2

	PM_Move,					//! pfnPM_Move()				(wd) SDK2
	PM_Init,					//! pfnPM_Init()				Server version of player movement initialization; (wd) SDK2
	PM_FindTextureType,			//! pfnPM_FindTextureType()		(wd) SDK2
	
	SetupVisibility,			//! pfnSetupVisibility()		Set up PVS and PAS for networking for this client; (wd) SDK2
	UpdateClientData,			//! pfnUpdateClientData()		Set up data sent only to specific client; (wd) SDK2
	AddToFullPack,				//! pfnAddToFullPack()			(wd) SDK2
	CreateBaseline,				//! pfnCreateBaseline()			Tweak entity baseline for network encoding, allows setup of player baselines, too.; (wd) SDK2
	RegisterEncoders,			//! pfnRegisterEncoders()		Callbacks for network encoding; (wd) SDK2
	GetWeaponData,				//! pfnGetWeaponData()			(wd) SDK2
	CmdStart,					//! pfnCmdStart()				(wd) SDK2
	CmdEnd,						//! pfnCmdEnd()					(wd) SDK2
	ConnectionlessPacket,		//! pfnConnectionlessPacket()	(wd) SDK2
	GetHullBounds,				//! pfnGetHullBounds()			(wd) SDK2
	CreateInstancedBaselines,	//! pfnCreateInstancedBaselines()	(wd) SDK2
	InconsistentFile,			//! pfnInconsistentFile()		(wd) SDK2
	AllowLagCompensation,		//! pfnAllowLagCompensation()	(wd) SDK2
};

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

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, 
		int *interfaceVersion )
{
	LOG_DEVELOPER(PLID, "called: GetEntityAPI2; version=%d", *interfaceVersion);
	if(!pFunctionTable) {
		LOG_ERROR(PLID, "GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		LOG_ERROR(PLID, 
				"GetEntityAPI2 version mismatch; requested=%d ours=%d", 
				*interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is 
		//! out of date.
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

static NEW_DLL_FUNCTIONS gNewFunctionTable = 
{
	OnFreeEntPrivateData,		//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
	GameShutdown,				//! pfnGameShutdown()
	ShouldCollide,				//! pfnShouldCollide()
	// Added 2005-08-11 (no SDK update)
	CvarValue,					//! pfnCvarValue()
	// Added 2005-11-22 (no SDK update)
	CvarValue2,					//! pfnCvarValue2()
};

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, 
		int *interfaceVersion) 
{
	LOG_DEVELOPER(PLID, "called: GetNewDLLFunctions; version=%d", 
			*interfaceVersion);
	if(!pNewFunctionTable) {
		LOG_ERROR(PLID, 
				"GetNewDLLFunctions called with null pNewFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		LOG_ERROR(PLID, 
				"GetNewDLLFunctions version mismatch; requested=%d ours=%d", 
				*interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		//! Tell engine what version we had, so it can figure out who is 
		//! out of date.
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return(FALSE);
	}
	memcpy(pNewFunctionTable, &gNewFunctionTable, sizeof(NEW_DLL_FUNCTIONS));
	return TRUE;
}
