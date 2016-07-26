#include "precompiled.h"

// Engine routines, functions returning "void".
#define META_ENGINE_HANDLE_void(FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	main_hook_function_void(offsetof(engine_info_t, pfnName), e_api_engine, offsetof(enginefuncs_t, pfnName), &packed_args); \
	API_END_TSC_TRACKING()

// Engine routines, functions returning an actual value.
#define META_ENGINE_HANDLE(ret_t, ret_init, FN_TYPE, pfnName, pack_args_type, pfn_args) \
	API_START_TSC_TRACKING(); \
	API_PACK_ARGS(pack_args_type, pfn_args); \
	class_ret_t ret_val(main_hook_function(class_ret_t((ret_t)ret_init), offsetof(engine_info_t, pfnName), e_api_engine, offsetof(enginefuncs_t, pfnName), &packed_args)); \
	API_END_TSC_TRACKING()

// For varargs functions
#ifndef DO_NOT_FIX_VARARG_ENGINE_API_WARPERS
	#define MAKE_FORMATED_STRING(szFmt) \
		char strbuf[MAX_STRBUF_LEN]; \
		char *buf = strbuf; \
		{ \
			int len; \
			va_list vargs; \
			va_start(vargs, szFmt); \
			len = safe_vsnprintf(strbuf, sizeof(strbuf), szFmt, vargs); \
			va_end(vargs); \
			if ((unsigned)len >= sizeof(strbuf)) { \
				buf = (char *)malloc(len + 1); \
				if (buf) { \
					va_start(vargs, szFmt); \
					safevoid_vsnprintf(buf, len + 1, szFmt, vargs); \
					va_end(vargs); \
				} else { \
					buf = strbuf; \
				} \
			} \
		}
	#define CLEAN_FORMATED_STRING() \
		if (buf != strbuf) \
			free(buf);
#else
	#define MAKE_FORMATED_STRING(szFmt) \
		char buf[MAX_STRBUF_LEN]; \
		va_list ap; \
		va_start(ap, szFmt); \
		safevoid_vsnprintf(buf, sizeof(buf), szFmt, ap); \
		va_end(ap);

	#define CLEAN_FORMATED_STRING()
#endif

// Engine routines, printf-style functions returning "void".
#define META_ENGINE_HANDLE_void_varargs(FN_TYPE, pfnName, pack_args_type, pfn_arg, fmt_arg) \
	MAKE_FORMATED_STRING(fmt_arg); \
	API_START_TSC_TRACKING(); \
	META_DEBUG(engine_info.pfnName.loglevel, ("In %s: fmt=%s", engine_info.pfnName.name, fmt_arg)); \
	API_PACK_ARGS(pack_args_type, (pfn_arg, "%s", buf)); \
	main_hook_function_void(offsetof(engine_info_t, pfnName), e_api_engine, offsetof(enginefuncs_t, pfnName), &packed_args); \
	API_END_TSC_TRACKING() \
	CLEAN_FORMATED_STRING()

// Engine routines, printf-style functions returning an actual value.
#define META_ENGINE_HANDLE_varargs(ret_t, ret_init, FN_TYPE, pfnName, pack_args_type, pfn_arg, fmt_arg) \
	MAKE_FORMATED_STRING(fmt_arg); \
	API_START_TSC_TRACKING(); \
	META_DEBUG(engine_info.pfnName.loglevel, ("In %s: fmt=%s", engine_info.pfnName.name, fmt_arg)); \
	API_PACK_ARGS(pack_args_type, (pfn_arg, "%s", buf)); \
	class_ret_t ret_val(main_hook_function(class_ret_t((ret_t)ret_init), offsetof(engine_info_t, pfnName), e_api_engine, offsetof(enginefuncs_t, pfnName), &packed_args)); \
	API_END_TSC_TRACKING() \
	CLEAN_FORMATED_STRING()


int mm_PrecacheModel(const char *s)
{
	META_ENGINE_HANDLE(int, 0, FN_PRECACHEMODEL, pfnPrecacheModel, p, (s));
	RETURN_API(int)
}

int mm_PrecacheSound(const char *s)
{
	META_ENGINE_HANDLE(int, 0, FN_PRECACHESOUND, pfnPrecacheSound, p, (s));
	RETURN_API(int)
}

void mm_SetModel(edict_t *e, const char *m)
{
	META_ENGINE_HANDLE_void(FN_SETMODEL, pfnSetModel, 2p, (e, m));
	RETURN_API_void()
}

int mm_ModelIndex(const char *m)
{
	META_ENGINE_HANDLE(int, 0, FN_MODELINDEX, pfnModelIndex, p, (m));
	RETURN_API(int)
}

int mm_ModelFrames(int modelIndex)
{
	META_ENGINE_HANDLE(int, 0, FN_MODELFRAMES, pfnModelFrames, i, (modelIndex));
	RETURN_API(int)
}

void mm_SetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
	META_ENGINE_HANDLE_void(FN_SETSIZE, pfnSetSize, 3p, (e, rgflMin, rgflMax));
	RETURN_API_void()
}

void mm_ChangeLevel(const char *s1, const char *s2)
{
	META_ENGINE_HANDLE_void(FN_CHANGELEVEL, pfnChangeLevel, 2p, (s1, s2));
	RETURN_API_void()
}

void mm_GetSpawnParms(edict_t *ent)
{
	META_ENGINE_HANDLE_void(FN_GETSPAWNPARMS, pfnGetSpawnParms, p, (ent));
	RETURN_API_void()
}

void mm_SaveSpawnParms(edict_t *ent)
{
	META_ENGINE_HANDLE_void(FN_SAVESPAWNPARMS, pfnSaveSpawnParms, p, (ent));
	RETURN_API_void()
}

float mm_VecToYaw(const float *rgflVector)
{
	META_ENGINE_HANDLE(float, 0.0, FN_VECTOYAW, pfnVecToYaw, p, (rgflVector));
	RETURN_API(float)
}

void mm_VecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
	META_ENGINE_HANDLE_void(FN_VECTOANGLES, pfnVecToAngles, 2p, (rgflVectorIn, rgflVectorOut));
	RETURN_API_void()
}

void mm_MoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
	META_ENGINE_HANDLE_void(FN_MOVETOORIGIN, pfnMoveToOrigin, 2pfi, (ent, pflGoal, dist, iMoveType));
	RETURN_API_void()
}

void mm_ChangeYaw(edict_t *ent)
{
	META_ENGINE_HANDLE_void(FN_CHANGEYAW, pfnChangeYaw, p, (ent));
	RETURN_API_void()
}

