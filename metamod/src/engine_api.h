#pragma once

#include <archtypes.h>

// Plugin's GetEngineFunctions, called by metamod.
typedef int (*GET_ENGINE_FUNCTIONS_FN)(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion);

// According to SDK engine/eiface.h:
// ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT. INTERFACE VERSION IS FROZEN AT 138
#define ENGINE_INTERFACE_VERSION 138

// Protect against other projects which use this include file but use the
// normal enginefuncs_t type for their meta_engfuncs.
#ifdef METAMOD_CORE
	#include "meta_eiface.h"	// meta_enginefuncs_t
	extern meta_enginefuncs_t meta_engfuncs;
#else
	extern enginefuncs_t meta_engfuncs;
#endif

extern int mm_PrecacheModel(const char *s);
extern int mm_PrecacheSound(const char *s);
extern void mm_SetModel(edict_t *e, const char *m);
extern int mm_ModelIndex(const char *m);
extern int mm_ModelFrames(int modelIndex);

extern void mm_SetSize(edict_t *e, const float *rgflMin, const float *rgflMax);
extern void mm_ChangeLevel(const char *s1, const char *s2);
extern void mm_GetSpawnParms(edict_t *ent);
extern void mm_SaveSpawnParms(edict_t *ent);

extern float mm_VecToYaw(const float *rgflVector);
extern void mm_VecToAngles(const float *rgflVectorIn, float *rgflVectorOut);
extern void mm_MoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
extern void mm_ChangeYaw(edict_t *ent);
extern void mm_ChangePitch(edict_t *ent);

extern edict_t *mm_FindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
extern int mm_GetEntityIllum(edict_t *pEnt);
extern edict_t *mm_FindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad);
extern edict_t *mm_FindClientInPVS(edict_t *pEdict);
extern edict_t *mm_EntitiesInPVS(edict_t *pplayer);

extern void mm_MakeVectors(const float *rgflVector);
extern void mm_AngleVectors(const float *rgflVector, float *forward, float *right, float *up);

extern edict_t *mm_CreateEntity();
extern void mm_RemoveEntity(edict_t *e);
extern edict_t *mm_CreateNamedEntity(int className);

extern void mm_MakeStatic(edict_t *ent);
extern int mm_EntIsOnFloor(edict_t *e);
extern int mm_DropToFloor(edict_t *e);

extern int mm_WalkMove(edict_t *ent, float yaw, float dist, int iMode);
extern void mm_SetOrigin(edict_t *e, const float *rgflOrigin);

extern void mm_EmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
extern void mm_EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);

extern void mm_TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
extern void mm_TraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
extern int mm_TraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
extern void mm_TraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
extern void mm_TraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
extern const char *mm_TraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2);
extern void mm_TraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
extern void mm_GetAimVector(edict_t *ent, float speed, float *rgflReturn);

extern void mm_ServerCommand(const char *str);
extern void mm_ServerExecute();
extern void ClientCommand(edict_t *pEdict, const char *szFmt, ...);

extern void mm_ParticleEffect(const float *org, const float *dir, float color, float count);
extern void mm_LightStyle(int style, const char *val);
extern int mm_DecalIndex(const char *name);
extern int mm_PointContents(const float *rgflVector);

extern void mm_MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
extern void mm_MessageEnd();

extern void mm_WriteByte(int iValue);
extern void mm_WriteChar(int iValue);
extern void mm_WriteShort(int iValue);
extern void mm_WriteLong(int iValue);
extern void mm_WriteAngle(float flValue);
extern void mm_WriteCoord(float flValue);
extern void mm_WriteString(const char *sz);
extern void mm_WriteEntity(int iValue);

extern void mm_CVarRegister(cvar_t *pCvar);
extern float mm_CVarGetFloat(const char *szVarName);
extern const char *mm_CVarGetString(const char *szVarName);
extern void mm_CVarSetFloat(const char *szVarName, float flValue);
extern void mm_CVarSetString(const char *szVarName, const char *szValue);

