// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dllapi_post.h - prototypes and typedefs for "post" versions of HL DLL 
//                 routines

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

#ifndef DLLAPI_POST_H
#define DLLAPI_POST_H

#include <sdk_util.h>		// BOOL
#include <osdep.h>

// from SDK dlls/cbase.h:
extern int DispatchSpawn_Post( edict_t *pent );
extern void DispatchThink_Post( edict_t *pent );
extern void DispatchUse_Post( edict_t *pentUsed, edict_t *pentOther );
extern void DispatchTouch_Post( edict_t *pentTouched, edict_t *pentOther );
extern void DispatchBlocked_Post( edict_t *pentBlocked, edict_t *pentOther );
extern void DispatchKeyValue_Post( edict_t *pentKeyvalue, KeyValueData *pkvd );
extern void DispatchSave_Post( edict_t *pent, SAVERESTOREDATA *pSaveData );
extern int  DispatchRestore_Post( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
extern void DispatchObjectCollsionBox_Post( edict_t *pent );
extern void SaveWriteFields_Post( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveReadFields_Post( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveGlobalState_Post( SAVERESTOREDATA *pSaveData );
extern void RestoreGlobalState_Post( SAVERESTOREDATA *pSaveData );
extern void ResetGlobalState_Post( void );

// from SDK dlls/client.h:
extern BOOL ClientConnect_Post( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );
extern void ClientDisconnect_Post( edict_t *pEntity );
extern void ClientKill_Post( edict_t *pEntity );
extern void ClientPutInServer_Post( edict_t *pEntity );
extern void ClientCommand_Post( edict_t *pEntity );
extern void ClientUserInfoChanged_Post( edict_t *pEntity, char *infobuffer );
extern void ServerActivate_Post( edict_t *pEdictList, int edictCount, int clientMax );
extern void ServerDeactivate_Post( void );
extern void PlayerPreThink_Post( edict_t *pEntity );
extern void PlayerPostThink_Post( edict_t *pEntity );
extern void StartFrame_Post( void );
extern void ParmsNewLevel_Post( void );
extern void ParmsChangeLevel_Post( void );
extern const char *GetGameDescription_Post( void );
extern void PlayerCustomization_Post( edict_t *pEntity, customization_t *pCust );
extern void SpectatorConnect_Post( edict_t *pEntity );
extern void SpectatorDisconnect_Post( edict_t *pEntity );
extern void SpectatorThink_Post( edict_t *pEntity );
extern void Sys_Error_Post( const char *error_string );

// from SDK pm_shared/pm_shared.h:
extern void PM_Move_Post( struct playermove_s *ppmove, int server );
extern void PM_Init_Post( struct playermove_s *ppmove );
extern char PM_FindTextureType_Post( char *name );

// from SDK dlls/client.h:
extern void SetupVisibility_Post( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas );
extern void UpdateClientData_Post( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );
extern int AddToFullPack_Post( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
extern void CreateBaseline_Post( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs );
extern void RegisterEncoders_Post( void );
extern int GetWeaponData_Post( struct edict_s *player, struct weapon_data_s *info );
extern void CmdStart_Post( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed );
extern void CmdEnd_Post( const edict_t *player );
extern int ConnectionlessPacket_Post( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
extern int GetHullBounds_Post( int hullnumber, float *mins, float *maxs );
extern void CreateInstancedBaselines_Post( void );
extern int InconsistentFile_Post( const edict_t *player, const char *filename, char *disconnect_message );
extern int AllowLagCompensation_Post( void );

// no example from SDK...
extern void OnFreeEntPrivateData_Post(edict_t pEnt);
extern void GameShutdown_Post(void);
extern int ShouldCollide_Post(edict_t *pentTouched, edict_t *pentOther);

// Added 2005-08-11 (no SDK update)
void CvarValue_Post(const edict_t *pEdict, const char *szValue);

// Added 2005-11-22 (no SDK update)
void CvarValue2_Post(const edict_t *pEdict, int requestID, const char *cvarName, const char *value);


#endif /* DLLAPI_H */