void mm_ChangePitch(edict_t *ent)
{
	META_ENGINE_HANDLE_void(FN_CHANGEPITCH, pfnChangePitch, p, (ent));
	RETURN_API_void()
}

edict_t *mm_FindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_FINDENTITYBYSTRING, pfnFindEntityByString, 3p, (pEdictStartSearchAfter, pszField, pszValue));
	RETURN_API(edict_t *)
}

int mm_GetEntityIllum(edict_t *pEnt)
{
	META_ENGINE_HANDLE(int, 0, FN_GETENTITYILLUM, pfnGetEntityIllum, p, (pEnt));
	RETURN_API(int)
}

edict_t *mm_FindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_FINDENTITYINSPHERE, pfnFindEntityInSphere, 2pf, (pEdictStartSearchAfter, org, rad));
	RETURN_API(edict_t *)
}

edict_t *mm_FindClientInPVS(edict_t *pEdict)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_FINDCLIENTINPVS, pfnFindClientInPVS, p, (pEdict));
	RETURN_API(edict_t *)
}

edict_t *mm_EntitiesInPVS(edict_t *pplayer)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_ENTITIESINPVS, pfnEntitiesInPVS, p, (pplayer));
	RETURN_API(edict_t *)
}

void mm_MakeVectors(const float *rgflVector)
{
	META_ENGINE_HANDLE_void(FN_MAKEVECTORS, pfnMakeVectors, p, (rgflVector));
	RETURN_API_void()
}

void mm_AngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
	META_ENGINE_HANDLE_void(FN_ANGLEVECTORS, pfnAngleVectors, 4p, (rgflVector, forward, right, up));
	RETURN_API_void()
}

edict_t *mm_CreateEntity()
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_CREATEENTITY, pfnCreateEntity, void, (VOID_ARG));
	RETURN_API(edict_t *)
}

void mm_RemoveEntity(edict_t *e)
{
	META_ENGINE_HANDLE_void(FN_REMOVEENTITY, pfnRemoveEntity, p, (e));
	RETURN_API_void()
}

edict_t *mm_CreateNamedEntity(int className)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_CREATENAMEDENTITY, pfnCreateNamedEntity, i, (className));
	RETURN_API(edict_t *)
}

void mm_MakeStatic(edict_t *ent)
{
	META_ENGINE_HANDLE_void(FN_MAKESTATIC, pfnMakeStatic, p, (ent));
	RETURN_API_void()
}

int mm_EntIsOnFloor(edict_t *e)
{
	META_ENGINE_HANDLE(int, 0, FN_ENTISONFLOOR, pfnEntIsOnFloor, p, (e));
	RETURN_API(int)
}

int mm_DropToFloor(edict_t *e)
{
	META_ENGINE_HANDLE(int, 0, FN_DROPTOFLOOR, pfnDropToFloor, p, (e));
	RETURN_API(int)
}

int mm_WalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
	META_ENGINE_HANDLE(int, 0, FN_WALKMOVE, pfnWalkMove, p2fi, (ent, yaw, dist, iMode));
	RETURN_API(int)
}

void mm_SetOrigin(edict_t *e, const float *rgflOrigin)
{
	META_ENGINE_HANDLE_void(FN_SETORIGIN, pfnSetOrigin, 2p, (e, rgflOrigin));
	RETURN_API_void()
}

void mm_EmitSound(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch)
{
	META_ENGINE_HANDLE_void(FN_EMITSOUND, pfnEmitSound, pip2f2i, (entity, channel, sample, volume, attenuation, fFlags, pitch));
	RETURN_API_void()
}

void mm_EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
	META_ENGINE_HANDLE_void(FN_EMITAMBIENTSOUND, pfnEmitAmbientSound, 3p2f2i, (entity, pos, samp, vol, attenuation, fFlags, pitch));
	RETURN_API_void()
}

void mm_TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	META_ENGINE_HANDLE_void(FN_TRACELINE, pfnTraceLine, 2pi2p, (v1, v2, fNoMonsters, pentToSkip, ptr));
	RETURN_API_void()
}

void mm_TraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr)
{
	META_ENGINE_HANDLE_void(FN_TRACETOSS, pfnTraceToss, 3p, (pent, pentToIgnore, ptr));
	RETURN_API_void()
}

int mm_TraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	META_ENGINE_HANDLE(int, 0, FN_TRACEMONSTERHULL, pfnTraceMonsterHull, 3pi2p, (pEdict, v1, v2, fNoMonsters, pentToSkip, ptr));
	RETURN_API(int)
}

void mm_TraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
	META_ENGINE_HANDLE_void(FN_TRACEHULL, pfnTraceHull, 2p2i2p, (v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr));
	RETURN_API_void()
}

void mm_TraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
	META_ENGINE_HANDLE_void(FN_TRACEMODEL, pfnTraceModel, 2pi2p, (v1, v2, hullNumber, pent, ptr));
	RETURN_API_void()
}

const char *mm_TraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_TRACETEXTURE, pfnTraceTexture, 3p, (pTextureEntity, v1, v2));
	RETURN_API(const char *)
}

void mm_TraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
	META_ENGINE_HANDLE_void(FN_TRACESPHERE, pfnTraceSphere, 2pif2p, (v1, v2, fNoMonsters, radius, pentToSkip, ptr));
	RETURN_API_void()
}

void mm_GetAimVector(edict_t *ent, float speed, float *rgflReturn)
{
	META_ENGINE_HANDLE_void(FN_GETAIMVECTOR, pfnGetAimVector, pfp, (ent, speed, rgflReturn));
	RETURN_API_void()
}

void mm_ServerCommand(char *str)
{
	META_ENGINE_HANDLE_void(FN_SERVERCOMMAND, pfnServerCommand, p, (str));
	RETURN_API_void()
}

void mm_ServerExecute()
{
	META_ENGINE_HANDLE_void(FN_SERVEREXECUTE, pfnServerExecute, void, (VOID_ARG));
	RETURN_API_void()
}

void mm_engClientCommand(edict_t *pEdict, char *szFmt, ...)
{
	META_ENGINE_HANDLE_void_varargs(FN_CLIENTCOMMAND_ENG, pfnClientCommand, 2pV, pEdict, szFmt);
	RETURN_API_void()
}

void mm_ParticleEffect(const float *org, const float *dir, float color, float count)
{
	META_ENGINE_HANDLE_void(FN_PARTICLEEFFECT, pfnParticleEffect, 2p2f, (org, dir, color, count));
	RETURN_API_void()
}

void mm_LightStyle(int style, char *val)
{
	META_ENGINE_HANDLE_void(FN_LIGHTSTYLE, pfnLightStyle, ip, (style, val));
	RETURN_API_void()
}