extern void mm_AlertMessage(ALERT_TYPE atype, const char *szFmt, ...);
extern void mm_EngineFprintf(void *pfile, const char *szFmt, ...);
extern void *mm_PvAllocEntPrivateData(edict_t *pEdict, int32 cb);
extern void *mm_PvEntPrivateData(edict_t *pEdict);
extern void mm_FreeEntPrivateData(edict_t *pEdict);

extern const char *mm_SzFromIndex(int iString);
extern int mm_AllocString(const char *szValue);

extern struct entvars_s *mm_GetVarsOfEnt(edict_t *pEdict);
extern edict_t *mm_PEntityOfEntOffset(int iEntOffset);
extern int mm_EntOffsetOfPEntity(const edict_t *pEdict);
extern int mm_IndexOfEdict(const edict_t *pEdict);
extern edict_t *mm_PEntityOfEntIndex(int iEntIndex);
extern edict_t *mm_FindEntityByVars(struct entvars_s *pvars);
extern void *mm_GetModelPtr(edict_t *pEdict);

extern int mm_RegUserMsg(const char *pszName, int iSize);

extern void mm_AnimationAutomove(const edict_t *pEdict, float flTime);
extern void mm_GetBonePosition(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles);

extern uint32 mm_FunctionFromName(const char *pName);
extern const char *mm_NameForFunction(uint32 function);

extern void mm_ClientPrintf(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg); //! JOHN: engine callbacks so game DLL can print messages to individual clients
extern void mm_ServerPrint(const char *szMsg);

extern const char *mm_Cmd_Args(); //! these 3 added
extern const char *mm_Cmd_Argv(int argc); //! so game DLL can easily
extern int mm_Cmd_Argc(); //! access client 'cmd' strings

extern void mm_GetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles);

extern void mm_CRC32_Init(CRC32_t *pulCRC);
extern void mm_CRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len);
extern void mm_CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch);
extern CRC32_t mm_CRC32_Final(CRC32_t pulCRC);

extern int32 mm_RandomLong(int32 lLow, int32 lHigh);
extern float mm_RandomFloat(float flLow, float flHigh);

extern void mm_SetView(const edict_t *pClient, const edict_t *pViewent);
extern float mm_Time();
extern void mm_CrosshairAngle(const edict_t *pClient, float pitch, float yaw);

extern byte *mm_LoadFileForMe(const char *filename, int *pLength);
extern void mm_FreeFile(void *buffer);

extern void mm_EndSection(const char *pszSectionName); //! trigger_endsection
extern int mm_CompareFileTime(char *filename1, char *filename2, int *iCompare);
extern void mm_GetGameDir(char *szGetGameDir);
extern void mm_Cvar_RegisterVariable(cvar_t *variable);
extern void mm_FadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
extern void mm_SetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed);
extern edict_t *mm_CreateFakeClient(const char *netname); //! returns NULL if fake client can't be created
extern void mm_RunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
extern int mm_NumberOfEntities();

extern char *mm_GetInfoKeyBuffer(edict_t *e); //! passing in NULL gets the serverinfo
extern char *mm_InfoKeyValue(char *infobuffer, const char *key);
extern void mm_SetKeyValue(char *infobuffer, const char *key, const char *value);
extern void mm_SetClientKeyValue(int clientIndex, char *infobuffer, const char *key, const char *value);

extern int mm_IsMapValid(char *filename);
extern void mm_StaticDecal(const float *origin, int decalIndex, int entityIndex, int modelIndex);
extern int mm_PrecacheGeneric(const char *s);
extern int mm_GetPlayerUserId(edict_t *e); //! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
extern void mm_BuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
extern int mm_IsDedicatedServer();//! is this a dedicated server?
extern cvar_t *mm_CVarGetPointer(const char *szVarName);
extern unsigned int mm_GetPlayerWONId(edict_t *e); //! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients

// 8/1/99 TFF Physics additions
extern void mm_Info_RemoveKey(char *s, const char *key);
extern const char *mm_GetPhysicsKeyValue(const edict_t *pClient, const char *key);
extern void mm_SetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value);
extern const char *mm_GetPhysicsInfoString(const edict_t *pClient);
extern unsigned short mm_PrecacheEvent(int type, const char *psz);
extern void mm_PlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
extern unsigned char *mm_SetFatPVS(float *org);
extern unsigned char *mm_SetFatPAS(float *org);
extern int mm_CheckVisibility(edict_t *entity, unsigned char *pset);

extern void mm_DeltaSetField(struct delta_s *pFields, const char *fieldname);
extern void mm_DeltaUnsetField(struct delta_s *pFields, const char *fieldname);
extern void mm_DeltaAddEncoder(const char *name, void (*conditionalencode)(struct delta_s *pFields, const unsigned char *from, const unsigned char *to));
extern int mm_GetCurrentPlayer();
extern int mm_CanSkipPlayer(const edict_t *player);
extern int mm_DeltaFindField(struct delta_s *pFields, const char *fieldname);
extern void mm_DeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber);
extern void mm_DeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber);
extern void mm_SetGroupMask(int mask, int op);
extern int CreateInstancedBaseline(int classname, struct entity_state_s *baseline);
extern void mm_Cvar_DirectSet(struct cvar_s *var, const char *value);

// Forces the client and server to be running with the same version of the specified file e.g., a player model).
// Calling this has no effect in single player
extern void mm_ForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename);
extern void mm_GetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss);

extern void mm_AddServerCommand(const char *cmd_name, void (*function)());
extern qboolean mm_Voice_GetClientListening(int iReceiver, int iSender);
extern qboolean mm_Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen);
extern const char *mm_pfnGetPlayerAuthId(edict_t *e);
extern sequenceEntry_s *mm_SequenceGet(const char *fileName, const char *entryName);
extern sentenceEntry_s *mm_SequencePickSentence(const char *groupName, int pickMethod, int *picked);
extern int mm_GetFileSize(const char *filename);
extern unsigned int mm_GetApproxWavePlayLen(const char *filepath);
extern int mm_IsCareerMatch();
extern int mm_GetLocalizedStringLength(const char *label);
extern void mm_RegisterTutorMessageShown(int mid);
extern int mm_GetTimesTutorMessageShown(int mid);
extern void mm_ProcessTutorMessageDecayBuffer(int *buffer, int bufferLength);
extern void mm_ConstructTutorMessageDecayBuffer(int *buffer, int bufferLength);
extern void mm_ResetTutorMessageDecayData();

extern void mm_QueryClientCvarValue(const edict_t *pEdict, const char *cvarName);	//! Obsolete! Use mm_QueryClientCvarValue2 instead
extern void mm_QueryClientCvarValue2(const edict_t *pEdict, const char *cvarName, int requestID);
extern int mm_EngCheckParm(const char *pchCmdLineToken, char **ppnext);

