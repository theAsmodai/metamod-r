// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engine_api.cpp - implementation of Half-Life engine functions

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
#include "support_meta.h"		// MAX_STRBUF_LEN
#include "log_plugin.h"

int PrecacheModel(char *s) {
	ENGINE_TRACE(pfnPrecacheModel, P_PRE, ("model=%s", s));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PrecacheSound(char *s) {
	ENGINE_TRACE(pfnPrecacheSound, P_PRE, ("sound=%s", s));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetModel(edict_t *e, const char *m) {
	edict_t *ed = e;
	ENGINE_TRACE(pfnSetModel, P_PRE, ("classname=%s netname=%s model=%s", 
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil",
				m));
	RETURN_META(MRES_IGNORED);
}
int ModelIndex(const char *m) {
	ENGINE_TRACE(pfnModelIndex, P_PRE, ("model=%s", m));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int ModelFrames(int modelIndex) {
	ENGINE_TRACE(pfnModelFrames, P_PRE, ("index=%d", modelIndex));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void SetSize(edict_t *e, const float *rgflMin, const float *rgflMax) {
	ENGINE_TRACE(pfnSetSize, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangeLevel(char *s1, char *s2) {
	ENGINE_TRACE(pfnChangeLevel, P_PRE, ("s1=%s, s2=%s", s1, s2));
	RETURN_META(MRES_IGNORED);
}
void GetSpawnParms(edict_t *ent) {
	ENGINE_TRACE(pfnGetSpawnParms, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void SaveSpawnParms(edict_t *ent) {
	ENGINE_TRACE(pfnSaveSpawnParms, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

float VecToYaw(const float *rgflVector) {
	ENGINE_TRACE(pfnVecToYaw, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void VecToAngles(const float *rgflVectorIn, float *rgflVectorOut) {
	ENGINE_TRACE(pfnVecToAngles, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void MoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType) {
	ENGINE_TRACE(pfnMoveToOrigin, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangeYaw(edict_t *ent) {
	ENGINE_TRACE(pfnChangeYaw, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void ChangePitch(edict_t *ent) {
	ENGINE_TRACE(pfnChangePitch, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

edict_t *FindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue) {
	edict_t *ed=pEdictStartSearchAfter;
	ENGINE_TRACE(pfnFindEntityByString, P_PRE, ("start=%s field=%s value=%s", 
				ed ? STRING(ed->v.classname) : "nil", pszField, pszValue));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int GetEntityIllum(edict_t *pEnt) {
	ENGINE_TRACE(pfnGetEntityIllum, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t *FindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad) {
	ENGINE_TRACE(pfnFindEntityInSphere, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *FindClientInPVS(edict_t *pEdict) {
	ENGINE_TRACE(pfnFindClientInPVS, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *EntitiesInPVS(edict_t *pplayer) {
	ENGINE_TRACE(pfnEntitiesInPVS, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeVectors(const float *rgflVector) {
	ENGINE_TRACE(pfnMakeVectors, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void AngleVectors(const float *rgflVector, float *forward, float *right, float *up) {
	ENGINE_TRACE(pfnAngleVectors, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

edict_t *CreateEntity(void) {
	ENGINE_TRACE(pfnCreateEntity, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RemoveEntity(edict_t *e) {
	ENGINE_TRACE(pfnRemoveEntity, P_PRE, ("name=%s", STRING(e->v.classname)));
	RETURN_META(MRES_IGNORED);
}
edict_t *CreateNamedEntity(int className) {
	ENGINE_TRACE(pfnCreateNamedEntity, P_PRE, ("name=%s", STRING(className)));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeStatic(edict_t *ent) {
	ENGINE_TRACE(pfnMakeStatic, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int EntIsOnFloor(edict_t *e) {
	ENGINE_TRACE(pfnEntIsOnFloor, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DropToFloor(edict_t *e) {
	ENGINE_TRACE(pfnDropToFloor, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int WalkMove(edict_t *ent, float yaw, float dist, int iMode) {
	ENGINE_TRACE(pfnWalkMove, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetOrigin(edict_t *e, const float *rgflOrigin) {
	edict_t *ed = e;
	ENGINE_TRACE(pfnSetOrigin, P_PRE, ("classname=%s netname=%s",
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META(MRES_IGNORED);
}

void EmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch) {
	ENGINE_TRACE(pfnEmitSound, P_PRE, ("sample=%s", sample));
	RETURN_META(MRES_IGNORED);
}
void EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch) {
	ENGINE_TRACE(pfnEmitAmbientSound, P_PRE, ("sample=%s", samp));
	RETURN_META(MRES_IGNORED);
}

void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceLine, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void TraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceToss, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int TraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceMonsterHull, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void TraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceHull, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void TraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceModel, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
const char *TraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 ) {
	ENGINE_TRACE(pfnTraceTexture, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void TraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr) {
	ENGINE_TRACE(pfnTraceSphere, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void GetAimVector(edict_t *ent, float speed, float *rgflReturn) {
	ENGINE_TRACE(pfnGetAimVector, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void ServerCommand(char *str) {
	ENGINE_TRACE(pfnServerCommand, P_PRE, ("cmd=%s", str));
	RETURN_META(MRES_IGNORED);
}
void ServerExecute(void) {
	ENGINE_TRACE(pfnServerExecute, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void engClientCommand(edict_t *pEdict, char *szFmt, ...) {
	va_list ap;
	char buf[1024];
	char *cp;
	va_start(ap, szFmt);
	vsnprintf(buf, sizeof(buf), szFmt, ap);
	va_end(ap);
	cp=buf+strlen(buf)-1;
	if(*cp=='\n') *cp='\0';
	ENGINE_TRACE(pfnClientCommand, P_PRE, ("cmd='%s'", buf));
	RETURN_META(MRES_IGNORED);
}

void ParticleEffect(const float *org, const float *dir, float color, float count) {
	ENGINE_TRACE(pfnParticleEffect, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void LightStyle(int style, char *val) {
	ENGINE_TRACE(pfnLightStyle, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int DecalIndex(const char *name) {
	ENGINE_TRACE(pfnDecalIndex, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PointContents(const float *rgflVector) {
	ENGINE_TRACE(pfnPointContents, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {
	const char *name, *dest;
	name=GET_USER_MSG_NAME(PLID, msg_type, NULL);
	if(!name) name="unknown";
	dest=msg_dest_types[msg_dest];
	if(!dest) dest="unknown";
	ENGINE_TRACE(pfnMessageBegin, P_PRE, ("type=%s(%d), dest=%s(%d), classname=%s netname=%s", name, msg_type, dest, msg_dest,
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META(MRES_IGNORED);
}
void MessageEnd(void) {
	ENGINE_TRACE(pfnMessageEnd, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void WriteByte(int iValue) {
	ENGINE_TRACE(pfnWriteByte, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteChar(int iValue) {
	ENGINE_TRACE(pfnWriteChar, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteShort(int iValue) {
	ENGINE_TRACE(pfnWriteShort, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteLong(int iValue) {
	ENGINE_TRACE(pfnWriteLong, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteAngle(float flValue) {
	ENGINE_TRACE(pfnWriteAngle, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteCoord(float flValue) {
	ENGINE_TRACE(pfnWriteCoord, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void WriteString(const char *sz) {
	ENGINE_TRACE(pfnWriteString, P_PRE, ("string=%s", sz));
	RETURN_META(MRES_IGNORED);
}
void WriteEntity(int iValue) {
	ENGINE_TRACE(pfnWriteEntity, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void CVarRegister(cvar_t *pCvar) {
	ENGINE_TRACE(pfnCVarRegister, P_PRE, ("cvar=%s", pCvar->name));
	RETURN_META(MRES_IGNORED);
}
float CVarGetFloat(const char *szVarName) {
	// more trace output in Post
	ENGINE_TRACE(pfnCVarGetFloat, P_PRE, ("cvar=%s", szVarName));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
const char *CVarGetString(const char *szVarName) {
	// more trace output in Post
	ENGINE_TRACE(pfnCVarGetString, P_PRE, ("cvar=%s", szVarName));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void CVarSetFloat(const char *szVarName, float flValue) {
	ENGINE_TRACE(pfnCVarSetFloat, P_PRE, ("cvar=%s, val=%f", szVarName, flValue));
	RETURN_META(MRES_IGNORED);
}
void CVarSetString(const char *szVarName, const char *szValue) {
	ENGINE_TRACE(pfnCVarSetString, P_PRE, ("cvar=%s, val=%s", szVarName, szValue));
	RETURN_META(MRES_IGNORED);
}

void AlertMessage(ALERT_TYPE atype, char *szFmt, ...) {
	char *astr;
	va_list ap;
	char buf[MAX_STRBUF_LEN];
	switch(atype) {
		case at_notice:
			astr="at_notice";
			break;
		case at_console:
			astr="at_console";
			break;
		case at_aiconsole:
			astr="at_aiconsole";
			break;
		case at_warning:
			astr="at_warning";
			break;
		case at_error:
			astr="at_error";
			break;
		case at_logged:
			astr="at_logged";
			break;
		default:
			astr="unknown";
			break;
	}
	va_start(ap, szFmt);
	vsnprintf(buf, sizeof(buf), szFmt, ap);
	va_end(ap);
	ENGINE_TRACE(pfnAlertMessage, P_PRE, ("atype=%s(%d) msg=%s", astr, 
				atype, buf));
	RETURN_META(MRES_IGNORED);
}
#ifdef HLSDK_3_2_OLD_EIFACE
void EngineFprintf(FILE *pfile, char *szFmt, ...) {
#else
void EngineFprintf(void *pfile, char *szFmt, ...) {
#endif
	va_list ap;
	char buf[1024];
	va_start(ap, szFmt);
	vsnprintf(buf, sizeof(buf), szFmt, ap);
	va_end(ap);
	ENGINE_TRACE(pfnEngineFprintf, P_PRE, ("line=%s", buf));
	RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
void *PvAllocEntPrivateData(edict_t *pEdict, long cb) {
#else
void *PvAllocEntPrivateData(edict_t *pEdict, int32 cb) {
#endif
	ENGINE_TRACE(pfnPvAllocEntPrivateData, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void *PvEntPrivateData(edict_t *pEdict) {
	ENGINE_TRACE(pfnPvEntPrivateData, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeEntPrivateData(edict_t *pEdict) {
	ENGINE_TRACE(pfnFreeEntPrivateData, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

const char *SzFromIndex(int iString) {
	ENGINE_TRACE(pfnSzFromIndex, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int AllocString(const char *szValue) {
	// more trace output in Post
	ENGINE_TRACE(pfnAllocString, P_PRE, ("str=%s", szValue));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

struct entvars_s *GetVarsOfEnt(edict_t *pEdict) {
	ENGINE_TRACE(pfnGetVarsOfEnt, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *PEntityOfEntOffset(int iEntOffset) {
	ENGINE_TRACE(pfnPEntityOfEntOffset, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int EntOffsetOfPEntity(const edict_t *pEdict) {
	const edict_t *ed=pEdict;
	ENGINE_TRACE(pfnEntOffsetOfPEntity, P_PRE, ("classname=%s netname=%s",
				ed ? STRING(ed->v.classname) : "nil",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int IndexOfEdict(const edict_t *pEdict) {
	ENGINE_TRACE(pfnIndexOfEdict, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t *PEntityOfEntIndex(int iEntIndex) {
	ENGINE_TRACE(pfnPEntityOfEntIndex, P_PRE, ("index=%d", iEntIndex));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t *FindEntityByVars(struct entvars_s *pvars) {
	ENGINE_TRACE(pfnFindEntityByVars, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void *GetModelPtr(edict_t *pEdict) {
	ENGINE_TRACE(pfnGetModelPtr, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int RegUserMsg(const char *pszName, int iSize) {
	// more trace output in Post
	ENGINE_TRACE(pfnRegUserMsg, P_PRE, ("msg=%s size=%d", pszName, iSize));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void AnimationAutomove(const edict_t *pEdict, float flTime) {
	ENGINE_TRACE(pfnAnimationAutomove, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void GetBonePosition(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles ) {
	ENGINE_TRACE(pfnGetBonePosition, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
unsigned long FunctionFromName( const char *pName ) {
#else
uint32 FunctionFromName( const char *pName ) {
#endif
	ENGINE_TRACE(pfnFunctionFromName, P_PRE, ("name=%s", pName));
	RETURN_META_VALUE(MRES_IGNORED, 0UL);
}
#ifdef HLSDK_3_2_OLD_EIFACE
const char *NameForFunction( unsigned long function ) {
#else
const char *NameForFunction( uint32 function ) {
#endif
	ENGINE_TRACE(pfnNameForFunction, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

//! JOHN: engine callbacks so game DLL can print messages to individual clients
void ClientPrintf( edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg ) {
	ENGINE_TRACE(pfnClientPrintf, P_PRE, ("msg=%s", szMsg));
	RETURN_META(MRES_IGNORED);
}
void ServerPrint( const char *szMsg ) {
	ENGINE_TRACE(pfnServerPrint, P_PRE, ("msg=%s", szMsg));
	RETURN_META(MRES_IGNORED);
}

//! these 3 added so game DLL can easily access client 'cmd' strings
const char *Cmd_Args( void ) {
	// trace output in Post
	ENGINE_TRACE(pfnCmd_Args, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
const char *Cmd_Argv( int argc ) {
	// more trace output in Post
	ENGINE_TRACE(pfnCmd_Argv, P_PRE, ("argc=%d", argc));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int Cmd_Argc( void ) {
	// trace output in Post
	ENGINE_TRACE(pfnCmd_Argc, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void GetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles ) {
	ENGINE_TRACE(pfnGetAttachment, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void CRC32_Init(CRC32_t *pulCRC) {
	ENGINE_TRACE(pfnCRC32_Init, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len) {
	ENGINE_TRACE(pfnCRC32_ProcessBuffer, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch) {
	ENGINE_TRACE(pfnCRC32_ProcessByte, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
CRC32_t CRC32_Final(CRC32_t pulCRC) {
	ENGINE_TRACE(pfnCRC32_Final, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

#ifdef HLSDK_3_2_OLD_EIFACE
long RandomLong(long lLow, long lHigh) {
#else
int32 RandomLong(int32 lLow, int32 lHigh) {
#endif
	// more output in Post
	ENGINE_TRACE(pfnRandomLong, P_PRE, ("low=%ld, high=%ld", lLow, lHigh));
	RETURN_META_VALUE(MRES_IGNORED, 0L);
}
float RandomFloat(float flLow, float flHigh) {
	// more output in Post
	ENGINE_TRACE(pfnRandomFloat, P_PRE, ("low=%f, high=%f", flLow, flHigh));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}

void SetView(const edict_t *pClient, const edict_t *pViewent ) {
	ENGINE_TRACE(pfnSetView, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
float Time( void ) {
	// trace output in Post
	ENGINE_TRACE(pfnTime, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void CrosshairAngle(const edict_t *pClient, float pitch, float yaw) {
	ENGINE_TRACE(pfnCrosshairAngle, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

byte *LoadFileForMe(char *filename, int *pLength) {
	ENGINE_TRACE(pfnLoadFileForMe, P_PRE, ("file=%s", filename));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeFile(void *buffer) {
	ENGINE_TRACE(pfnFreeFile, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

//! trigger_endsection
void EndSection(const char *pszSectionName) {
	ENGINE_TRACE(pfnEndSection, P_PRE, ("section=%s", pszSectionName));
	RETURN_META(MRES_IGNORED);
}
int CompareFileTime(char *filename1, char *filename2, int *iCompare) {
	ENGINE_TRACE(pfnCompareFileTime, P_PRE, ("file1=%s, file2=%s", filename1, filename2));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void GetGameDir(char *szGetGameDir) {
	// trace output in Post
	ENGINE_TRACE(pfnGetGameDir, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void Cvar_RegisterVariable(cvar_t *variable) {
	ENGINE_TRACE(pfnCvar_RegisterVariable, P_PRE, ("cvar=%s", variable->name));
	RETURN_META(MRES_IGNORED);
}
void FadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) {
	ENGINE_TRACE(pfnFadeClientVolume, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void SetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed) {
	ENGINE_TRACE(pfnSetClientMaxspeed, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
//! returns NULL if fake client can't be created
edict_t * CreateFakeClient(const char *netname) {
	ENGINE_TRACE(pfnCreateFakeClient, P_PRE, ("name=%s", netname));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec ) {
	ENGINE_TRACE(pfnRunPlayerMove, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int NumberOfEntities(void) {
	// trace output in Post
	ENGINE_TRACE(pfnNumberOfEntities, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

//! passing in NULL gets the serverinfo
char *GetInfoKeyBuffer(edict_t *e) {
	ENGINE_TRACE(pfnGetInfoKeyBuffer, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
char *InfoKeyValue(char *infobuffer, char *key) {
	ENGINE_TRACE(pfnInfoKeyValue, P_PRE, ("key=%s", key));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetKeyValue(char *infobuffer, char *key, char *value) {
	ENGINE_TRACE(pfnSetKeyValue, P_PRE, ("key=%s, value=%s", key, value));
	RETURN_META(MRES_IGNORED);
}
void SetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value) {
	ENGINE_TRACE(pfnSetClientKeyValue, P_PRE, ("index=%d, key=%s, value=%s", clientIndex, key, value));
	RETURN_META(MRES_IGNORED);
}

int IsMapValid(char *filename) {
	// more trace output in Post
	ENGINE_TRACE(pfnIsMapValid, P_PRE, ("filename=%s", filename));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void StaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex ) {
	ENGINE_TRACE(pfnStaticDecal, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int PrecacheGeneric(char *s) {
	ENGINE_TRACE(pfnPrecacheGeneric, P_PRE, ("name=%s", s));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
//! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
int GetPlayerUserId(edict_t *e ) {
	// more trace output in Post
	edict_t *ed = e;
	ENGINE_TRACE(pfnGetPlayerUserId, P_PRE, ("netname=%s",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void BuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) 
{
	ENGINE_TRACE(pfnBuildSoundMsg, P_PRE, ("sample=%s", sample));
	RETURN_META(MRES_IGNORED);
}
//! is this a dedicated server?
int IsDedicatedServer(void) {
	ENGINE_TRACE(pfnIsDedicatedServer, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
cvar_t *CVarGetPointer(const char *szVarName) {
	ENGINE_TRACE(pfnCVarGetPointer, P_PRE, ("cvar=%s", szVarName));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
//! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
unsigned int GetPlayerWONId(edict_t *e) {
	// more output in Post
	edict_t *ed = e;
	ENGINE_TRACE(pfnGetPlayerWONId, P_PRE, ("netname=%s",
				ed ? STRING(ed->v.netname) : "nil"));
	RETURN_META_VALUE(MRES_IGNORED, 0U);
}

//! YWB 8/1/99 TFF Physics additions
void Info_RemoveKey( char *s, const char *key ) {
	ENGINE_TRACE(pfnInfo_RemoveKey, P_PRE, ("key=%s", key));
	RETURN_META(MRES_IGNORED);
}
const char *GetPhysicsKeyValue( const edict_t *pClient, const char *key ) {
	ENGINE_TRACE(pfnGetPhysicsKeyValue, P_PRE, ("key=%s", key));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetPhysicsKeyValue( const edict_t *pClient, const char *key, const char *value ) {
	ENGINE_TRACE(pfnSetPhysicsKeyValue, P_PRE, ("key=%s, value=%s", key, value));
	RETURN_META(MRES_IGNORED);
}
const char *GetPhysicsInfoString( const edict_t *pClient ) {
	ENGINE_TRACE(pfnGetPhysicsInfoString, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned short PrecacheEvent( int type, const char *psz ) {
	ENGINE_TRACE(pfnPrecacheEvent, P_PRE, ("event=%s", psz));
	RETURN_META_VALUE(MRES_IGNORED, 0U);
}
void PlaybackEvent( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 ) 
{
	ENGINE_TRACE(pfnPlaybackEvent, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

unsigned char *SetFatPVS( float *org ) {
	ENGINE_TRACE(pfnSetFatPVS, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned char *SetFatPAS( float *org ) {
	ENGINE_TRACE(pfnSetFatPAS, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int CheckVisibility( const edict_t *entity, unsigned char *pset ) {
	ENGINE_TRACE(pfnCheckVisibility, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DeltaSetField( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaSetField, P_PRE, ("field=%s", fieldname));
	RETURN_META(MRES_IGNORED);
}
void DeltaUnsetField( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaUnsetField, P_PRE, ("field=%s", fieldname));
	RETURN_META(MRES_IGNORED);
}
void DeltaAddEncoder( char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) ) {
	ENGINE_TRACE(pfnDeltaAddEncoder, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
int GetCurrentPlayer( void ) {
	// trace output in Post
	ENGINE_TRACE(pfnGetCurrentPlayer, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int CanSkipPlayer( const edict_t *player ) {
	ENGINE_TRACE(pfnCanSkipPlayer, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DeltaFindField( struct delta_s *pFields, const char *fieldname ) {
	ENGINE_TRACE(pfnDeltaFindField, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DeltaSetFieldByIndex( struct delta_s *pFields, int fieldNumber ) {
	ENGINE_TRACE(pfnDeltaSetFieldByIndex, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}
void DeltaUnsetFieldByIndex( struct delta_s *pFields, int fieldNumber ) {
	ENGINE_TRACE(pfnDeltaUnsetFieldByIndex, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void SetGroupMask( int mask, int op ) {
	ENGINE_TRACE(pfnSetGroupMask, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

int engCreateInstancedBaseline( int classname, struct entity_state_s *baseline ) {
	ENGINE_TRACE(pfnCreateInstancedBaseline, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}
void Cvar_DirectSet( struct cvar_s *var, char *value ) {
	ENGINE_TRACE(pfnCvar_DirectSet, P_PRE, ("cvar=%s", var->name));
	RETURN_META(MRES_IGNORED);
}

//! Forces the client and server to be running with the same version of the specified file
//!( e.g., a player model ).
//! Calling this has no effect in single player
void ForceUnmodified( FORCE_TYPE type, float *mins, float *maxs, const char *filename ) {
	ENGINE_TRACE(pfnForceUnmodified, P_PRE, ("file=%s", filename));
	RETURN_META(MRES_IGNORED);
}

void GetPlayerStats( const edict_t *pClient, int *ping, int *packet_loss ) {
	ENGINE_TRACE(pfnGetPlayerStats, P_PRE, ("name=%s", STRING(pClient->v.netname)));
	RETURN_META(MRES_IGNORED);
}

void AddServerCommand( char *cmd_name, void (*function) (void) ) {
	ENGINE_TRACE(pfnAddServerCommand, P_PRE, ("cmd=%s", cmd_name));
	RETURN_META(MRES_IGNORED);
}

// Added in SDK 2.2:

//! For voice communications, set which clients hear eachother.
//! NOTE: these functions take player entity indices (starting at 1).
qboolean Voice_GetClientListening(int iReceiver, int iSender) {
	ENGINE_TRACE(pfnVoice_GetClientListening, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, false);
}
qboolean Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen) {
	ENGINE_TRACE(pfnVoice_SetClientListening, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, false);
}

// Added for HL 1109 (no SDK update):

const char *GetPlayerAuthId(edict_t *e) {
	// trace output in Post
	ENGINE_TRACE(pfnGetPlayerAuthId, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

// Added 2003/11/10 (no SDK update):

sequenceEntry_s*SequenceGet(const char* fileName, const char* entryName) {
	ENGINE_TRACE(pfnSequenceGet, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

sentenceEntry_s *SequencePickSentence(const char* groupName, int pickMethod, int *picked) {
	ENGINE_TRACE(pfnSequencePickSentence, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int GetFileSize(char *filename) {
	ENGINE_TRACE(pfnGetFileSize, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned int GetApproxWavePlayLen(const char *filepath) {
	ENGINE_TRACE(pfnGetApproxWavePlayLen, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int IsCareerMatch(void) {
	ENGINE_TRACE(pfnIsCareerMatch, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

int GetLocalizedStringLength(const char *label) {
	ENGINE_TRACE(pfnGetLocalizedStringLength, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void RegisterTutorMessageShown(int mid) {
	ENGINE_TRACE(pfnRegisterTutorMessageShown, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

int GetTimesTutorMessageShown(int mid) {
	ENGINE_TRACE(pfnGetTimesTutorMessageShown, P_PRE, (""));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ProcessTutorMessageDecayBuffer(int *buffer, int bufferLength) {
	ENGINE_TRACE(pfnProcessTutorMessageDecayBuffer, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void ConstructTutorMessageDecayBuffer(int *buffer, int bufferLength) {
	ENGINE_TRACE(pfnConstructTutorMessageDecayBuffer, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void ResetTutorMessageDecayData(void) {
	ENGINE_TRACE(pfnResetTutorMessageDecayData, P_PRE, (""));
	RETURN_META(MRES_IGNORED);
}

void QueryClientCvarValue(const edict_t *pEdict, const char *cvarName)
{
	ENGINE_TRACE(pfnQueryClientCvarValue, P_PRE, ("queried=%s",cvarName?cvarName:"nil"));
	RETURN_META(MRES_IGNORED);
}

void QueryClientCvarValue2(const edict_t *pEdict, const char *cvarName, int requestID)
{
	ENGINE_TRACE(pfnQueryClientCvarValue2, P_PRE, ("queried=%s, requestID=%d",cvarName?cvarName:"nil",requestID));
	RETURN_META(MRES_IGNORED);
}

int EngCheckParm(const char *pchCmdLineToken, char **pchNextVal)
{
	ENGINE_TRACE(pfnEngCheckParm, P_PRE, ("token=%s",pchCmdLineToken?pchCmdLineToken:"nil"));
	RETURN_META_VALUE(MRES_IGNORED, 0);
}

enginefuncs_t meta_engfuncs = {
	PrecacheModel,			// pfnPrecacheModel()
	PrecacheSound,			// pfnPrecacheSound()
	SetModel,				// pfnSetModel()
	ModelIndex,				// pfnModelIndex()
	ModelFrames,			// pfnModelFrames()

	SetSize,				// pfnSetSize()
	ChangeLevel,			// pfnChangeLevel()
	GetSpawnParms,			// pfnGetSpawnParms()
	SaveSpawnParms,			// pfnSaveSpawnParms()

	VecToYaw,				// pfnVecToYaw()
	VecToAngles,			// pfnVecToAngles()
	MoveToOrigin,			// pfnMoveToOrigin()
	ChangeYaw,				// pfnChangeYaw()
	ChangePitch,			// pfnChangePitch()

	FindEntityByString,		// pfnFindEntityByString()
	GetEntityIllum,			// pfnGetEntityIllum()
	FindEntityInSphere,		// pfnFindEntityInSphere()
	FindClientInPVS,		// pfnFindClientInPVS()
	EntitiesInPVS,			// pfnEntitiesInPVS()

	MakeVectors,			// pfnMakeVectors()
	AngleVectors,			// pfnAngleVectors()

	CreateEntity,			// pfnCreateEntity()
	RemoveEntity,			// pfnRemoveEntity()
	CreateNamedEntity,		// pfnCreateNamedEntity()

	MakeStatic,				// pfnMakeStatic()
	EntIsOnFloor,			// pfnEntIsOnFloor()
	DropToFloor,			// pfnDropToFloor()

	WalkMove,				// pfnWalkMove()
	SetOrigin,				// pfnSetOrigin()

	EmitSound,				// pfnEmitSound()
	EmitAmbientSound,		// pfnEmitAmbientSound()

	TraceLine,				// pfnTraceLine()
	TraceToss,				// pfnTraceToss()
	TraceMonsterHull,		// pfnTraceMonsterHull()
	TraceHull,				// pfnTraceHull()
	TraceModel,				// pfnTraceModel()
	TraceTexture,			// pfnTraceTexture()
	TraceSphere,			// pfnTraceSphere()
	GetAimVector,			// pfnGetAimVector()

	ServerCommand,			// pfnServerCommand()
	ServerExecute,			// pfnServerExecute()
	engClientCommand,		// pfnClientCommand()	// d'oh, ClientCommand in dllapi too

	ParticleEffect,			// pfnParticleEffect()
	LightStyle,				// pfnLightStyle()
	DecalIndex,				// pfnDecalIndex()
	PointContents,			// pfnPointContents()

	MessageBegin,			// pfnMessageBegin()
	MessageEnd,				// pfnMessageEnd()

	WriteByte,				// pfnWriteByte()
	WriteChar,				// pfnWriteChar()
	WriteShort,				// pfnWriteShort()
	WriteLong,				// pfnWriteLong()
	WriteAngle,				// pfnWriteAngle()
	WriteCoord,				// pfnWriteCoord()
	WriteString,			// pfnWriteString()
	WriteEntity,			// pfnWriteEntity()

	CVarRegister,			// pfnCVarRegister()
	CVarGetFloat,			// pfnCVarGetFloat()
	CVarGetString,			// pfnCVarGetString()
	CVarSetFloat,			// pfnCVarSetFloat()
	CVarSetString,			// pfnCVarSetString()

	AlertMessage,			// pfnAlertMessage()
	EngineFprintf,			// pfnEngineFprintf()

	PvAllocEntPrivateData,	// pfnPvAllocEntPrivateData()
	PvEntPrivateData,		// pfnPvEntPrivateData()
	FreeEntPrivateData,		// pfnFreeEntPrivateData()

	SzFromIndex,			// pfnSzFromIndex()
	AllocString,			// pfnAllocString()

	GetVarsOfEnt, 			// pfnGetVarsOfEnt()
	PEntityOfEntOffset,		// pfnPEntityOfEntOffset()
	EntOffsetOfPEntity,		// pfnEntOffsetOfPEntity()
	IndexOfEdict,			// pfnIndexOfEdict()
	PEntityOfEntIndex,		// pfnPEntityOfEntIndex()
	FindEntityByVars,		// pfnFindEntityByVars()
	GetModelPtr,			// pfnGetModelPtr()

	RegUserMsg,				// pfnRegUserMsg()

	AnimationAutomove,		// pfnAnimationAutomove()
	GetBonePosition,		// pfnGetBonePosition()

	FunctionFromName,		// pfnFunctionFromName()
	NameForFunction,		// pfnNameForFunction()

	ClientPrintf,			// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	ServerPrint,			// pfnServerPrint()

	Cmd_Args,				// pfnCmd_Args()				//! these 3 added 
	Cmd_Argv,				// pfnCmd_Argv()				//! so game DLL can easily 
	Cmd_Argc,				// pfnCmd_Argc()				//! access client 'cmd' strings

	GetAttachment,			// pfnGetAttachment()

	CRC32_Init,				// pfnCRC32_Init()
	CRC32_ProcessBuffer,	// pfnCRC32_ProcessBuffer()
	CRC32_ProcessByte,		// pfnCRC32_ProcessByte()
	CRC32_Final,			// pfnCRC32_Final()

	RandomLong,				// pfnRandomLong()
	RandomFloat,			// pfnRandomFloat()

	SetView,				// pfnSetView()
	Time,					// pfnTime()
	CrosshairAngle,			// pfnCrosshairAngle()

	LoadFileForMe,			// pfnLoadFileForMe()
	FreeFile,				// pfnFreeFile()

	EndSection,				// pfnEndSection()				//! trigger_endsection
	CompareFileTime,		// pfnCompareFileTime()
	GetGameDir,				// pfnGetGameDir()
	Cvar_RegisterVariable,	// pfnCvar_RegisterVariable()
	FadeClientVolume,		// pfnFadeClientVolume()
	SetClientMaxspeed,		// pfnSetClientMaxspeed()
	CreateFakeClient,		// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	RunPlayerMove,			// pfnRunPlayerMove()
	NumberOfEntities,		// pfnNumberOfEntities()

	GetInfoKeyBuffer,		// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	InfoKeyValue,			// pfnInfoKeyValue()
	SetKeyValue,			// pfnSetKeyValue()
	SetClientKeyValue,		// pfnSetClientKeyValue()

	IsMapValid,				// pfnIsMapValid()
	StaticDecal,			// pfnStaticDecal()
	PrecacheGeneric,		// pfnPrecacheGeneric()
	GetPlayerUserId, 		// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	BuildSoundMsg,			// pfnBuildSoundMsg()
	IsDedicatedServer,		// pfnIsDedicatedServer()		//! is this a dedicated server?
	CVarGetPointer,			// pfnCVarGetPointer()
	GetPlayerWONId,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	Info_RemoveKey,			// pfnInfo_RemoveKey()
	GetPhysicsKeyValue,		// pfnGetPhysicsKeyValue()
	SetPhysicsKeyValue,		// pfnSetPhysicsKeyValue()
	GetPhysicsInfoString,	// pfnGetPhysicsInfoString()
	PrecacheEvent,			// pfnPrecacheEvent()
	PlaybackEvent,			// pfnPlaybackEvent()

	SetFatPVS,				// pfnSetFatPVS()
	SetFatPAS,				// pfnSetFatPAS()

	CheckVisibility,		// pfnCheckVisibility()

	DeltaSetField,			// pfnDeltaSetField()
	DeltaUnsetField,		// pfnDeltaUnsetField()
	DeltaAddEncoder,		// pfnDeltaAddEncoder()
	GetCurrentPlayer,		// pfnGetCurrentPlayer()
	CanSkipPlayer,			// pfnCanSkipPlayer()
	DeltaFindField,			// pfnDeltaFindField()
	DeltaSetFieldByIndex,	// pfnDeltaSetFieldByIndex()
	DeltaUnsetFieldByIndex,	// pfnDeltaUnsetFieldByIndex()

	SetGroupMask,			// pfnSetGroupMask()

	engCreateInstancedBaseline, // pfnCreateInstancedBaseline()		// d'oh, CreateInstancedBaseline in dllapi too
	Cvar_DirectSet,			// pfnCvar_DirectSet()

	ForceUnmodified,		// pfnForceUnmodified()

	GetPlayerStats,			// pfnGetPlayerStats()

	AddServerCommand,		// pfnAddServerCommand()

	// Added in SDK 2.2:
	Voice_GetClientListening,	// pfnVoice_GetClientListening()
	Voice_SetClientListening,	// pfnVoice_SetClientListening()

	// Added for HL 1109 (no SDK update):
	GetPlayerAuthId,		// pfnGetPlayerAuthId()

	// Added 2003/11/10 (no SDK update):
	SequenceGet,					// pfnSequenceGet()
	SequencePickSentence,			// pfnSequencePickSentence()
	GetFileSize,					// pfnGetFileSize()
	GetApproxWavePlayLen,			// pfnGetApproxWavePlayLen()
	IsCareerMatch,					// pfnIsCareerMatch()
	GetLocalizedStringLength,		// pfnGetLocalizedStringLength()
	RegisterTutorMessageShown,		// pfnRegisterTutorMessageShown()
	GetTimesTutorMessageShown,		// pfnGetTimesTutorMessageShown()
	ProcessTutorMessageDecayBuffer,	// pfnProcessTutorMessageDecayBuffer()
	ConstructTutorMessageDecayBuffer,	// pfnConstructTutorMessageDecayBuffer()
	ResetTutorMessageDecayData,		// pfnResetTutorMessageDecayData()
	QueryClientCvarValue,			// pfnQueryClientCvarValue()
	QueryClientCvarValue2,			// pfnQueryClientCvarValue2()
	EngCheckParm,					// pfnEngCheckParm()
};

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, 
		int *interfaceVersion) 
{
	LOG_DEVELOPER(PLID, "called: GetEngineFunctions; version=%d", 
			*interfaceVersion);
	if(!pengfuncsFromEngine) {
		LOG_ERROR(PLID, 
				"GetEngineFunctions called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, 
				"GetEngineFunctions version mismatch; requested=%d ours=%d", 
				*interfaceVersion, ENGINE_INTERFACE_VERSION);
		// Tell metamod what version we had, so it can figure out who is 
		// out of date.
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
	return TRUE;
}