int mm_DecalIndex(const char *name)
{
	META_ENGINE_HANDLE(int, 0, FN_DECALINDEX, pfnDecalIndex, p, (name));
	RETURN_API(int)
}

int mm_PointContents(const float *rgflVector)
{
	META_ENGINE_HANDLE(int, 0, FN_POINTCONTENTS, pfnPointContents, p, (rgflVector));
	RETURN_API(int)
}

void mm_MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	META_ENGINE_HANDLE_void(FN_MESSAGEBEGIN, pfnMessageBegin, 2i2p, (msg_dest, msg_type, pOrigin, ed));
	RETURN_API_void()
}

void mm_MessageEnd()
{
	META_ENGINE_HANDLE_void(FN_MESSAGEEND, pfnMessageEnd, void, (VOID_ARG));
	RETURN_API_void()
}

void mm_WriteByte(int iValue)
{
	META_ENGINE_HANDLE_void(FN_WRITEBYTE, pfnWriteByte, i, (iValue));
	RETURN_API_void()
}

void mm_WriteChar(int iValue)
{
	META_ENGINE_HANDLE_void(FN_WRITECHAR, pfnWriteChar, i, (iValue));
	RETURN_API_void()
}

void mm_WriteShort(int iValue)
{
	META_ENGINE_HANDLE_void(FN_WRITESHORT, pfnWriteShort, i, (iValue));
	RETURN_API_void()
}

void mm_WriteLong(int iValue)
{
	META_ENGINE_HANDLE_void(FN_WRITELONG, pfnWriteLong, i, (iValue));
	RETURN_API_void()
}

void mm_WriteAngle(float flValue)
{
	META_ENGINE_HANDLE_void(FN_WRITEANGLE, pfnWriteAngle, f, (flValue));
	RETURN_API_void()
}

void mm_WriteCoord(float flValue)
{
	META_ENGINE_HANDLE_void(FN_WRITECOORD, pfnWriteCoord, f, (flValue));
	RETURN_API_void()
}

void mm_WriteString(const char *sz)
{
	META_ENGINE_HANDLE_void(FN_WRITESTRING, pfnWriteString, p, (sz));
	RETURN_API_void()
}

void mm_WriteEntity(int iValue)
{
	META_ENGINE_HANDLE_void(FN_WRITEENTITY, pfnWriteEntity, i, (iValue));
	RETURN_API_void()
}

void mm_CVarRegister(cvar_t *pCvar)
{
	META_ENGINE_HANDLE_void(FN_CVARREGISTER, pfnCVarRegister, p, (pCvar));
	RETURN_API_void()
}

float mm_CVarGetFloat(const char *szVarName)
{
	META_ENGINE_HANDLE(float, 0.0, FN_CVARGETFLOAT, pfnCVarGetFloat, p, (szVarName));
	RETURN_API(float)
}

const char *mm_CVarGetString(const char *szVarName)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_CVARGETSTRING, pfnCVarGetString, p, (szVarName));
	RETURN_API(const char *)
}

void mm_CVarSetFloat(const char *szVarName, float flValue)
{
	META_ENGINE_HANDLE_void(FN_CVARSETFLOAT, pfnCVarSetFloat, pf, (szVarName, flValue));

	meta_debug_value = (int)meta_debug.value;

	RETURN_API_void()
}

void mm_CVarSetString(const char *szVarName, const char *szValue)
{
	META_ENGINE_HANDLE_void(FN_CVARSETSTRING, pfnCVarSetString, 2p, (szVarName, szValue));

	meta_debug_value = (int)meta_debug.value;

	RETURN_API_void()
}

void mm_AlertMessage(ALERT_TYPE atype, const char *szFmt, ...)
{
	META_ENGINE_HANDLE_void_varargs(FN_ALERTMESSAGE, pfnAlertMessage, ipV, atype, szFmt);
	RETURN_API_void()
}

void mm_EngineFprintf(void *pfile, const char *szFmt, ...)
{
	META_ENGINE_HANDLE_void_varargs(FN_ENGINEFPRINTF, pfnEngineFprintf, 2pV, pfile, szFmt);
	RETURN_API_void()
}

void *mm_PvAllocEntPrivateData(edict_t *pEdict, int32 cb)
{
	META_ENGINE_HANDLE(void *, NULL, FN_PVALLOCENTPRIVATEDATA, pfnPvAllocEntPrivateData, pi, (pEdict, cb));
	RETURN_API(void *)
}

void *mm_PvEntPrivateData(edict_t *pEdict)
{
	META_ENGINE_HANDLE(void *, NULL, FN_PVENTPRIVATEDATA, pfnPvEntPrivateData, p, (pEdict));
	RETURN_API(void *)
}

void mm_FreeEntPrivateData(edict_t *pEdict)
{
	META_ENGINE_HANDLE_void(FN_FREEENTPRIVATEDATA, pfnFreeEntPrivateData, p, (pEdict));
	RETURN_API_void()
}

const char *mm_SzFromIndex(int iString)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_SZFROMINDEX, pfnSzFromIndex, i, (iString));
	RETURN_API(const char *)
}

int mm_AllocString(const char *szValue)
{
	META_ENGINE_HANDLE(int, 0, FN_ALLOCSTRING, pfnAllocString, p, (szValue));
	RETURN_API(int)
}

struct entvars_s *mm_GetVarsOfEnt(edict_t *pEdict)
{
	META_ENGINE_HANDLE(struct entvars_s *, NULL, FN_GETVARSOFENT, pfnGetVarsOfEnt, p, (pEdict));
	RETURN_API(struct entvars_s *)
}

edict_t *mm_PEntityOfEntOffset(int iEntOffset)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_PENTITYOFENTOFFSET, pfnPEntityOfEntOffset, i, (iEntOffset));
	RETURN_API(edict_t *)
}

int mm_EntOffsetOfPEntity(const edict_t *pEdict)
{
	META_ENGINE_HANDLE(int, 0, FN_ENTOFFSETOFPENTITY, pfnEntOffsetOfPEntity, p, (pEdict));
	RETURN_API(int)
}

int mm_IndexOfEdict(const edict_t *pEdict)
{
	META_ENGINE_HANDLE(int, 0, FN_INDEXOFEDICT, pfnIndexOfEdict, p, (pEdict));
	RETURN_API(int)
}

edict_t *mm_PEntityOfEntIndex(int iEntIndex)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_PENTITYOFENTINDEX, pfnPEntityOfEntIndex, i, (iEntIndex));
	RETURN_API(edict_t *)
}