// Typedefs for the above functions:
typedef int (*FN_PRECACHEMODEL)(const char *s);
typedef int (*FN_PRECACHESOUND)(const char *s);
typedef void (*FN_SETMODEL)(edict_t *e, const char *m);
typedef int (*FN_MODELINDEX)(const char *m);
typedef int (*FN_MODELFRAMES)(int modelIndex);
typedef void (*FN_SETSIZE)(edict_t *e, const float *rgflMin, const float *rgflMax);
typedef void (*FN_CHANGELEVEL)(const char *s1, const char *s2);
typedef void (*FN_GETSPAWNPARMS)(edict_t *ent);
typedef void (*FN_SAVESPAWNPARMS)(edict_t *ent);
typedef float (*FN_VECTOYAW)(const float *rgflVector);
typedef void (*FN_VECTOANGLES)(const float *rgflVectorIn, float *rgflVectorOut);
typedef void (*FN_MOVETOORIGIN)(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
typedef void (*FN_CHANGEYAW)(edict_t *ent);
typedef void (*FN_CHANGEPITCH)(edict_t *ent);
typedef edict_t *(*FN_FINDENTITYBYSTRING)(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
typedef int (*FN_GETENTITYILLUM)(edict_t *pEnt);
typedef edict_t *(*FN_FINDENTITYINSPHERE)(edict_t *pEdictStartSearchAfter, const float *org, float rad);
typedef edict_t *(*FN_FINDCLIENTINPVS)(edict_t *pEdict);
typedef edict_t *(*FN_ENTITIESINPVS)(edict_t *pplayer);
typedef void (*FN_MAKEVECTORS)(const float *rgflVector);
typedef void (*FN_ANGLEVECTORS)(const float *rgflVector, float *forward, float *right, float *up);
typedef edict_t *(*FN_CREATEENTITY)();
typedef void (*FN_REMOVEENTITY)(edict_t *e);
typedef edict_t *(*FN_CREATENAMEDENTITY)(int className);
typedef void (*FN_MAKESTATIC)(edict_t *ent);
typedef int (*FN_ENTISONFLOOR)(edict_t *e);
typedef int (*FN_DROPTOFLOOR)(edict_t *e);
typedef int (*FN_WALKMOVE)(edict_t *ent, float yaw, float dist, int iMode);
typedef void (*FN_SETORIGIN)(edict_t *e, const float *rgflOrigin);
typedef void (*FN_EMITSOUND)(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
typedef void (*FN_EMITAMBIENTSOUND)(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
typedef void (*FN_TRACELINE)(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACETOSS)(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
typedef int (*FN_TRACEMONSTERHULL)(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACEHULL)(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACEMODEL)(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
typedef const char *(*FN_TRACETEXTURE)(edict_t *pTextureEntity, const float *v1, const float *v2);
typedef void (*FN_TRACESPHERE)(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_GETAIMVECTOR)(edict_t *ent, float speed, float *rgflReturn);
typedef void (*FN_SERVERCOMMAND)(char *str);
typedef void (*FN_SERVEREXECUTE)();
typedef void (*FN_CLIENTCOMMAND_ENG)(edict_t *pEdict, char *szFmt, ...);
typedef void (*FN_PARTICLEEFFECT)(const float *org, const float *dir, float color, float count);
typedef void (*FN_LIGHTSTYLE)(int style, char *val);
typedef int (*FN_DECALINDEX)(const char *name);
typedef int (*FN_POINTCONTENTS)(const float *rgflVector);
typedef void (*FN_MESSAGEBEGIN)(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
typedef void (*FN_MESSAGEEND)();
typedef void (*FN_WRITEBYTE)(int iValue);
typedef void (*FN_WRITECHAR)(int iValue);
typedef void (*FN_WRITESHORT)(int iValue);
typedef void (*FN_WRITELONG)(int iValue);
typedef void (*FN_WRITEANGLE)(float flValue);
typedef void (*FN_WRITECOORD)(float flValue);
typedef void (*FN_WRITESTRING)(const char *sz);
typedef void (*FN_WRITEENTITY)(int iValue);
typedef void (*FN_CVARREGISTER)(cvar_t *pCvar);
typedef float (*FN_CVARGETFLOAT)(const char *szVarName);
typedef const char *(*FN_CVARGETSTRING)(const char *szVarName);
typedef void (*FN_CVARSETFLOAT)(const char *szVarName, float flValue);
typedef void (*FN_CVARSETSTRING)(const char *szVarName, const char *szValue);
typedef void (*FN_ALERTMESSAGE)(ALERT_TYPE atype, const char *szFmt, ...);
typedef void (*FN_ENGINEFPRINTF)(void *pfile, const char *szFmt, ...);
typedef void *(*FN_PVALLOCENTPRIVATEDATA)(edict_t *pEdict, int32 cb);
typedef void *(*FN_PVENTPRIVATEDATA)(edict_t *pEdict);
typedef void (*FN_FREEENTPRIVATEDATA)(edict_t *pEdict);
typedef const char *(*FN_SZFROMINDEX)(int iString);
typedef int (*FN_ALLOCSTRING)(const char *szValue);
typedef struct entvars_s *(*FN_GETVARSOFENT)(edict_t *pEdict);
typedef edict_t *(*FN_PENTITYOFENTOFFSET)(int iEntOffset);
typedef int (*FN_ENTOFFSETOFPENTITY)(const edict_t *pEdict);
typedef int (*FN_INDEXOFEDICT)(const edict_t *pEdict);
typedef edict_t *(*FN_PENTITYOFENTINDEX)(int iEntIndex);
typedef edict_t *(*FN_FINDENTITYBYVARS)(struct entvars_s *pvars);
typedef void *(*FN_GETMODELPTR)(edict_t *pEdict);
typedef int (*FN_REGUSERMSG)(const char *pszName, int iSize);
typedef void (*FN_ANIMATIONAUTOMOVE)(const edict_t *pEdict, float flTime);
typedef void (*FN_GETBONEPOSITION)(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles);
typedef uint32 (*FN_FUNCTIONFROMNAME)(const char *pName);
typedef const char *(*FN_NAMEFORFUNCTION)(uint32 function);
typedef void (*FN_CLIENTPRINTF)(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg);
typedef void (*FN_SERVERPRINT)(const char *szMsg);
typedef const char *(*FN_CMD_ARGS)();
typedef const char *(*FN_CMD_ARGV)(int argc);
typedef int (*FN_CMD_ARGC)();
typedef void (*FN_GETATTACHMENT)(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles);
typedef void (*FN_CRC32_INIT)(CRC32_t *pulCRC);
typedef void (*FN_CRC32_PROCESSBUFFER)(CRC32_t *pulCRC, void *p, int len);
typedef void (*FN_CRC32_PROCESSBYTE)(CRC32_t *pulCRC, unsigned char ch);
typedef CRC32_t (*FN_CRC32_FINAL)(CRC32_t pulCRC);
typedef int32 (*FN_RANDOMLONG)(int32 lLow, int32 lHigh);
typedef float (*FN_RANDOMFLOAT)(float flLow, float flHigh);
typedef void (*FN_SETVIEW)(const edict_t *pClient, const edict_t *pViewent);
typedef float (*FN_TIME)();
typedef void (*FN_CROSSHAIRANGLE)(const edict_t *pClient, float pitch, float yaw);
typedef byte *(*FN_LOADFILEFORME)(char *filename, int *pLength);
typedef void (*FN_FREEFILE)(void *buffer);
typedef void (*FN_ENDSECTION)(const char *pszSectionName);
typedef int (*FN_COMPAREFILETIME)(char *filename1, char *filename2, int *iCompare);
typedef void (*FN_GETGAMEDIR)(char *szGetGameDir);
typedef void (*FN_CVAR_REGISTERVARIABLE)(cvar_t *variable);
typedef void (*FN_FADECLIENTVOLUME)(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
typedef void (*FN_SETCLIENTMAXSPEED)(edict_t *pEdict, float fNewMaxspeed);
typedef edict_t *(*FN_CREATEFAKECLIENT)(const char *netname);
typedef void (*FN_RUNPLAYERMOVE)(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
typedef int (*FN_NUMBEROFENTITIES)();
typedef char *(*FN_GETINFOKEYBUFFER)(edict_t *e);
typedef char *(*FN_INFOKEYVALUE)(char *infobuffer, const char *key);
typedef void (*FN_SETKEYVALUE)(char *infobuffer, const char *key, const char *value);
typedef void (*FN_SETCLIENTKEYVALUE)(int clientIndex, char *infobuffer, const char *key, const char *value);
typedef int (*FN_ISMAPVALID)(char *filename);
typedef void (*FN_STATICDECAL)(const float *origin, int decalIndex, int entityIndex, int modelIndex);
typedef int (*FN_PRECACHEGENERIC)(char *s);
typedef int (*FN_GETPLAYERUSERID)(edict_t *e);
typedef void (*FN_BUILDSOUNDMSG)(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
typedef int (*FN_ISDEDICATEDSERVER)();
typedef cvar_t *(*FN_CVARGETPOINTER)(const char *szVarName);
typedef unsigned int (*FN_GETPLAYERWONID)(edict_t *e);
typedef void (*FN_INFO_REMOVEKEY)(char *s, const char *key);
typedef const char *(*FN_GETPHYSICSKEYVALUE)(const edict_t *pClient, const char *key);
typedef void (*FN_SETPHYSICSKEYVALUE)(const edict_t *pClient, const char *key, const char *value);
typedef const char *(*FN_GETPHYSICSINFOSTRING)(const edict_t *pClient);
typedef unsigned short (*FN_PRECACHEEVENT)(int type, const char *psz);
typedef void (*FN_PLAYBACKEVENT)(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
typedef unsigned char *(*FN_SETFATPVS)(float *org);
typedef unsigned char *(*FN_SETFATPAS)(float *org);
typedef int (*FN_CHECKVISIBILITY)(edict_t *entity, unsigned char *pset);
typedef void (*FN_DELTASETFIELD)(struct delta_s *pFields, const char *fieldname);
typedef void (*FN_DELTAUNSETFIELD)(struct delta_s *pFields, const char *fieldname);
typedef void (*FN_DELTAADDENCODER)(char *name, void (*conditionalencode)(struct delta_s *pFields, const unsigned char *from, const unsigned char *to));
typedef int (*FN_GETCURRENTPLAYER)();
typedef int (*FN_CANSKIPPLAYER)(const edict_t *player);
typedef int (*FN_DELTAFINDFIELD)(struct delta_s *pFields, const char *fieldname);
typedef void (*FN_DELTASETFIELDBYINDEX)(struct delta_s *pFields, int fieldNumber);
typedef void (*FN_DELTAUNSETFIELDBYINDEX)(struct delta_s *pFields, int fieldNumber);
typedef void (*FN_SETGROUPMASK)(int mask, int op);
typedef int (*FN_CREATEINSTANCEDBASELINE)(int classname, struct entity_state_s *baseline);
typedef void (*FN_CVAR_DIRECTSET)(struct cvar_s *var, const char *value);
typedef void (*FN_FORCEUNMODIFIED)(FORCE_TYPE type, float *mins, float *maxs, const char *filename);
typedef void (*FN_GETPLAYERSTATS)(const edict_t *pClient, int *ping, int *packet_loss);
typedef void (*FN_ADDSERVERCOMMAND)(char *cmd_name, void (*function)());
typedef qboolean (*FN_VOICE_GETCLIENTLISTENING)(int iReceiver, int iSender);
typedef qboolean (*FN_VOICE_SETCLIENTLISTENING)(int iReceiver, int iSender, qboolean bListen);
typedef const char *(*FN_GETPLAYERAUTHID)(edict_t *e);
typedef sequenceEntry_s *(*FN_SEQUENCEGET)(const char *fileName, const char *entryName);
typedef sentenceEntry_s *(*FN_SEQUENCEPICKSENTENCE)(const char *groupName, int pickMethod, int *picked);
typedef int (*FN_GETFILESIZE)(char *filename);
typedef unsigned int (*FN_GETAPPROXWAVEPLAYLEN)(const char *filepath);
typedef int (*FN_ISCAREERMATCH)();
typedef int (*FN_GETLOCALIZEDSTRINGLENGTH)(const char *label);
typedef void (*FN_REGISTERTUTORMESSAGESHOWN)(int mid);
typedef int (*FN_GETTIMESTUTORMESSAGESHOWN)(int mid);
typedef void (*FN_PROCESSTUTORMESSAGEDECAYBUFFER)(int *buffer, int bufferLength);
typedef void (*FN_CONSTRUCTTUTORMESSAGEDECAYBUFFER)(int *buffer, int bufferLength);
typedef void (*FN_RESETTUTORMESSAGEDECAYDATA)();
typedef void (*FN_QUERYCLIENTCVARVALUE)(const edict_t *pEdict, const char *cvarName); // Use FN_QUERYCLIENTCVARVALUE2 instead
typedef void (*FN_QUERYCLIENTCVARVALUE2)(const edict_t *pEdict, const char *cvarName, int requestID);
typedef int (*FN_CHECKPARM)(const char *pchCmdLineToken, char **ppnext);
