// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engine_post.h - prototypes and typedefs for "post" versions of HL engine 
//                 functions

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

#ifndef ENGINE_API_POST_H
#define ENGINE_API_POST_H

extern enginefuncs_t meta_engfuncs_post;

// from SDK engine/eiface.h:
extern int PrecacheModel_Post(char *s);
extern int PrecacheSound_Post(char *s);
extern void SetModel_Post(edict_t *e, const char *m);
extern int ModelIndex_Post(const char *m);
extern int ModelFrames_Post(int modelIndex);

extern void SetSize_Post(edict_t *e, const float *rgflMin, const float *rgflMax);
extern void ChangeLevel_Post(char *s1, char *s2);
extern void GetSpawnParms_Post(edict_t *ent);
extern void SaveSpawnParms_Post(edict_t *ent);

extern float VecToYaw_Post(const float *rgflVector);
extern void VecToAngles_Post(const float *rgflVectorIn, float *rgflVectorOut);
extern void MoveToOrigin_Post(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
extern void ChangeYaw_Post(edict_t *ent);
extern void ChangePitch_Post(edict_t *ent);

extern edict_t *FindEntityByString_Post(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
extern int GetEntityIllum_Post(edict_t *pEnt);
extern edict_t *FindEntityInSphere_Post(edict_t *pEdictStartSearchAfter, const float *org, float rad);
extern edict_t *FindClientInPVS_Post(edict_t *pEdict);
extern edict_t *EntitiesInPVS_Post(edict_t *pplayer);

extern void MakeVectors_Post(const float *rgflVector);
extern void AngleVectors_Post(const float *rgflVector, float *forward, float *right, float *up);

extern edict_t *CreateEntity_Post(void);
extern void RemoveEntity_Post(edict_t *e);
extern edict_t *CreateNamedEntity_Post(int className);

extern void MakeStatic_Post(edict_t *ent);
extern int EntIsOnFloor_Post(edict_t *e);
extern int DropToFloor_Post(edict_t *e);

extern int WalkMove_Post(edict_t *ent, float yaw, float dist, int iMode);
extern void SetOrigin_Post(edict_t *e, const float *rgflOrigin);

extern void EmitSound_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
extern void EmitAmbientSound_Post(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);

extern void TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
extern void TraceToss_Post(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
extern int TraceMonsterHull_Post(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
extern void TraceHull_Post(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
extern void TraceModel_Post(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
extern const char *TraceTexture_Post(edict_t *pTextureEntity, const float *v1, const float *v2 );
extern void TraceSphere_Post(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
extern void GetAimVector_Post(edict_t *ent, float speed, float *rgflReturn);

extern void ServerCommand_Post(char *str);
extern void ServerExecute_Post(void);
extern void ClientCommand_Post(edict_t *pEdict, char *szFmt, ...);

extern void ParticleEffect_Post(const float *org, const float *dir, float color, float count);
extern void LightStyle_Post(int style, char *val);
extern int DecalIndex_Post(const char *name);
extern int PointContents_Post(const float *rgflVector);

extern void MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
extern void MessageEnd_Post(void);

extern void WriteByte_Post(int iValue);
extern void WriteChar_Post(int iValue);
extern void WriteShort_Post(int iValue);
extern void WriteLong_Post(int iValue);
extern void WriteAngle_Post(float flValue);
extern void WriteCoord_Post(float flValue);
extern void WriteString_Post(const char *sz);
extern void WriteEntity_Post(int iValue);

extern void CVarRegister_Post(cvar_t *pCvar);
extern float CVarGetFloat_Post(const char *szVarName);
extern const char* CVarGetString_Post(const char *szVarName);
extern void CVarSetFloat_Post(const char *szVarName, float flValue);
extern void CVarSetString_Post(const char *szVarName, const char *szValue);

extern void AlertMessage_Post(ALERT_TYPE atype, char *szFmt, ...);
#ifdef HLSDK_3_2_OLD_EIFACE
extern void EngineFprintf_Post(FILE *pfile, char *szFmt, ...);
#else
extern void EngineFprintf_Post(void *pfile, char *szFmt, ...);
#endif

#ifdef HLSDK_3_2_OLD_EIFACE
extern void *PvAllocEntPrivateData_Post(edict_t *pEdict, long cb);
#else
extern void *PvAllocEntPrivateData_Post(edict_t *pEdict, int32 cb);
#endif
extern void *PvEntPrivateData_Post(edict_t *pEdict);
extern void FreeEntPrivateData_Post(edict_t *pEdict);

extern const char *SzFromIndex_Post(int iString);
extern int AllocString_Post(const char *szValue);

extern struct entvars_s *GetVarsOfEnt_Post(edict_t *pEdict);
extern edict_t *PEntityOfEntOffset_Post(int iEntOffset);
extern int EntOffsetOfPEntity_Post(const edict_t *pEdict);
extern int IndexOfEdict_Post(const edict_t *pEdict);
extern edict_t *PEntityOfEntIndex_Post(int iEntIndex);
extern edict_t *FindEntityByVars_Post(struct entvars_s *pvars);
extern void *GetModelPtr_Post(edict_t *pEdict);

extern int RegUserMsg_Post(const char *pszName, int iSize);

extern void AnimationAutomove_Post(const edict_t *pEdict, float flTime);
extern void GetBonePosition_Post(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles );

#ifdef HLSDK_3_2_OLD_EIFACE
extern unsigned long FunctionFromName_Post( const char *pName );
extern const char *NameForFunction_Post( unsigned long function );
#else
extern uint32 FunctionFromName_Post( const char *pName );
extern const char *NameForFunction_Post( uint32 function );
#endif

extern void ClientPrintf_Post( edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg ); //! JOHN: engine callbacks so game DLL can print messages to individual clients
extern void ServerPrint_Post( const char *szMsg );

extern const char *Cmd_Args_Post( void ); //! these 3 added 
extern const char *Cmd_Argv_Post( int argc ); //! so game DLL can easily 
extern int Cmd_Argc_Post( void ); //! access client 'cmd' strings

extern void GetAttachment_Post(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );

extern void CRC32_Init_Post(CRC32_t *pulCRC);
extern void CRC32_ProcessBuffer_Post(CRC32_t *pulCRC, void *p, int len);
extern void CRC32_ProcessByte_Post(CRC32_t *pulCRC, unsigned char ch);
extern CRC32_t CRC32_Final_Post(CRC32_t pulCRC);

#ifdef HLSDK_3_2_OLD_EIFACE
extern long RandomLong_Post(long lLow, long lHigh);
#else
extern int32 RandomLong_Post(int32 lLow, int32 lHigh);
#endif
extern float RandomFloat_Post(float flLow, float flHigh);

extern void SetView_Post(const edict_t *pClient, const edict_t *pViewent );
extern float Time_Post( void );
extern void CrosshairAngle_Post(const edict_t *pClient, float pitch, float yaw);

extern byte * LoadFileForMe_Post(char *filename, int *pLength);
extern void FreeFile_Post(void *buffer);

extern void EndSection_Post(const char *pszSectionName); //! trigger_endsection
extern int CompareFileTime_Post(char *filename1, char *filename2, int *iCompare);
extern void GetGameDir_Post(char *szGetGameDir);
extern void Cvar_RegisterVariable_Post(cvar_t *variable);
extern void FadeClientVolume_Post(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
extern void SetClientMaxspeed_Post(const edict_t *pEdict, float fNewMaxspeed);
extern edict_t * CreateFakeClient_Post(const char *netname); //! returns NULL if fake client can't be created
extern void RunPlayerMove_Post(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec );
extern int NumberOfEntities_Post(void);

extern char *GetInfoKeyBuffer_Post(edict_t *e); //! passing in NULL gets the serverinfo
extern char *InfoKeyValue_Post(char *infobuffer, char *key);
extern void SetKeyValue_Post(char *infobuffer, char *key, char *value);
extern void SetClientKeyValue_Post(int clientIndex, char *infobuffer, char *key, char *value);

extern int IsMapValid_Post(char *filename);
extern void StaticDecal_Post( const float *origin, int decalIndex, int entityIndex, int modelIndex );
extern int PrecacheGeneric_Post(char *s);
extern int GetPlayerUserId_Post(edict_t *e ); //! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
extern void BuildSoundMsg_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
extern int IsDedicatedServer_Post(void);//! is this a dedicated server?
extern cvar_t *CVarGetPointer_Post(const char *szVarName);
extern unsigned int GetPlayerWONId_Post(edict_t *e); //! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients

//! YWB 8/1/99 TFF Physics additions
extern void Info_RemoveKey_Post( char *s, const char *key );
extern const char *GetPhysicsKeyValue_Post( const edict_t *pClient, const char *key );
extern void SetPhysicsKeyValue_Post( const edict_t *pClient, const char *key, const char *value );
extern const char *GetPhysicsInfoString_Post( const edict_t *pClient );
extern unsigned short PrecacheEvent_Post( int type, const char *psz );
extern void PlaybackEvent_Post( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );

extern unsigned char *SetFatPVS_Post( float *org );
extern unsigned char *SetFatPAS_Post( float *org );

extern int CheckVisibility_Post( const edict_t *entity, unsigned char *pset );

extern void DeltaSetField_Post( struct delta_s *pFields, const char *fieldname );
extern void DeltaUnsetField_Post( struct delta_s *pFields, const char *fieldname );
extern void DeltaAddEncoder_Post( char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
extern int GetCurrentPlayer_Post( void );
extern int CanSkipPlayer_Post( const edict_t *player );
extern int DeltaFindField_Post( struct delta_s *pFields, const char *fieldname );
extern void DeltaSetFieldByIndex_Post( struct delta_s *pFields, int fieldNumber );
extern void DeltaUnsetFieldByIndex_Post( struct delta_s *pFields, int fieldNumber );

extern void SetGroupMask_Post( int mask, int op );

extern int CreateInstancedBaseline_Post( int classname, struct entity_state_s *baseline );
extern void Cvar_DirectSet_Post( struct cvar_s *var, char *value );

//! Forces the client and server to be running with the same version of the specified file
//!_Post( e.g., a player model ).
//! Calling this has no effect in single player
extern void ForceUnmodified_Post( FORCE_TYPE type, float *mins, float *maxs, const char *filename );

extern void GetPlayerStats_Post( const edict_t *pClient, int *ping, int *packet_loss );

extern void AddServerCommand_Post( char *cmd_name, void (*function) (void) );
// Added in SDK 2.2:
extern qboolean Voice_GetClientListening_Post(int iReceiver, int iSender);
extern qboolean Voice_SetClientListening_Post(int iReceiver, int iSender, qboolean bListen);

// Added for HL 1109 (no SDK update):
extern const char *GetPlayerAuthId(edict_t *e);

// Added 2003/11/10 (no SDK update):
extern sequenceEntry_s *SequenceGet(const char* fileName, const char* entryName);
extern sentenceEntry_s *SequencePickSentence(const char* groupName, int pickMethod, int *picked);
extern int GetFileSize(char *filename);
extern unsigned int GetApproxWavePlayLen(const char *filepath);
extern int IsCareerMatch(void);
extern int GetLocalizedStringLength(const char *label);
extern void RegisterTutorMessageShown(int mid);
extern int GetTimesTutorMessageShown(int mid);
extern void ProcessTutorMessageDecayBuffer(int *buffer, int bufferLength);
extern void ConstructTutorMessageDecayBuffer(int *buffer, int bufferLength);
extern void ResetTutorMessageDecayData(void);


// Added 2005-08-11 (no SDK update):
extern void QueryClientCvarValue_Post(const edict_t *pEdict, const char *cvar);

// Added 2005-11-22 (no SDK update):
extern void QueryClientCvarValue2_Post(const edict_t *pEdict, const char *cvar, int requestID);

// Added 2009-06-17 (no SDK update):
extern int EngCheckParm_Post(const char *pchCmdLineToken, char **pchNextVal);

#endif /* ENGINE_API_POST_H */