edict_t *mm_FindEntityByVars(struct entvars_s *pvars)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_FINDENTITYBYVARS, pfnFindEntityByVars, p, (pvars));
	RETURN_API(edict_t *)
}

void *mm_GetModelPtr(edict_t *pEdict)
{
	META_ENGINE_HANDLE(void *, NULL, FN_GETMODELPTR, pfnGetModelPtr, p, (pEdict));
	RETURN_API(void *)
}

int mm_RegUserMsg(const char *pszName, int iSize)
{
	int imsgid;
	MRegMsg *nmsg=NULL;
	META_ENGINE_HANDLE(int, 0, FN_REGUSERMSG, pfnRegUserMsg, pi, (pszName, iSize));
	// Expand the macro, since we need to do extra work.
	/// RETURN_API(int)
	imsgid = GET_RET_CLASS(ret_val, int);

	// Add the msgid, name, and size to our saved list, if we haven't
	// already.
	nmsg=RegMsgs->find(imsgid);
	if (nmsg)
{
		if (FStrEq(pszName, nmsg->name))
			// This name/msgid pair was already registered.
			META_DEBUG(3, ("user message registered again: name=%s, msgid=%d", pszName, imsgid));
		else
			// This msgid was previously used by a different message name.
			META_WARNING("user message id reused: msgid=%d, oldname=%s, newname=%s", imsgid, nmsg->name, pszName);
	}
	else
		RegMsgs->add(pszName, imsgid, iSize);

	return imsgid;
}

void mm_AnimationAutomove(const edict_t *pEdict, float flTime)
{
	META_ENGINE_HANDLE_void(FN_ANIMATIONAUTOMOVE, pfnAnimationAutomove, pf, (pEdict, flTime));
	RETURN_API_void()
}

void mm_GetBonePosition(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles)
{
	META_ENGINE_HANDLE_void(FN_GETBONEPOSITION, pfnGetBonePosition, pi2p, (pEdict, iBone, rgflOrigin, rgflAngles));
	RETURN_API_void()
}

uint32 mm_FunctionFromName(const char *pName)
{
	META_ENGINE_HANDLE(uint32, 0, FN_FUNCTIONFROMNAME, pfnFunctionFromName, p, (pName));
	RETURN_API(uint32)
}

const char *mm_NameForFunction(uint32 function)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_NAMEFORFUNCTION, pfnNameForFunction, ui, (function));
	RETURN_API(const char *)
}

//! JOHN: engine callbacks so game DLL can print messages to individual clients
void mm_ClientPrintf(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg)
{
	META_ENGINE_HANDLE_void(FN_CLIENTPRINTF, pfnClientPrintf, pip, (pEdict, ptype, szMsg));
	RETURN_API_void()
}

void mm_ServerPrint(const char *szMsg)
{
	META_ENGINE_HANDLE_void(FN_SERVERPRINT, pfnServerPrint, p, (szMsg));
	RETURN_API_void()
}

//! these 3 added so game DLL can easily access client 'cmd' strings
const char *mm_Cmd_Args(void)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_CMD_ARGS, pfnCmd_Args, void, (VOID_ARG));
	RETURN_API(const char *)
}

const char *mm_Cmd_Argv(int argc)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_CMD_ARGV, pfnCmd_Argv, i, (argc));
	RETURN_API(const char *)
}

int mm_Cmd_Argc(void)
{
	META_ENGINE_HANDLE(int, 0, FN_CMD_ARGC, pfnCmd_Argc, void, (VOID_ARG));
	RETURN_API(int)
}

void mm_GetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles)
{
	META_ENGINE_HANDLE_void(FN_GETATTACHMENT, pfnGetAttachment, pi2p, (pEdict, iAttachment, rgflOrigin, rgflAngles));
	RETURN_API_void()
}

void mm_CRC32_Init(CRC32_t *pulCRC)
{
	META_ENGINE_HANDLE_void(FN_CRC32_INIT, pfnCRC32_Init, p, (pulCRC));
	RETURN_API_void()
}

void mm_CRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
	META_ENGINE_HANDLE_void(FN_CRC32_PROCESSBUFFER, pfnCRC32_ProcessBuffer, 2pi, (pulCRC, p, len));
	RETURN_API_void()
}

void mm_CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
	META_ENGINE_HANDLE_void(FN_CRC32_PROCESSBYTE, pfnCRC32_ProcessByte, puc, (pulCRC, ch));
	RETURN_API_void()
}

CRC32_t mm_CRC32_Final(CRC32_t pulCRC)
{
	META_ENGINE_HANDLE(CRC32_t, 0, FN_CRC32_FINAL, pfnCRC32_Final, ul, (pulCRC));
	RETURN_API(CRC32_t)
}

int32 mm_RandomLong(int32 lLow, int32 lHigh)
{
	META_ENGINE_HANDLE(int32, 0, FN_RANDOMLONG, pfnRandomLong, 2i, (lLow, lHigh));
	RETURN_API(int32)
}

float mm_RandomFloat(float flLow, float flHigh)
{
	META_ENGINE_HANDLE(float, 0.0, FN_RANDOMFLOAT, pfnRandomFloat, 2f, (flLow, flHigh));
	RETURN_API(float)
}

void mm_SetView(const edict_t *pClient, const edict_t *pViewent)
{
	META_ENGINE_HANDLE_void(FN_SETVIEW, pfnSetView, 2p, (pClient, pViewent));
	RETURN_API_void()
}

float mm_Time(void)
{
	META_ENGINE_HANDLE(float, 0.0, FN_TIME, pfnTime, void, (VOID_ARG));
	RETURN_API(float)
}

void mm_CrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
	META_ENGINE_HANDLE_void(FN_CROSSHAIRANGLE, pfnCrosshairAngle, p2f, (pClient, pitch, yaw));
	RETURN_API_void()
}

byte *mm_LoadFileForMe(char *filename, int *pLength)
{
	META_ENGINE_HANDLE(byte *, NULL, FN_LOADFILEFORME, pfnLoadFileForMe, 2p, (filename, pLength));
	RETURN_API(byte *)
}

void mm_FreeFile(void *buffer)
{
	META_ENGINE_HANDLE_void(FN_FREEFILE, pfnFreeFile, p, (buffer));
	RETURN_API_void()
}

//! trigger_endsection
void mm_EndSection(const char *pszSectionName)
{
	META_ENGINE_HANDLE_void(FN_ENDSECTION, pfnEndSection, p, (pszSectionName));
	RETURN_API_void()
}

int mm_CompareFileTime(char *filename1, char *filename2, int *iCompare)
{
	META_ENGINE_HANDLE(int, 0, FN_COMPAREFILETIME, pfnCompareFileTime, 3p, (filename1, filename2, iCompare));
	RETURN_API(int)
}

