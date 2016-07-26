// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dllapi.h - prototypes and typedefs for Half-Life DLL API routines

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

#ifndef DLLAPI_H
#define DLLAPI_H

#include "sdk_util.h"	// BOOL
#include "osdep.h"		// DLLEXPORT, etc

// Typedefs for these are provided in SDK engine/eiface.h, but I didn't
// like the names (APIFUNCTION, APIFUNCTION2, NEW_DLL_FUNCTIONS_FN).
typedef int (*GETENTITYAPI_FN) (DLL_FUNCTIONS *pFunctionTable, int interfaceVersion);
typedef int (*GETENTITYAPI2_FN) (DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);
typedef int (*GETNEWDLLFUNCTIONS_FN) (NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);

// From SDK dlls/cbase.h:
C_DLLEXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion );
C_DLLEXPORT int GetEntityAPI2( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion );

// No example in SDK..
// From Adminmod dll.cpp:
C_DLLEXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion );


// From SDK dlls/game.h:
extern void mm_GameDLLInit( void );

// From SDK dlls/cbase.h:
extern int mm_DispatchSpawn( edict_t *pent );
extern void mm_DispatchThink( edict_t *pent );
extern void mm_DispatchUse( edict_t *pentUsed, edict_t *pentOther );
extern void mm_DispatchTouch( edict_t *pentTouched, edict_t *pentOther );
extern void mm_DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther );
extern void mm_DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd );
extern void mm_DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData );
extern int  mm_DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
extern void mm_DispatchObjectCollisionBox( edict_t *pent );
extern void mm_SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void mm_SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void mm_SaveGlobalState( SAVERESTOREDATA *pSaveData );
extern void mm_RestoreGlobalState( SAVERESTOREDATA *pSaveData );
extern void mm_ResetGlobalState( void );

// From SDK dlls/client.h:
extern BOOL mm_ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );
extern void mm_ClientDisconnect( edict_t *pEntity );
extern void mm_ClientKill( edict_t *pEntity );
extern void mm_ClientPutInServer( edict_t *pEntity );
extern void mm_ClientCommand( edict_t *pEntity );
extern void mm_ClientUserInfoChanged( edict_t *pEntity, char *infobuffer );
extern void mm_ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
extern void mm_ServerDeactivate( void );
extern void mm_PlayerPreThink( edict_t *pEntity );
extern void mm_PlayerPostThink( edict_t *pEntity );
extern void mm_StartFrame( void );
extern void mm_ParmsNewLevel( void );
extern void mm_ParmsChangeLevel( void );
extern const char *mm_GetGameDescription( void );
extern void mm_PlayerCustomization( edict_t *pEntity, customization_t *pCust );
extern void mm_SpectatorConnect ( edict_t *pEntity );
extern void mm_SpectatorDisconnect ( edict_t *pEntity );
extern void mm_SpectatorThink ( edict_t *pEntity );
extern void mm_Sys_Error( const char *error_string );

// From SDK pm_shared/pm_shared.h:
extern void mm_PM_Move ( struct playermove_s *ppmove, int server );
extern void mm_PM_Init ( struct playermove_s *ppmove );
extern char mm_PM_FindTextureType ( const char *name );

// From SDK dlls/client.h:
extern void mm_SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas );
extern void mm_UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );
extern int mm_AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
extern void mm_CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs );
extern void mm_RegisterEncoders( void );
extern int mm_GetWeaponData( struct edict_s *player, struct weapon_data_s *info );
extern void mm_CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed );
extern void mm_CmdEnd ( const edict_t *player );
extern int mm_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
extern int mm_GetHullBounds( int hullnumber, float *mins, float *maxs );
extern void mm_CreateInstancedBaselines ( void );
extern int mm_InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message );
extern int mm_AllowLagCompensation( void );

// No example from SDK...
extern void mm_OnFreeEntPrivateData(edict_t pEnt);
extern void mm_GameShutdown(void);
extern int mm_ShouldCollide(edict_t *pentTouched, edict_t *pentOther);
//Added 2005-08-11 (no SDK update)
extern void mm_CvarValue(const edict_t *pEnt, const char *value);   //! Obsolete! Use mm_CvarValue2 instead
//Added 2005-11-22 (no SDK update)
extern void mm_CvarValue2(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);


// Typedefs for the above functions:

typedef void (*FN_GAMEINIT) ( void );
typedef int (*FN_DISPATCHSPAWN) ( edict_t *pent );
typedef void (*FN_DISPATCHTHINK) ( edict_t *pent );
typedef void (*FN_DISPATCHUSE) ( edict_t *pentUsed, edict_t *pentOther );
typedef void (*FN_DISPATCHTOUCH) ( edict_t *pentTouched, edict_t *pentOther );
typedef void (*FN_DISPATCHBLOCKED) ( edict_t *pentBlocked, edict_t *pentOther );
typedef void (*FN_DISPATCHKEYVALUE) ( edict_t *pentKeyvalue, KeyValueData *pkvd );
typedef void (*FN_DISPATCHSAVE) ( edict_t *pent, SAVERESTOREDATA *pSaveData );
typedef int (*FN_DISPATCHRESTORE) ( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
typedef void (*FN_DISPATCHOBJECTCOLLISIONBOX) ( edict_t *pent );
typedef void (*FN_SAVEWRITEFIELDS) ( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
typedef void (*FN_SAVEREADFIELDS) ( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
typedef void (*FN_SAVEGLOBALSTATE) ( SAVERESTOREDATA *pSaveData );
typedef void (*FN_RESTOREGLOBALSTATE) ( SAVERESTOREDATA *pSaveData );
typedef void (*FN_RESETGLOBALSTATE) ( void );

typedef BOOL (*FN_CLIENTCONNECT) ( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );
typedef void (*FN_CLIENTDISCONNECT) ( edict_t *pEntity );
typedef void (*FN_CLIENTKILL) ( edict_t *pEntity );
typedef void (*FN_CLIENTPUTINSERVER) ( edict_t *pEntity );
typedef void (*FN_CLIENTCOMMAND) ( edict_t *pEntity );
typedef void (*FN_CLIENTUSERINFOCHANGED) ( edict_t *pEntity, char *infobuffer );
typedef void (*FN_SERVERACTIVATE) ( edict_t *pEdictList, int edictCount, int clientMax );
typedef void (*FN_SERVERDEACTIVATE) ( void );
typedef void (*FN_PLAYERPRETHINK) ( edict_t *pEntity );
typedef void (*FN_PLAYERPOSTTHINK) ( edict_t *pEntity );
typedef void (*FN_STARTFRAME) ( void );
typedef void (*FN_PARMSNEWLEVEL) ( void );
typedef void (*FN_PARMSCHANGELEVEL) ( void );
typedef const char *(*FN_GETGAMEDESCRIPTION) ( void );
typedef void (*FN_PLAYERCUSTOMIZATION) ( edict_t *pEntity, customization_t *pCust );
typedef void (*FN_SPECTATORCONNECT)  ( edict_t *pEntity );
typedef void (*FN_SPECTATORDISCONNECT)  ( edict_t *pEntity );
typedef void (*FN_SPECTATORTHINK)  ( edict_t *pEntity );
typedef void (*FN_SYS_ERROR) ( const char *error_string );

typedef void (*FN_PM_MOVE) ( struct playermove_s *ppmove, int server );
typedef void (*FN_PM_INIT) ( struct playermove_s *ppmove );
typedef char (*FN_PM_FINDTEXTURETYPE) ( char *name );

typedef void (*FN_SETUPVISIBILITY) ( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas );
typedef void (*FN_UPDATECLIENTDATA)  ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );
typedef int (*FN_ADDTOFULLPACK) ( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
typedef void (*FN_CREATEBASELINE) ( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs );
typedef void (*FN_REGISTERENCODERS) ( void );
typedef int (*FN_GETWEAPONDATA) ( struct edict_s *player, struct weapon_data_s *info );
typedef void (*FN_CMDSTART) ( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed );
typedef void (*FN_CMDEND)  ( const edict_t *player );
typedef int (*FN_CONNECTIONLESSPACKET) ( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
typedef int (*FN_GETHULLBOUNDS) ( int hullnumber, float *mins, float *maxs );
typedef void (*FN_CREATEINSTANCEDBASELINES)  ( void );
typedef int (*FN_INCONSISTENTFILE) ( const edict_t *player, const char *filename, char *disconnect_message );
typedef int (*FN_ALLOWLAGCOMPENSATION) ( void );

typedef void (*FN_ONFREEENTPRIVATEDATA) (edict_t *pEnt);
typedef void (*FN_GAMESHUTDOWN) (void);
typedef int (*FN_SHOULDCOLLIDE) (edict_t *pentTouched, edict_t *pentOther);
//Added 2005-08-11 (no SDK update)
typedef void (*FN_CVARVALUE)(const edict_t *pEnt, const char *value);   //! Obsolete! Use FN_CVARVALUE2 instead
//Added 2005-11-22 (no SDK update)
typedef void (*FN_CVARVALUE2)(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);

#endif /* DLLAPI_H */
