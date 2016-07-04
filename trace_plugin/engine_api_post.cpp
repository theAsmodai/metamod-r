// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engine_api_post.cpp - implementation of "post" versions of Half-Life engine 
//                       functions

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

#include <stdio.h>
#include <stdarg.h>

#include <extdll.h>

#include "engine_api.h"
#include "meta_api.h"
#include "trace_api.h"
#include "log_plugin.h"


int PrecacheModel_Post(char *s) {
	ENGINE_TRACE(pfnPrecacheModel, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PrecacheSound_Post(char *s) {
	ENGINE_TRACE(pfnPrecacheSound, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetModel_Post(edict_t *e, const char *m) {
	ENGINE_TRACE(pfnSetModel, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int ModelIndex_Post(const char *m) {
	ENGINE_TRACE(pfnModelIndex, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int ModelFrames_Post(int modelIndex) {
	ENGINE_TRACE(pfnModelFrames, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void SetSize_Post(edict_t *e, const float *rgflMin, const float *rgflMax) {
	ENGINE_TRACE(pfnSetSize, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangeLevel_Post(char *s1, char *s2) {
	// trace output in Pre
	ENGINE_TRACE(pfnChangeLevel, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void GetSpawnParms_Post(edict_t *ent) {
	ENGINE_TRACE(pfnGetSpawnParms, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void SaveSpawnParms_Post(edict_t *ent) {
	ENGINE_TRACE(pfnSaveSpawnParms, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

float VecToYaw_Post(const float *rgflVector) {
	ENGINE_TRACE(pfnVecToYaw, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void VecToAngles_Post(const float *rgflVectorIn, float *rgflVectorOut) {
	ENGINE_TRACE(pfnVecToAngles, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void MoveToOrigin_Post(edict_t *ent, const float *pflGoal, float dist, int iMoveType) {
	ENGINE_TRACE(pfnMoveToOrigin, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangeYaw_Post(edict_t *ent) {
	ENGINE_TRACE(pfnChangeYaw, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangePitch_Post(edict_t *ent) {
	ENGINE_TRACE(pfnChangePitch, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

edict_t *FindEntityByString_Post(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue) {
	edict_t *ed=META_RESULT_ORIG_RET(edict_t *);
	ENGINE_TRACE(pfnFindEntityByString, P_POST, ("classname=%s netname=%s", 
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int GetEntityIllum_Post(edict_t *pEnt) {
	ENGINE_TRACE(pfnGetEntityIllum, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t *FindEntityInSphere_Post(edict_t *pEdictStartSearchAfter, const float *org, float rad) {
	edict_t *ret;
	ret=META_RESULT_ORIG_RET(edict_t *);
	ENGINE_TRACE(pfnFindEntityInSphere, P_POST, ("previous=%s current=%s", 
				pEdictStartSearchAfter ? STRING(pEdictStartSearchAfter->v.classname) : "nil",
				ret ? STRING(ret->v.classname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *FindClientInPVS_Post(edict_t *pEdict) {
	ENGINE_TRACE(pfnFindClientInPVS, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *EntitiesInPVS_Post(edict_t *pplayer) {
	ENGINE_TRACE(pfnEntitiesInPVS, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeVectors_Post(const float *rgflVector) {
	ENGINE_TRACE(pfnMakeVectors, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void AngleVectors_Post(const float *rgflVector, float *forward, float *right, float *up) {
	ENGINE_TRACE(pfnAngleVectors, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

edict_t *CreateEntity_Post(void) {
	ENGINE_TRACE(pfnCreateEntity, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RemoveEntity_Post(edict_t *e) {
	ENGINE_TRACE(pfnRemoveEntity, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
edict_t *CreateNamedEntity_Post(int className) {
	edict_t *ret;
	ret=META_RESULT_ORIG_RET(edict_t *);
	ENGINE_TRACE(pfnCreateNamedEntity, P_POST, ("created=%s", 
				ret ? STRING(ret->v.classname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeStatic_Post(edict_t *ent) {
	ENGINE_TRACE(pfnMakeStatic, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int EntIsOnFloor_Post(edict_t *e) {
	ENGINE_TRACE(pfnEntIsOnFloor, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DropToFloor_Post(edict_t *e) {
	ENGINE_TRACE(pfnDropToFloor, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int WalkMove_Post(edict_t *ent, float yaw, float dist, int iMode) {
	ENGINE_TRACE(pfnWalkMove, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetOrigin_Post(edict_t *e, const float *rgflOrigin) {
	ENGINE_TRACE(pfnSetOrigin, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void EmitSound_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch) {
	ENGINE_TRACE(pfnEmitSound, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void EmitAmbientSound_Post(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch) {
	ENGINE_TRACE(pfnEmitAmbientSound, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void TraceLine_Post(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceLine, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void TraceToss_Post(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceToss, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int TraceMonsterHull_Post(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceMonsterHull, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void TraceHull_Post(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceHull, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void TraceModel_Post(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceModel, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
const char *TraceTexture_Post(edict_t *pTextureEntity, const float *v1, const float *v2 ) {
	ENGINE_TRACE(pfnTraceTexture, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void TraceSphere_Post(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceSphere, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void GetAimVector_Post(edict_t *ent, float speed, float *rgflReturn) {
	ENGINE_TRACE(pfnGetAimVector, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void ServerCommand_Post(char *str) {
	// trace output in Pre
	ENGINE_TRACE(pfnServerCommand, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void ServerExecute_Post(void) {
	ENGINE_TRACE(pfnServerExecute, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void engClientCommand_Post(edict_t *pEdict, char *szFmt, ...) {
	// trace output in Pre
	ENGINE_TRACE(pfnClientCommand, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void ParticleEffect_Post(const float *org, const float *dir, float color, float count) {
	ENGINE_TRACE(pfnParticleEffect, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void LightStyle_Post(int style, char *val) {
	ENGINE_TRACE(pfnLightStyle, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int DecalIndex_Post(const char *name) {
	ENGINE_TRACE(pfnDecalIndex, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PointContents_Post(const float *rgflVector) {
	ENGINE_TRACE(pfnPointContents, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
	if(msg_type > 64)
		ENGINE_TRACE(pfnMessageBegin, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void MessageEnd_Post(void) {
	ENGINE_TRACE(pfnMessageEnd, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void WriteByte_Post(int iValue) {
	ENGINE_TRACE(pfnWriteByte, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteChar_Post(int iValue) {
	ENGINE_TRACE(pfnWriteChar, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteShort_Post(int iValue) {
	ENGINE_TRACE(pfnWriteShort, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteLong_Post(int iValue) {
	ENGINE_TRACE(pfnWriteLong, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteAngle_Post(float flValue) {
	ENGINE_TRACE(pfnWriteAngle, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteCoord_Post(float flValue) {
	ENGINE_TRACE(pfnWriteCoord, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteString_Post(const char *sz) {
	// trace output in Pre
	ENGINE_TRACE(pfnWriteString, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteEntity_Post(int iValue) {
	ENGINE_TRACE(pfnWriteEntity, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void CVarRegister_Post(cvar_t *pCvar) {
	// trace output in Pre
	ENGINE_TRACE(pfnCVarRegister, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
float CVarGetFloat_Post(const char *szVarName) {
	ENGINE_TRACE(pfnCVarGetFloat, P_POST, ("cvar=%s val=%f",
				szVarName, META_RESULT_ORIG_RET(float)));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
const char *CVarGetString_Post(const char *szVarName) {
	const char *val=META_RESULT_ORIG_RET(const char *);
	ENGINE_TRACE(pfnCVarGetString, P_POST, ("cvar=%s val=%s",
				szVarName,
				val ? val : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void CVarSetFloat_Post(const char *szVarName, float flValue) {
	// trace output in Pre
	ENGINE_TRACE(pfnCVarSetFloat, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void CVarSetString_Post(const char *szVarName, const char *szValue) {
	// trace output in Pre
	ENGINE_TRACE(pfnCVarSetString, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void AlertMessage_Post(ALERT_TYPE atype, char *szFmt, ...) {
	// trace output in Pre
	ENGINE_TRACE(pfnAlertMessage, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
#ifdef HLSDK_3_2_OLD_EIFACE
void EngineFprintf_Post(FILE *pfile, char *szFmt, ...) {
#else 
void EngineFprintf_Post(void *pfile, char *szFmt, ...) {
#endif
	// trace output in Pre
	ENGINE_TRACE(pfnEngineFprintf, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
void *PvAllocEntPrivateData_Post(edict_t *pEdict, long cb) {
#else
void *PvAllocEntPrivateData_Post(edict_t *pEdict, int32 cb) {
#endif
	ENGINE_TRACE(pfnPvAllocEntPrivateData, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void *PvEntPrivateData_Post(edict_t *pEdict) {
	ENGINE_TRACE(pfnPvEntPrivateData, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeEntPrivateData_Post(edict_t *pEdict) {
	ENGINE_TRACE(pfnFreeEntPrivateData, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

const char *SzFromIndex_Post(int iString) {
	ENGINE_TRACE(pfnSzFromIndex, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int AllocString_Post(const char *szValue) {
	ENGINE_TRACE(pfnAllocString, P_POST, ("str=%s alloc=%d",
				szValue, META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

struct entvars_s *GetVarsOfEnt_Post(edict_t *pEdict) {
	ENGINE_TRACE(pfnGetVarsOfEnt, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *PEntityOfEntOffset_Post(int iEntOffset) {
	ENGINE_TRACE(pfnPEntityOfEntOffset, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int EntOffsetOfPEntity_Post(const edict_t *pEdict) {
	ENGINE_TRACE(pfnEntOffsetOfPEntity, P_POST, ("offset=%d", 
				META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int IndexOfEdict_Post(const edict_t *pEdict) {
	ENGINE_TRACE(pfnIndexOfEdict, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t *PEntityOfEntIndex_Post(int iEntIndex) {
	edict_t *ed=META_RESULT_ORIG_RET(edict_t *);
	ENGINE_TRACE(pfnPEntityOfEntIndex, P_POST, ("classname=%s netname=%s", 
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *FindEntityByVars_Post(struct entvars_s *pvars) {
	ENGINE_TRACE(pfnFindEntityByVars, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void *GetModelPtr_Post(edict_t *pEdict) {
	ENGINE_TRACE(pfnGetModelPtr, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int RegUserMsg_Post(const char *pszName, int iSize) {
	int ret;
	ENGINE_TRACE(pfnRegUserMsg, P_POST, ("msg=%s, id=%d, size=%d", 
				pszName, META_RESULT_ORIG_RET(int), iSize));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void AnimationAutomove_Post(const edict_t *pEdict, float flTime) {
	ENGINE_TRACE(pfnAnimationAutomove, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void GetBonePosition_Post(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles ) {
	ENGINE_TRACE(pfnGetBonePosition, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
unsigned long FunctionFromName_Post( const char *pName ) {
#else
uint32 FunctionFromName_Post( const char *pName ) {
#endif
	ENGINE_TRACE(pfnFunctionFromName, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0UL);
}
#ifdef HLSDK_3_2_OLD_EIFACE
const char *NameForFunction_Post( unsigned long function ) {
#else
const char *NameForFunction_Post( uint32 function ) {
#endif
	ENGINE_TRACE(pfnNameForFunction, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

//! JOHN: engine callbacks so game DLL can print messages to individual clients
void ClientPrintf_Post( edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg ) {
	// trace output in Pre
	ENGINE_TRACE(pfnClientPrintf, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void ServerPrint_Post( const char *szMsg ) {
	// trace output in Pre
	ENGINE_TRACE(pfnServerPrint, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

//! these 3 added so game DLL can easily access client 'cmd' strings
const char *Cmd_Args_Post( void ) {
	ENGINE_TRACE(pfnCmd_Args, P_POST, ("args=%s", 
				META_RESULT_ORIG_RET(char *)));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
const char *Cmd_Argv_Post( int argc ) {
	ENGINE_TRACE(pfnCmd_Argv, P_POST, ("arg=%s", 
				META_RESULT_ORIG_RET(char *)));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int Cmd_Argc_Post( void ) {
	ENGINE_TRACE(pfnCmd_Argc, P_POST, ("argc=%d", 
				META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void GetAttachment_Post(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles ) {
	ENGINE_TRACE(pfnGetAttachment, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void CRC32_Init_Post(CRC32_t *pulCRC) {
	ENGINE_TRACE(pfnCRC32_Init, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessBuffer_Post(CRC32_t *pulCRC, void *p, int len) {
	ENGINE_TRACE(pfnCRC32_ProcessBuffer, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessByte_Post(CRC32_t *pulCRC, unsigned char ch) {
	ENGINE_TRACE(pfnCRC32_ProcessByte, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
CRC32_t CRC32_Final_Post(CRC32_t pulCRC) {
	ENGINE_TRACE(pfnCRC32_Final, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

#ifdef HLSDK_3_2_OLD_EIFACE
long RandomLong_Post(long lLow, long lHigh) {
#else
int32 RandomLong_Post(int32 lLow, int32 lHigh) {
#endif
	ENGINE_TRACE(pfnRandomLong, P_POST, ("random=%ld", 
				META_RESULT_ORIG_RET(long)));
	RETURN_META_VALUE(MRES_IGNORED, 0L);
}
float RandomFloat_Post(float flLow, float flHigh) {
	ENGINE_TRACE(pfnRandomFloat, P_POST, ("random=%f", 
				META_RESULT_ORIG_RET(float)));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}

void SetView_Post(const edict_t *pClient, const edict_t *pViewent ) {
	ENGINE_TRACE(pfnSetView, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
float Time_Post( void ) {
	ENGINE_TRACE(pfnTime, P_POST, ("time=%f", 
				META_RESULT_ORIG_RET(float)));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void CrosshairAngle_Post(const edict_t *pClient, float pitch, float yaw) {
	ENGINE_TRACE(pfnCrosshairAngle, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

byte *LoadFileForMe_Post(char *filename, int *pLength) {
	ENGINE_TRACE(pfnLoadFileForMe, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeFile_Post(void *buffer) {
	ENGINE_TRACE(pfnFreeFile, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

//! trigger_endsection
void EndSection_Post(const char *pszSectionName) {
	ENGINE_TRACE(pfnEndSection, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int CompareFileTime_Post(char *filename1, char *filename2, int *iCompare) {
	ENGINE_TRACE(pfnCompareFileTime, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void GetGameDir_Post(char *szGetGameDir) {
	ENGINE_TRACE(pfnGetGameDir, P_POST, ("gamedir=%s", szGetGameDir));
	RETURN_META(MRES_IGNORED);
}
void Cvar_RegisterVariable_Post(cvar_t *variable) {
	// trace output in Pre
	ENGINE_TRACE(pfnCvar_RegisterVariable, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void FadeClientVolume_Post(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) {
	ENGINE_TRACE(pfnFadeClientVolume, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void SetClientMaxspeed_Post(const edict_t *pEdict, float fNewMaxspeed) {
	ENGINE_TRACE(pfnSetClientMaxspeed, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
//! returns NULL if fake client can't be created
edict_t * CreateFakeClient_Post(const char *netname) {
	ENGINE_TRACE(pfnCreateFakeClient, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RunPlayerMove_Post(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec ) {
	ENGINE_TRACE(pfnRunPlayerMove, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int NumberOfEntities_Post(void) {
	ENGINE_TRACE(pfnNumberOfEntities, P_POST, ("num=%d", 
				META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

//! passing in NULL gets the serverinfo
char *GetInfoKeyBuffer_Post(edict_t *e) {
	ENGINE_TRACE(pfnGetInfoKeyBuffer, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
char *InfoKeyValue_Post(char *infobuffer, char *key) {
	ENGINE_TRACE(pfnInfoKeyValue, P_POST, ("value=%s", 
				META_RESULT_ORIG_RET(char *)));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetKeyValue_Post(char *infobuffer, char *key, char *value) {
	ENGINE_TRACE(pfnSetKeyValue, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void SetClientKeyValue_Post(int clientIndex, char *infobuffer, char *key, char *value) {
	ENGINE_TRACE(pfnSetClientKeyValue, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

int IsMapValid_Post(char *filename) {
	ENGINE_TRACE(pfnIsMapValid, P_POST, ("file=%s, val=%d",
				filename, META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void StaticDecal_Post( const float *origin, int decalIndex, int entityIndex, int modelIndex ) {
	ENGINE_TRACE(pfnStaticDecal, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int PrecacheGeneric_Post(char *s) {
	ENGINE_TRACE(pfnPrecacheGeneric, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
//! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
int GetPlayerUserId_Post(edict_t *e ) {
	edict_t *ed = e;
	int userid=META_RESULT_ORIG_RET(int);
	ENGINE_TRACE(pfnGetPlayerUserId, P_POST, ("netname=%s userid=%d",
				ed ? STRING(ed->v.netname) : "nil",
				userid));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void BuildSoundMsg_Post(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) 
{
	ENGINE_TRACE(pfnBuildSoundMsg, P_POST, ("sample=%s", sample));
	RETURN_META(MRES_IGNORED);
}
//! is this a dedicated server?
int IsDedicatedServer_Post(void) {
	ENGINE_TRACE(pfnIsDedicatedServer, P_POST, ("val=%d",
				META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
cvar_t *CVarGetPointer_Post(const char *szVarName) {
	// output in Pre
	ENGINE_TRACE(pfnCVarGetPointer, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
//! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
unsigned int GetPlayerWONId_Post(edict_t *e) {
	edict_t *ed = e;
	unsigned int wonid=META_RESULT_ORIG_RET(unsigned int);
	ENGINE_TRACE(pfnGetPlayerWONId, P_POST, ("netname=%s wonid=%u",
				ed ? STRING(ed->v.netname) : "nil",
				wonid));
	RETURN_META_VALUE(MRES_IGNORED, 0U);
}

//! YWB 8/1/99 TFF Physics additions
void Info_RemoveKey_Post( char *s, const char *key ) {
	ENGINE_TRACE(pfnInfo_RemoveKey, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
const char *GetPhysicsKeyValue_Post( const edict_t *pClient, const char *key ) {
	ENGINE_TRACE(pfnGetPhysicsKeyValue, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetPhysicsKeyValue_Post( const edict_t *pClient, const char *key, const char *value ) {
	ENGINE_TRACE(pfnSetPhysicsKeyValue, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
const char *GetPhysicsInfoString_Post( const edict_t *pClient ) {
	ENGINE_TRACE(pfnGetPhysicsInfoString, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned short PrecacheEvent_Post( int type, const char *psz ) {
	ENGINE_TRACE(pfnPrecacheEvent, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0U);
}
void PlaybackEvent_Post( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 ) 
{
	ENGINE_TRACE(pfnPlaybackEvent, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

unsigned char *SetFatPVS_Post( float *org ) {
	ENGINE_TRACE(pfnSetFatPVS, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned char *SetFatPAS_Post( float *org ) {
	ENGINE_TRACE(pfnSetFatPAS, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int CheckVisibility_Post( const edict_t *entity, unsigned char *pset ) {
	ENGINE_TRACE(pfnCheckVisibility, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DeltaSetField_Post( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaSetField, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void DeltaUnsetField_Post( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaUnsetField, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void DeltaAddEncoder_Post( char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) ) {
	ENGINE_TRACE(pfnDeltaAddEncoder, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
int GetCurrentPlayer_Post( void ) {
	ENGINE_TRACE(pfnGetCurrentPlayer, P_POST, ("val=%d",
				META_RESULT_ORIG_RET(int)));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int CanSkipPlayer_Post( const edict_t *player ) {
	ENGINE_TRACE(pfnCanSkipPlayer, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DeltaFindField_Post( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaFindField, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DeltaSetFieldByIndex_Post( struct delta_s *pFields, int fieldNumber ) {
	ENGINE_TRACE(pfnDeltaSetFieldByIndex, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}
void DeltaUnsetFieldByIndex_Post( struct delta_s *pFields, int fieldNumber ) {
	ENGINE_TRACE(pfnDeltaUnsetFieldByIndex, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void SetGroupMask_Post( int mask, int op ) {
	ENGINE_TRACE(pfnSetGroupMask, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

int engCreateInstancedBaseline_Post( int classname, struct entity_state_s *baseline ) {
	ENGINE_TRACE(pfnCreateInstancedBaseline, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void Cvar_DirectSet_Post( struct cvar_s *var, char *value ) {
	ENGINE_TRACE(pfnCvar_DirectSet, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

//! Forces the client and server to be running with the same version of the specified file
//!( e.g., a player model ).
//! Calling this has no effect in single player
void ForceUnmodified_Post( FORCE_TYPE type, float *mins, float *maxs, const char *filename ) {
	// trace output in Pre
	ENGINE_TRACE(pfnForceUnmodified, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

void GetPlayerStats_Post( const edict_t *pClient, int *ping, int *packet_loss ) {
	const edict_t *ed=pClient;
	ENGINE_TRACE(pfnGetPlayerStats, P_POST, 
			("netname=%s ping=%d packet_loss=%d",
				ed ? STRING(ed->v.netname) : "nil",
				*ping,
				*packet_loss));
	RETURN_META(MRES_IGNORED);
}

void AddServerCommand_Post( char *cmd_name, void (*function) (void) ) {
	// trace output in Pre
	ENGINE_TRACE(pfnAddServerCommand, P_POST, (""));
	RETURN_META(MRES_IGNORED);
}

// Added in SDK 2.2:

//! For voice communications, set which clients hear eachother.
//! NOTE: these functions take player entity indices (starting at 1).
qboolean Voice_GetClientListening_Post(int iReceiver, int iSender) {
	ENGINE_TRACE(pfnVoice_GetClientListening, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, false);
}
qboolean Voice_SetClientListening_Post(int iReceiver, int iSender, qboolean bListen) {
	ENGINE_TRACE(pfnVoice_SetClientListening, P_POST, (""));
	RETURN_META_VALUE(MRES_IGNORED, false);
}

// Added for HL 1109 (no SDK update):

const char *GetPlayerAuthId_Post(edict_t *e) {
	edict_t *ed = e;
	const char *authid=META_RESULT_ORIG_RET(const char *);
	ENGINE_TRACE(pfnGetPlayerAuthId, P_POST, ("netname=%s authid=%s",
				ed ? STRING(ed->v.netname) : "nil",
				authid ? authid : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

// Added 2003/11/10 (no SDK update):

sequenceEntry_s *SequenceGet_Post(const char* fileName, const char* entryName) {
	// trace output in Post
	ENGINE_TRACE(pfnSequenceGet, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

sentenceEntry_s *SequencePickSentence_Post(const char* groupName, int pickMethod, int *picked) {
	// trace output in Post
	ENGINE_TRACE(pfnSequencePickSentence, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int GetFileSize_Post(char *filename) {
	// trace output in Post
	ENGINE_TRACE(pfnGetFileSize, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned int GetApproxWavePlayLen_Post(const char *filepath) {
	// trace output in Post
	ENGINE_TRACE(pfnGetApproxWavePlayLen, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int IsCareerMatch_Post(void) {
	// trace output in Post
	ENGINE_TRACE(pfnIsCareerMatch, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int GetLocalizedStringLength_Post(const char *label) {
	// trace output in Post
	ENGINE_TRACE(pfnGetLocalizedStringLength, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void RegisterTutorMessageShown_Post(int mid) {
	// trace output in Post
	ENGINE_TRACE(pfnRegisterTutorMessageShown, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

int GetTimesTutorMessageShown_Post(int mid) {
	// trace output in Post
	ENGINE_TRACE(pfnGetTimesTutorMessageShown, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ProcessTutorMessageDecayBuffer_Post(int *buffer, int bufferLength) {
	// trace output in Post
	ENGINE_TRACE(pfnProcessTutorMessageDecayBuffer, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void ConstructTutorMessageDecayBuffer_Post(int *buffer, int bufferLength) {
	// trace output in Post
	ENGINE_TRACE(pfnConstructTutorMessageDecayBuffer, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void ResetTutorMessageDecayData_Post(void) {
	// trace output in Post
	ENGINE_TRACE(pfnResetTutorMessageDecayData, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

// Added 2005-08-11 (no SDK update):

void QueryClientCvarValue_Post(const edict_t *pEdict, const char *cvar) {
	// trace output in Post
	ENGINE_TRACE(pfnQueryClientCvarValue, P_POST, ("queried=%s",cvar?cvar:"nil"));
	RETURN_META(MRES_IGNORED);
}

// Added 2005-11-22 (no SDK update):

void QueryClientCvarValue2_Post(const edict_t *pEdict, const char *cvar, int requestID) {
	// trace output in Post
	ENGINE_TRACE(pfnQueryClientCvarValue2, P_POST, ("queried=%s, requestID=%d",cvar?cvar:"nil",requestID));
	RETURN_META(MRES_IGNORED);
}

// Added 2009-06-17 (no SDK update):

int EngCheckParm_Post(const char *pchCmdLineToken, char **pchNextVal) {
	// trace output in Post
	ENGINE_TRACE(pfnEngCheckParm, P_POST, ("token=%s, nextval=%s",pchCmdLineToken?pchCmdLineToken:"nil",pchNextVal?*pchNextVal:"nil"));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

enginefuncs_t meta_engfuncs_post = {
	PrecacheModel_Post,			// pfnPrecacheModel()
	PrecacheSound_Post,			// pfnPrecacheSound()
	SetModel_Post,				// pfnSetModel()
	ModelIndex_Post,			// pfnModelIndex()
	ModelFrames_Post,			// pfnModelFrames()

	SetSize_Post,				// pfnSetSize()
	ChangeLevel_Post,			// pfnChangeLevel()
	GetSpawnParms_Post,			// pfnGetSpawnParms()
	SaveSpawnParms_Post,		// pfnSaveSpawnParms()

	VecToYaw_Post,				// pfnVecToYaw()
	VecToAngles_Post,			// pfnVecToAngles()
	MoveToOrigin_Post,			// pfnMoveToOrigin()
	ChangeYaw_Post,				// pfnChangeYaw()
	ChangePitch_Post,			// pfnChangePitch()

	FindEntityByString_Post,	// pfnFindEntityByString()
	GetEntityIllum_Post,		// pfnGetEntityIllum()
	FindEntityInSphere_Post,	// pfnFindEntityInSphere()
	FindClientInPVS_Post,		// pfnFindClientInPVS()
	EntitiesInPVS_Post,			// pfnEntitiesInPVS()

	MakeVectors_Post,			// pfnMakeVectors()
	AngleVectors_Post,			// pfnAngleVectors()

	CreateEntity_Post,			// pfnCreateEntity()
	RemoveEntity_Post,			// pfnRemoveEntity()
	CreateNamedEntity_Post,		// pfnCreateNamedEntity()

	MakeStatic_Post,			// pfnMakeStatic()
	EntIsOnFloor_Post,			// pfnEntIsOnFloor()
	DropToFloor_Post,			// pfnDropToFloor()

	WalkMove_Post,				// pfnWalkMove()
	SetOrigin_Post,				// pfnSetOrigin()

	EmitSound_Post,				// pfnEmitSound()
	EmitAmbientSound_Post,		// pfnEmitAmbientSound()

	TraceLine_Post,				// pfnTraceLine()
	TraceToss_Post,				// pfnTraceToss()
	TraceMonsterHull_Post,		// pfnTraceMonsterHull()
	TraceHull_Post,				// pfnTraceHull()
	TraceModel_Post,			// pfnTraceModel()
	TraceTexture_Post,			// pfnTraceTexture()
	TraceSphere_Post,			// pfnTraceSphere()
	GetAimVector_Post,			// pfnGetAimVector()

	ServerCommand_Post,			// pfnServerCommand()
	ServerExecute_Post,			// pfnServerExecute()
	engClientCommand_Post,		// pfnClientCommand()	// d'oh, ClientCommand in dllapi too

	ParticleEffect_Post,		// pfnParticleEffect()
	LightStyle_Post,			// pfnLightStyle()
	DecalIndex_Post,			// pfnDecalIndex()
	PointContents_Post,			// pfnPointContents()

	MessageBegin_Post,			// pfnMessageBegin()
	MessageEnd_Post,			// pfnMessageEnd()

	WriteByte_Post,				// pfnWriteByte()
	WriteChar_Post,				// pfnWriteChar()
	WriteShort_Post,			// pfnWriteShort()
	WriteLong_Post,				// pfnWriteLong()
	WriteAngle_Post,			// pfnWriteAngle()
	WriteCoord_Post,			// pfnWriteCoord()
	WriteString_Post,			// pfnWriteString()
	WriteEntity_Post,			// pfnWriteEntity()

	CVarRegister_Post,			// pfnCVarRegister()
	CVarGetFloat_Post,			// pfnCVarGetFloat()
	CVarGetString_Post,			// pfnCVarGetString()
	CVarSetFloat_Post,			// pfnCVarSetFloat()
	CVarSetString_Post,			// pfnCVarSetString()

	AlertMessage_Post,			// pfnAlertMessage()
	EngineFprintf_Post,			// pfnEngineFprintf()

	PvAllocEntPrivateData_Post,	// pfnPvAllocEntPrivateData()
	PvEntPrivateData_Post,		// pfnPvEntPrivateData()
	FreeEntPrivateData_Post,	// pfnFreeEntPrivateData()

	SzFromIndex_Post,			// pfnSzFromIndex()
	AllocString_Post,			// pfnAllocString()

	GetVarsOfEnt_Post, 			// pfnGetVarsOfEnt()
	PEntityOfEntOffset_Post,	// pfnPEntityOfEntOffset()
	EntOffsetOfPEntity_Post,	// pfnEntOffsetOfPEntity()
	IndexOfEdict_Post,			// pfnIndexOfEdict()
	PEntityOfEntIndex_Post,		// pfnPEntityOfEntIndex()
	FindEntityByVars_Post,		// pfnFindEntityByVars()
	GetModelPtr_Post,			// pfnGetModelPtr()

	RegUserMsg_Post,			// pfnRegUserMsg()

	AnimationAutomove_Post,		// pfnAnimationAutomove()
	GetBonePosition_Post,		// pfnGetBonePosition()

	FunctionFromName_Post,		// pfnFunctionFromName()
	NameForFunction_Post,		// pfnNameForFunction()

	ClientPrintf_Post,			// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	ServerPrint_Post,			// pfnServerPrint()

	Cmd_Args_Post,				// pfnCmd_Args()				//! these 3 added 
	Cmd_Argv_Post,				// pfnCmd_Argv()				//! so game DLL can easily 
	Cmd_Argc_Post,				// pfnCmd_Argc()				//! access client 'cmd' strings

	GetAttachment_Post,			// pfnGetAttachment()

	CRC32_Init_Post,			// pfnCRC32_Init()
	CRC32_ProcessBuffer_Post,	// pfnCRC32_ProcessBuffer()
	CRC32_ProcessByte_Post,		// pfnCRC32_ProcessByte()
	CRC32_Final_Post,			// pfnCRC32_Final()

	RandomLong_Post,			// pfnRandomLong()
	RandomFloat_Post,			// pfnRandomFloat()

	SetView_Post,				// pfnSetView()
	Time_Post,					// pfnTime()
	CrosshairAngle_Post,		// pfnCrosshairAngle()

	LoadFileForMe_Post,			// pfnLoadFileForMe()
	FreeFile_Post,				// pfnFreeFile()

	EndSection_Post,			// pfnEndSection()				//! trigger_endsection
	CompareFileTime_Post,		// pfnCompareFileTime()
	GetGameDir_Post,			// pfnGetGameDir()
	Cvar_RegisterVariable_Post,	// pfnCvar_RegisterVariable()
	FadeClientVolume_Post,		// pfnFadeClientVolume()
	SetClientMaxspeed_Post,		// pfnSetClientMaxspeed()
	CreateFakeClient_Post,		// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	RunPlayerMove_Post,			// pfnRunPlayerMove()
	NumberOfEntities_Post,		// pfnNumberOfEntities()

	GetInfoKeyBuffer_Post,		// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	InfoKeyValue_Post,			// pfnInfoKeyValue()
	SetKeyValue_Post,			// pfnSetKeyValue()
	SetClientKeyValue_Post,		// pfnSetClientKeyValue()

	IsMapValid_Post,			// pfnIsMapValid()
	StaticDecal_Post,			// pfnStaticDecal()
	PrecacheGeneric_Post,		// pfnPrecacheGeneric()
	GetPlayerUserId_Post, 		// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	BuildSoundMsg_Post,			// pfnBuildSoundMsg()
	IsDedicatedServer_Post,		// pfnIsDedicatedServer()		//! is this a dedicated server?
	CVarGetPointer_Post,		// pfnCVarGetPointer()
	GetPlayerWONId_Post,		// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	Info_RemoveKey_Post,		// pfnInfo_RemoveKey()
	GetPhysicsKeyValue_Post,	// pfnGetPhysicsKeyValue()
	SetPhysicsKeyValue_Post,	// pfnSetPhysicsKeyValue()
	GetPhysicsInfoString_Post,	// pfnGetPhysicsInfoString()
	PrecacheEvent_Post,			// pfnPrecacheEvent()
	PlaybackEvent_Post,			// pfnPlaybackEvent()

	SetFatPVS_Post,				// pfnSetFatPVS()
	SetFatPAS_Post,				// pfnSetFatPAS()

	CheckVisibility_Post,		// pfnCheckVisibility()

	DeltaSetField_Post,			// pfnDeltaSetField()
	DeltaUnsetField_Post,		// pfnDeltaUnsetField()
	DeltaAddEncoder_Post,		// pfnDeltaAddEncoder()
	GetCurrentPlayer_Post,		// pfnGetCurrentPlayer()
	CanSkipPlayer_Post,			// pfnCanSkipPlayer()
	DeltaFindField_Post,		// pfnDeltaFindField()
	DeltaSetFieldByIndex_Post,	// pfnDeltaSetFieldByIndex()
	DeltaUnsetFieldByIndex_Post,// pfnDeltaUnsetFieldByIndex()

	SetGroupMask_Post,			// pfnSetGroupMask()

	engCreateInstancedBaseline_Post, // pfnCreateInstancedBaseline()		// d'oh, CreateInstancedBaseline in dllapi too
	Cvar_DirectSet_Post,		// pfnCvar_DirectSet()

	ForceUnmodified_Post,		// pfnForceUnmodified()

	GetPlayerStats_Post,		// pfnGetPlayerStats()

	AddServerCommand_Post,		// pfnAddServerCommand()

	// Added in SDK 2.2:
	Voice_GetClientListening_Post,	// pfnVoice_GetClientListening()
	Voice_SetClientListening_Post,	// pfnVoice_SetClientListening()

	// Added for HL 1109 (no SDK update):
	GetPlayerAuthId_Post,		// pfnGetPlayerAuthId()

	// Added 2003/11/10 (no SDK update):
	SequenceGet_Post,					// pfnSequenceGet()
	SequencePickSentence_Post,			// pfnSequencePickSentence()
	GetFileSize_Post,					// pfnGetFileSize()
	GetApproxWavePlayLen_Post,			// pfnGetApproxWavePlayLen()
	IsCareerMatch_Post,					// pfnIsCareerMatch()
	GetLocalizedStringLength_Post,		// pfnGetLocalizedStringLength()
	RegisterTutorMessageShown_Post,		// pfnRegisterTutorMessageShown()
	GetTimesTutorMessageShown_Post,		// pfnGetTimesTutorMessageShown()
	ProcessTutorMessageDecayBuffer_Post,	// pfnProcessTutorMessageDecayBuffer()
	ConstructTutorMessageDecayBuffer_Post,	// pfnConstructTutorMessageDecayBuffer()
	ResetTutorMessageDecayData_Post,		// pfnResetTutorMessageDecayData()
	// Added 2005-08-11 (no SDK update):
	QueryClientCvarValue_Post,			// pfnQueryClientCvarValue()
	// Added 2005-11-22 (no SDK update):
	QueryClientCvarValue2_Post,			// pfnQueryClientCvarValue2()
	// Added 2009-06-17 (no SDK update):
	EngCheckParm_Post,					// pfnEngCheckParm()
};

C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion ) 
{
	LOG_DEVELOPER(PLID, "called: GetEngineFunctions_Post; version=%d", *interfaceVersion);
	if(!pengfuncsFromEngine) {
		LOG_ERROR(PLID, "GetEngineFunctions_Post called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, "GetEngineFunctions_Post version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		// Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs_post, sizeof(enginefuncs_t));
	return TRUE;
}