void mm_GetGameDir(char *szGetGameDir)
{
	META_ENGINE_HANDLE_void(FN_GETGAMEDIR, pfnGetGameDir, p, (szGetGameDir));
	RETURN_API_void()
}

void mm_Cvar_RegisterVariable(cvar_t *variable)
{
	META_ENGINE_HANDLE_void(FN_CVAR_REGISTERVARIABLE, pfnCvar_RegisterVariable, p, (variable));
	RETURN_API_void()
}

void mm_FadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
	META_ENGINE_HANDLE_void(FN_FADECLIENTVOLUME, pfnFadeClientVolume, p4i, (pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds));
	RETURN_API_void()
}

void mm_SetClientMaxspeed(edict_t *pEdict, float fNewMaxspeed)
{
	META_ENGINE_HANDLE_void(FN_SETCLIENTMAXSPEED, pfnSetClientMaxspeed, pf, (pEdict, fNewMaxspeed));
	RETURN_API_void()
}

//! returns NULL if fake client can't be created
edict_t *mm_CreateFakeClient(const char *netname)
{
	META_ENGINE_HANDLE(edict_t *, NULL, FN_CREATEFAKECLIENT, pfnCreateFakeClient, p, (netname));
	RETURN_API(edict_t *)
}

void mm_RunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec)
{
	META_ENGINE_HANDLE_void(FN_RUNPLAYERMOVE, pfnRunPlayerMove, 2p3fus2uc, (fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec));
	RETURN_API_void()
}

int mm_NumberOfEntities()
{
	META_ENGINE_HANDLE(int, 0, FN_NUMBEROFENTITIES, pfnNumberOfEntities, void, (VOID_ARG));
	RETURN_API(int)
}

//! passing in NULL gets the serverinfo
char *mm_GetInfoKeyBuffer(edict_t *e)
{
	META_ENGINE_HANDLE(char *, NULL, FN_GETINFOKEYBUFFER, pfnGetInfoKeyBuffer, p, (e));
	RETURN_API(char *)
}

char *mm_InfoKeyValue(char *infobuffer, const char *key)
{
	META_ENGINE_HANDLE(char *, NULL, FN_INFOKEYVALUE, pfnInfoKeyValue, 2p, (infobuffer, key));
	RETURN_API(char *)
}

void mm_SetKeyValue(char *infobuffer, const char *key, const char *value)
{
	META_ENGINE_HANDLE_void(FN_SETKEYVALUE, pfnSetKeyValue, 3p, (infobuffer, key, value));
	RETURN_API_void()
}

void mm_SetClientKeyValue(int clientIndex, char *infobuffer, const char *key, const char *value)
{
	META_ENGINE_HANDLE_void(FN_SETCLIENTKEYVALUE, pfnSetClientKeyValue, i3p, (clientIndex, infobuffer, key, value));
	RETURN_API_void()
}

int mm_IsMapValid(char *filename)
{
	META_ENGINE_HANDLE(int, 0, FN_ISMAPVALID, pfnIsMapValid, p, (filename));
	RETURN_API(int)
}

void mm_StaticDecal(const float *origin, int decalIndex, int entityIndex, int modelIndex)
{
	META_ENGINE_HANDLE_void(FN_STATICDECAL, pfnStaticDecal, p3i, (origin, decalIndex, entityIndex, modelIndex));
	RETURN_API_void()
}

int mm_PrecacheGeneric(char *s)
{
	META_ENGINE_HANDLE(int, 0, FN_PRECACHEGENERIC, pfnPrecacheGeneric, p, (s));
	RETURN_API(int)
}

//! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
int mm_GetPlayerUserId(edict_t *e)
{
	META_ENGINE_HANDLE(int, 0, FN_GETPLAYERUSERID, pfnGetPlayerUserId, p, (e));
	RETURN_API(int)
}

void mm_BuildSoundMsg(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	META_ENGINE_HANDLE_void(FN_BUILDSOUNDMSG, pfnBuildSoundMsg, pip2f4i2p, (entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed));
	RETURN_API_void()
}

//! is this a dedicated server?
int mm_IsDedicatedServer()
{
	META_ENGINE_HANDLE(int, 0, FN_ISDEDICATEDSERVER, pfnIsDedicatedServer, void, (VOID_ARG));
	RETURN_API(int)
}

cvar_t *mm_CVarGetPointer(const char *szVarName)
{
	META_ENGINE_HANDLE(cvar_t *, NULL, FN_CVARGETPOINTER, pfnCVarGetPointer, p, (szVarName));
	RETURN_API(cvar_t *)
}

//! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
unsigned int mm_GetPlayerWONId(edict_t *e)
{
	META_ENGINE_HANDLE(unsigned int, 0, FN_GETPLAYERWONID, pfnGetPlayerWONId, p, (e));
	RETURN_API(unsigned int)
}

//! YWB 8/1/99 TFF Physics additions
void mm_Info_RemoveKey(char *s, const char *key)
{
	META_ENGINE_HANDLE_void(FN_INFO_REMOVEKEY, pfnInfo_RemoveKey, 2p, (s, key));
	RETURN_API_void()
}

const char *mm_GetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_GETPHYSICSKEYVALUE, pfnGetPhysicsKeyValue, 2p, (pClient, key));
	RETURN_API(const char *)
}

void mm_SetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
	META_ENGINE_HANDLE_void(FN_SETPHYSICSKEYVALUE, pfnSetPhysicsKeyValue, 3p, (pClient, key, value));
	RETURN_API_void()
}

const char *mm_GetPhysicsInfoString(const edict_t *pClient)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_GETPHYSICSINFOSTRING, pfnGetPhysicsInfoString, p, (pClient));
	RETURN_API(const char *)
}

unsigned short mm_PrecacheEvent(int type, const char *psz)
{
	META_ENGINE_HANDLE(unsigned short, 0, FN_PRECACHEEVENT, pfnPrecacheEvent, ip, (type, psz));
	RETURN_API(unsigned short)
}

void mm_PlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
	META_ENGINE_HANDLE_void(FN_PLAYBACKEVENT, pfnPlaybackEvent, ipusf2p2f4i, (flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2));
	RETURN_API_void()
}

unsigned char *mm_SetFatPVS(float *org)
{
	META_ENGINE_HANDLE(unsigned char *, 0, FN_SETFATPVS, pfnSetFatPVS, p, (org));
	RETURN_API(unsigned char *)
}

unsigned char *mm_SetFatPAS(float *org)
{
	META_ENGINE_HANDLE(unsigned char *, 0, FN_SETFATPAS, pfnSetFatPAS, p, (org));
	RETURN_API(unsigned char *)
}

int mm_CheckVisibility(edict_t *entity, unsigned char *pset)
{
	META_ENGINE_HANDLE(int, 0, FN_CHECKVISIBILITY, pfnCheckVisibility, 2p, (entity, pset));
	RETURN_API(int)
}

void mm_DeltaSetField(struct delta_s *pFields, const char *fieldname)
{
	META_ENGINE_HANDLE_void(FN_DELTASETFIELD, pfnDeltaSetField, 2p, (pFields, fieldname));
	RETURN_API_void()
}

void mm_DeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
	META_ENGINE_HANDLE_void(FN_DELTAUNSETFIELD, pfnDeltaUnsetField, 2p, (pFields, fieldname));
	RETURN_API_void()
}

void mm_DeltaAddEncoder(char *name, void (*conditionalencode)(struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
	META_ENGINE_HANDLE_void(FN_DELTAADDENCODER, pfnDeltaAddEncoder, 2p, (name, (void*)conditionalencode));
	RETURN_API_void()
}

int mm_GetCurrentPlayer(void)
{
	META_ENGINE_HANDLE(int, 0, FN_GETCURRENTPLAYER, pfnGetCurrentPlayer, void, (VOID_ARG));
	RETURN_API(int)
}

int mm_CanSkipPlayer(const edict_t *player)
{
	META_ENGINE_HANDLE(int, 0, FN_CANSKIPPLAYER, pfnCanSkipPlayer, p, (player));
	RETURN_API(int)
}

int mm_DeltaFindField(struct delta_s *pFields, const char *fieldname)
{
	META_ENGINE_HANDLE(int, 0, FN_DELTAFINDFIELD, pfnDeltaFindField, 2p, (pFields, fieldname));
	RETURN_API(int)
}

void mm_DeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	META_ENGINE_HANDLE_void(FN_DELTASETFIELDBYINDEX, pfnDeltaSetFieldByIndex, pi, (pFields, fieldNumber));
	RETURN_API_void()
}

void mm_DeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	META_ENGINE_HANDLE_void(FN_DELTAUNSETFIELDBYINDEX, pfnDeltaUnsetFieldByIndex, pi, (pFields, fieldNumber));
	RETURN_API_void()
}

void mm_SetGroupMask(int mask, int op)
{
	META_ENGINE_HANDLE_void(FN_SETGROUPMASK, pfnSetGroupMask, 2i, (mask, op));
	RETURN_API_void()
}

int mm_engCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
	META_ENGINE_HANDLE(int, 0, FN_CREATEINSTANCEDBASELINE, pfnCreateInstancedBaseline, ip, (classname, baseline));
	RETURN_API(int)
}

void mm_Cvar_DirectSet(struct cvar_s *var, const char *value)
{
	META_ENGINE_HANDLE_void(FN_CVAR_DIRECTSET, pfnCvar_DirectSet, 2p, (var, value));

	meta_debug_value = (int)meta_debug.value;

	RETURN_API_void()
}

//! Forces the client and server to be running with the same version of the specified file
//!(e.g., a player model).
//! Calling this has no effect in single player
void mm_ForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
	META_ENGINE_HANDLE_void(FN_FORCEUNMODIFIED, pfnForceUnmodified, i3p, (type, mins, maxs, filename));
	RETURN_API_void()
}

void mm_GetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
	META_ENGINE_HANDLE_void(FN_GETPLAYERSTATS, pfnGetPlayerStats, 3p, (pClient, ping, packet_loss));
	RETURN_API_void()
}

void mm_AddServerCommand(char *cmd_name, void (*function) ())
{
	META_ENGINE_HANDLE_void(FN_ADDSERVERCOMMAND, pfnAddServerCommand, 2p, (cmd_name, (void*)function));
	RETURN_API_void()
}

//! For voice communications, set which clients hear eachother.
//! NOTE: these functions take player entity indices (starting at 1).
qboolean mm_Voice_GetClientListening(int iReceiver, int iSender)
{
	META_ENGINE_HANDLE(qboolean, false, FN_VOICE_GETCLIENTLISTENING, pfnVoice_GetClientListening, 2i, (iReceiver, iSender));
	RETURN_API(qboolean)
}

qboolean mm_Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen)
{
	META_ENGINE_HANDLE(qboolean, false, FN_VOICE_SETCLIENTLISTENING, pfnVoice_SetClientListening, 3i, (iReceiver, iSender, bListen));
	RETURN_API(qboolean)
}

const char *mm_GetPlayerAuthId(edict_t *e)
{
	META_ENGINE_HANDLE(const char *, NULL, FN_GETPLAYERAUTHID, pfnGetPlayerAuthId, p, (e));
	RETURN_API(const char *)
}

sequenceEntry_s *mm_SequenceGet(const char *fileName, const char *entryName)
{
	META_ENGINE_HANDLE(sequenceEntry_s *, NULL, FN_SEQUENCEGET, pfnSequenceGet, 2p, (fileName, entryName));
	RETURN_API(sequenceEntry_s *)
}

sentenceEntry_s *mm_SequencePickSentence(const char *groupName, int pickMethod, int *picked)
{
	META_ENGINE_HANDLE(sentenceEntry_s *, NULL, FN_SEQUENCEPICKSENTENCE, pfnSequencePickSentence, pip, (groupName, pickMethod, picked));
	RETURN_API(sentenceEntry_s *)
}

int mm_GetFileSize(char *filename)
{
	META_ENGINE_HANDLE(int, 0, FN_GETFILESIZE, pfnGetFileSize, p, (filename));
	RETURN_API(int)
}

unsigned int mm_GetApproxWavePlayLen(const char *filepath)
{
	META_ENGINE_HANDLE(unsigned int, 0, FN_GETAPPROXWAVEPLAYLEN, pfnGetApproxWavePlayLen, p, (filepath));
	RETURN_API(unsigned int)
}

int mm_IsCareerMatch()
{
	META_ENGINE_HANDLE(int, 0, FN_ISCAREERMATCH, pfnIsCareerMatch, void, (VOID_ARG));
	RETURN_API(int)
}

int mm_GetLocalizedStringLength(const char *label)
{
	META_ENGINE_HANDLE(int, 0, FN_GETLOCALIZEDSTRINGLENGTH, pfnGetLocalizedStringLength, p, (label));
	RETURN_API(int)
}

void mm_RegisterTutorMessageShown(int mid)
{
	META_ENGINE_HANDLE_void(FN_REGISTERTUTORMESSAGESHOWN, pfnRegisterTutorMessageShown, i, (mid));
	RETURN_API_void()
}

int mm_GetTimesTutorMessageShown(int mid)
{
	META_ENGINE_HANDLE(int, 0, FN_GETTIMESTUTORMESSAGESHOWN, pfnGetTimesTutorMessageShown, i, (mid));
	RETURN_API(int)
}

void mm_ProcessTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	META_ENGINE_HANDLE_void(FN_PROCESSTUTORMESSAGEDECAYBUFFER, pfnProcessTutorMessageDecayBuffer, pi, (buffer, bufferLength));
	RETURN_API_void()
}

void mm_ConstructTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	META_ENGINE_HANDLE_void(FN_CONSTRUCTTUTORMESSAGEDECAYBUFFER, pfnConstructTutorMessageDecayBuffer, pi, (buffer, bufferLength));
	RETURN_API_void()
}

void mm_ResetTutorMessageDecayData()
{
	META_ENGINE_HANDLE_void(FN_RESETTUTORMESSAGEDECAYDATA, pfnResetTutorMessageDecayData, void, (VOID_ARG));
	RETURN_API_void()
}

void mm_QueryClientCvarValue(const edict_t *player, const char *cvarName)
{
	META_ENGINE_HANDLE_void(FN_QUERYCLIENTCVARVALUE, pfnQueryClientCvarValue, 2p, (player, cvarName));
	RETURN_API_void()
}

void mm_QueryClientCvarValue2(const edict_t *player, const char *cvarName, int requestID)
{
	META_ENGINE_HANDLE_void(FN_QUERYCLIENTCVARVALUE2, pfnQueryClientCvarValue2, 2pi, (player, cvarName, requestID));
	RETURN_API_void()
}

int mm_EngCheckParm(const char *pchCmdLineToken, char **pchNextVal)
{
	META_ENGINE_HANDLE(int, 0, FN_ENGCHECKPARM, pfnEngCheckParm, 2p, (pchCmdLineToken, pchNextVal));
	RETURN_API(int)
}

meta_enginefuncs_t meta_engfuncs(
	&mm_PrecacheModel,			// pfnPrecacheModel()
	&mm_PrecacheSound,			// pfnPrecacheSound()
	&mm_SetModel,				// pfnSetModel()
	&mm_ModelIndex,				// pfnModelIndex()
	&mm_ModelFrames,			// pfnModelFrames()

	&mm_SetSize,				// pfnSetSize()
	&mm_ChangeLevel,			// pfnChangeLevel()
	&mm_GetSpawnParms,			// pfnGetSpawnParms()
	&mm_SaveSpawnParms,			// pfnSaveSpawnParms()

	&mm_VecToYaw,				// pfnVecToYaw()
	&mm_VecToAngles,			// pfnVecToAngles()
	&mm_MoveToOrigin,			// pfnMoveToOrigin()
	&mm_ChangeYaw,				// pfnChangeYaw()
	&mm_ChangePitch,			// pfnChangePitch()

	&mm_FindEntityByString,			// pfnFindEntityByString()
	&mm_GetEntityIllum,			// pfnGetEntityIllum()
	&mm_FindEntityInSphere,			// pfnFindEntityInSphere()
	&mm_FindClientInPVS,			// pfnFindClientInPVS()
	&mm_EntitiesInPVS,			// pfnEntitiesInPVS()

	&mm_MakeVectors,			// pfnMakeVectors()
	&mm_AngleVectors,			// pfnAngleVectors()

	&mm_CreateEntity,			// pfnCreateEntity()
	&mm_RemoveEntity,			// pfnRemoveEntity()
	&mm_CreateNamedEntity,			// pfnCreateNamedEntity()

	&mm_MakeStatic,				// pfnMakeStatic()
	&mm_EntIsOnFloor,			// pfnEntIsOnFloor()
	&mm_DropToFloor,			// pfnDropToFloor()

	&mm_WalkMove,				// pfnWalkMove()
	&mm_SetOrigin,				// pfnSetOrigin()

	&mm_EmitSound,				// pfnEmitSound()
	&mm_EmitAmbientSound,			// pfnEmitAmbientSound()

	&mm_TraceLine,				// pfnTraceLine()
	&mm_TraceToss,				// pfnTraceToss()
	&mm_TraceMonsterHull,			// pfnTraceMonsterHull()
	&mm_TraceHull,				// pfnTraceHull()
	&mm_TraceModel,				// pfnTraceModel()
	&mm_TraceTexture,			// pfnTraceTexture()
	&mm_TraceSphere,			// pfnTraceSphere()
	&mm_GetAimVector,			// pfnGetAimVector()

	&mm_ServerCommand,			// pfnServerCommand()
	&mm_ServerExecute,			// pfnServerExecute()
	&mm_engClientCommand,			// pfnClientCommand()	// D'oh, ClientCommand in dllapi too.

	&mm_ParticleEffect,			// pfnParticleEffect()
	&mm_LightStyle,				// pfnLightStyle()
	&mm_DecalIndex,				// pfnDecalIndex()
	&mm_PointContents,			// pfnPointContents()

	&mm_MessageBegin,			// pfnMessageBegin()
	&mm_MessageEnd,				// pfnMessageEnd()

	&mm_WriteByte,				// pfnWriteByte()
	&mm_WriteChar,				// pfnWriteChar()
	&mm_WriteShort,				// pfnWriteShort()
	&mm_WriteLong,				// pfnWriteLong()
	&mm_WriteAngle,				// pfnWriteAngle()
	&mm_WriteCoord,				// pfnWriteCoord()
	&mm_WriteString,			// pfnWriteString()
	&mm_WriteEntity,			// pfnWriteEntity()

	&mm_CVarRegister,			// pfnCVarRegister()
	&mm_CVarGetFloat,			// pfnCVarGetFloat()
	&mm_CVarGetString,			// pfnCVarGetString()
	&mm_CVarSetFloat,			// pfnCVarSetFloat()
	&mm_CVarSetString,			// pfnCVarSetString()

	&mm_AlertMessage,			// pfnAlertMessage()
	&mm_EngineFprintf,			// pfnEngineFprintf()

	&mm_PvAllocEntPrivateData,		// pfnPvAllocEntPrivateData()
	&mm_PvEntPrivateData,			// pfnPvEntPrivateData()
	&mm_FreeEntPrivateData,			// pfnFreeEntPrivateData()

	&mm_SzFromIndex,			// pfnSzFromIndex()
	&mm_AllocString,			// pfnAllocString()

	&mm_GetVarsOfEnt, 			// pfnGetVarsOfEnt()
	&mm_PEntityOfEntOffset,			// pfnPEntityOfEntOffset()
	&mm_EntOffsetOfPEntity,			// pfnEntOffsetOfPEntity()
	&mm_IndexOfEdict,			// pfnIndexOfEdict()
	&mm_PEntityOfEntIndex,			// pfnPEntityOfEntIndex()
	&mm_FindEntityByVars,			// pfnFindEntityByVars()
	&mm_GetModelPtr,			// pfnGetModelPtr()

	&mm_RegUserMsg,				// pfnRegUserMsg()

	&mm_AnimationAutomove,			// pfnAnimationAutomove()
	&mm_GetBonePosition,			// pfnGetBonePosition()

	&mm_FunctionFromName,			// pfnFunctionFromName()
	&mm_NameForFunction,			// pfnNameForFunction()

	&mm_ClientPrintf,			// pfnClientPrintf()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	&mm_ServerPrint,			// pfnServerPrint()

	&mm_Cmd_Args,				// pfnCmd_Args()				//! these 3 added
	&mm_Cmd_Argv,				// pfnCmd_Argv()				//! so game DLL can easily
	&mm_Cmd_Argc,				// pfnCmd_Argc()				//! access client 'cmd' strings

	&mm_GetAttachment,			// pfnGetAttachment()

	&mm_CRC32_Init,				// pfnCRC32_Init()
	&mm_CRC32_ProcessBuffer,		// pfnCRC32_ProcessBuffer()
	&mm_CRC32_ProcessByte,			// pfnCRC32_ProcessByte()
	&mm_CRC32_Final,			// pfnCRC32_Final()

	&mm_RandomLong,				// pfnRandomLong()
	&mm_RandomFloat,			// pfnRandomFloat()

	&mm_SetView,				// pfnSetView()
	&mm_Time,				// pfnTime()
	&mm_CrosshairAngle,			// pfnCrosshairAngle()

	&mm_LoadFileForMe,			// pfnLoadFileForMe()
	&mm_FreeFile,				// pfnFreeFile()

	&mm_EndSection,				// pfnEndSection()				//! trigger_endsection
	&mm_CompareFileTime,			// pfnCompareFileTime()
	&mm_GetGameDir,				// pfnGetGameDir()
	&mm_Cvar_RegisterVariable,		// pfnCvar_RegisterVariable()
	&mm_FadeClientVolume,			// pfnFadeClientVolume()
	&mm_SetClientMaxspeed,			// pfnSetClientMaxspeed()
	&mm_CreateFakeClient,			// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	&mm_RunPlayerMove,			// pfnRunPlayerMove()
	&mm_NumberOfEntities,			// pfnNumberOfEntities()

	&mm_GetInfoKeyBuffer,			// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	&mm_InfoKeyValue,			// pfnInfoKeyValue()
	&mm_SetKeyValue,			// pfnSetKeyValue()
	&mm_SetClientKeyValue,			// pfnSetClientKeyValue()

	&mm_IsMapValid,				// pfnIsMapValid()
	&mm_StaticDecal,			// pfnStaticDecal()
	&mm_PrecacheGeneric,			// pfnPrecacheGeneric()
	&mm_GetPlayerUserId, 			// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	&mm_BuildSoundMsg,			// pfnBuildSoundMsg()
	&mm_IsDedicatedServer,			// pfnIsDedicatedServer()		//! is this a dedicated server?
	&mm_CVarGetPointer,			// pfnCVarGetPointer()
	&mm_GetPlayerWONId,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	&mm_Info_RemoveKey,			// pfnInfo_RemoveKey()
	&mm_GetPhysicsKeyValue,			// pfnGetPhysicsKeyValue()
	&mm_SetPhysicsKeyValue,			// pfnSetPhysicsKeyValue()
	&mm_GetPhysicsInfoString,		// pfnGetPhysicsInfoString()
	&mm_PrecacheEvent,			// pfnPrecacheEvent()
	&mm_PlaybackEvent,			// pfnPlaybackEvent()
	&mm_SetFatPVS,				// pfnSetFatPVS()
	&mm_SetFatPAS,				// pfnSetFatPAS()
	&mm_CheckVisibility,			// pfnCheckVisibility()
	&mm_DeltaSetField,			// pfnDeltaSetField()
	&mm_DeltaUnsetField,			// pfnDeltaUnsetField()
	&mm_DeltaAddEncoder,			// pfnDeltaAddEncoder()
	&mm_GetCurrentPlayer,			// pfnGetCurrentPlayer()
	&mm_CanSkipPlayer,			// pfnCanSkipPlayer()
	&mm_DeltaFindField,			// pfnDeltaFindField()
	&mm_DeltaSetFieldByIndex,		// pfnDeltaSetFieldByIndex()
	&mm_DeltaUnsetFieldByIndex,		// pfnDeltaUnsetFieldByIndex()

	&mm_SetGroupMask,			// pfnSetGroupMask()

	&mm_engCreateInstancedBaseline,		// pfnCreateInstancedBaseline()		// D'oh, CreateInstancedBaseline in dllapi too.
	&mm_Cvar_DirectSet,			// pfnCvar_DirectSet()

	&mm_ForceUnmodified,			// pfnForceUnmodified()
	&mm_GetPlayerStats,			// pfnGetPlayerStats()

	&mm_AddServerCommand,			// pfnAddServerCommand()

	&mm_Voice_GetClientListening,		// pfnVoice_GetClientListening()
	&mm_Voice_SetClientListening,		// pfnVoice_SetClientListening()

	&mm_GetPlayerAuthId,			// pfnGetPlayerAuthId()

	&mm_SequenceGet,			// pfnSequenceGet()
	&mm_SequencePickSentence,		// pfnSequencePickSentence()
	&mm_GetFileSize,			// pfnGetFileSize()
	&mm_GetApproxWavePlayLen,		// pfnGetApproxWavePlayLen()
	&mm_IsCareerMatch,			// pfnIsCareerMatch()
	&mm_GetLocalizedStringLength,		// pfnGetLocalizedStringLength()
	&mm_RegisterTutorMessageShown,		// pfnRegisterTutorMessageShown()
	&mm_GetTimesTutorMessageShown,		// pfnGetTimesTutorMessageShown()
	&mm_ProcessTutorMessageDecayBuffer,	// pfnProcessTutorMessageDecayBuffer()
	&mm_ConstructTutorMessageDecayBuffer,	// pfnConstructTutorMessageDecayBuffer()
	&mm_ResetTutorMessageDecayData,		// pfnResetTutorMessageDecayData()

	&mm_QueryClientCvarValue,		// pfnQueryClientCvarValue()
	&mm_QueryClientCvarValue2,		// pfnQueryClientCvarValue2()
	&mm_EngCheckParm			// pfnEngCheckParm()
);
