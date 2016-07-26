#include "precompiled.h"

// meta_new_dll_functions_t
meta_new_dll_functions_t::meta_new_dll_functions_t(
	void (*_pfnOnFreeEntPrivateData)(edict_t *),
	void (*_pfnGameShutdown)        (),
	int  (*_pfnShouldCollide)       (edict_t *, edict_t *),
	void (*_pfnCvarValue)           (const edict_t *, const char *),
	void (*_pfnCvarValue2)          (const edict_t *, int, const char *, const char *))
{
	pfnOnFreeEntPrivateData = _pfnOnFreeEntPrivateData;
	pfnGameShutdown = _pfnGameShutdown;
	pfnShouldCollide = _pfnShouldCollide;
	pfnCvarValue = _pfnCvarValue;
	pfnCvarValue2 = _pfnCvarValue2;
}

void meta_new_dll_functions_t::copy_to(NEW_DLL_FUNCTIONS *_pFuncs)
{
	Q_memcpy(_pFuncs, this, sizeof(NEW_DLL_FUNCTIONS));
}

// meta_enginefuncs_t
meta_enginefuncs_t::meta_enginefuncs_t(
	int              (*_pfnPrecacheModel)                   (const char*),
	int              (*_pfnPrecacheSound)                   (const char*),
	void             (*_pfnSetModel)                        (edict_t*, const char*),
	int              (*_pfnModelIndex)                      (const char*),
	int              (*_pfnModelFrames)                     (int),
	void             (*_pfnSetSize)                         (edict_t*, const float*, const float*),
	void             (*_pfnChangeLevel)                     (const char*, const char*),
	void             (*_pfnGetSpawnParms)                   (edict_t*),
	void             (*_pfnSaveSpawnParms)                  (edict_t*),
	float            (*_pfnVecToYaw)                        (const float*),
	void             (*_pfnVecToAngles)                     (const float*, float*),
	void             (*_pfnMoveToOrigin)                    (edict_t*, const float*, float, int),
	void             (*_pfnChangeYaw)                       (edict_t*),
	void             (*_pfnChangePitch)                     (edict_t*),
	edict_t*         (*_pfnFindEntityByString)              (edict_t*, const char*, const char*),
	int              (*_pfnGetEntityIllum)                  (edict_t*),
	edict_t*         (*_pfnFindEntityInSphere)              (edict_t*, const float*, float),
	edict_t*         (*_pfnFindClientInPVS)                 (edict_t*),
	edict_t*         (*_pfnEntitiesInPVS)                   (edict_t*),
	void             (*_pfnMakeVectors)                     (const float*),
	void             (*_pfnAngleVectors)                    (const float*, float*, float*, float*),
	edict_t*         (*_pfnCreateEntity)                    (),
	void             (*_pfnRemoveEntity)                    (edict_t*),
	edict_t*         (*_pfnCreateNamedEntity)               (int),
	void             (*_pfnMakeStatic)                      (edict_t*),
	int              (*_pfnEntIsOnFloor)                    (edict_t*),
	int              (*_pfnDropToFloor)                     (edict_t*),
	int	         (*_pfnWalkMove)                        (edict_t*, float, float, int),
	void             (*_pfnSetOrigin)                       (edict_t*, const float*),
	void             (*_pfnEmitSound)                       (edict_t*, int, const char*, float, float, int, int),
	void             (*_pfnEmitAmbientSound)                (edict_t*, float*, const char*, float, float, int, int),
	void             (*_pfnTraceLine)                       (const float*, const float*, int, edict_t*, TraceResult*),
	void             (*_pfnTraceToss)                       (edict_t*, edict_t*, TraceResult*),
	int	         (*_pfnTraceMonsterHull)                (edict_t*, const float*, const float*, int, edict_t*, TraceResult*),
	void             (*_pfnTraceHull)                       (const float*, const float*, int, int, edict_t*, TraceResult*),
	void             (*_pfnTraceModel)                      (const float*, const float*, int, edict_t*, TraceResult*),
	const char*      (*_pfnTraceTexture)                    (edict_t*, const float*, const float*),
	void             (*_pfnTraceSphere)                     (const float*, const float*, int, float, edict_t*, TraceResult*),
	void             (*_pfnGetAimVector)                    (edict_t*, float, float*),
	void             (*_pfnServerCommand)                   (char*),
	void             (*_pfnServerExecute)                   (),
	void             (*_pfnClientCommand)                   (edict_t*, char*, ...),
	void             (*_pfnParticleEffect)                  (const float*, const float*, float, float),
	void             (*_pfnLightStyle)                      (int, char*),
	int	         (*_pfnDecalIndex)                      (const char*),
	int              (*_pfnPointContents)                   (const float*),
	void             (*_pfnMessageBegin)                    (int, int, const float*, edict_t*),
	void             (*_pfnMessageEnd)                      (),
	void             (*_pfnWriteByte)                       (int),
	void             (*_pfnWriteChar)                       (int),
	void             (*_pfnWriteShort)                      (int),
	void             (*_pfnWriteLong)                       (int),
	void             (*_pfnWriteAngle)                      (float),
	void             (*_pfnWriteCoord)                      (float),
	void             (*_pfnWriteString)                     (const char*),
	void             (*_pfnWriteEntity)                     (int),
	void             (*_pfnCVarRegister)                    (cvar_t*),
	float            (*_pfnCVarGetFloat)                    (const char*),
	const char*      (*_pfnCVarGetString)                   (const char*),
	void             (*_pfnCVarSetFloat)                    (const char*, float),
	void             (*_pfnCVarSetString)                   (const char*, const char*),
	void             (*_pfnAlertMessage)                    (ALERT_TYPE, const char*, ...),
	void             (*_pfnEngineFprintf)                   (void *, const char*, ...),
	void*            (*_pfnPvAllocEntPrivateData)           (edict_t*, int32),
	void*            (*_pfnPvEntPrivateData)                (edict_t*),
	void             (*_pfnFreeEntPrivateData)              (edict_t*),
	const char*      (*_pfnSzFromIndex)                     (int),
	int              (*_pfnAllocString)	                (const char*),
	struct entvars_s*(*_pfnGetVarsOfEnt)                    (edict_t*),
	edict_t*         (*_pfnPEntityOfEntOffset)              (int),
	int              (*_pfnEntOffsetOfPEntity)              (const edict_t*),
	int              (*_pfnIndexOfEdict)                    (const edict_t*),
	edict_t*         (*_pfnPEntityOfEntIndex)               (int),
	edict_t*         (*_pfnFindEntityByVars)                (struct entvars_s*),
	void*            (*_pfnGetModelPtr)                     (edict_t*),
	int              (*_pfnRegUserMsg)                      (const char*, int),
	void             (*_pfnAnimationAutomove)               (const edict_t*, float),
	void             (*_pfnGetBonePosition)                 (const edict_t*, int, float*, float*),
	uint32           (*_pfnFunctionFromName)                (const char*),
	const char*      (*_pfnNameForFunction)                 (uint32),
	void             (*_pfnClientPrintf)                    (edict_t*, PRINT_TYPE, const char*),
	void             (*_pfnServerPrint)                     (const char*),
	const char*      (*_pfnCmd_Args)                        (),
	const char*      (*_pfnCmd_Argv)                        (int argc),
	int	         (*_pfnCmd_Argc)                        (),
	void             (*_pfnGetAttachment)                   (const edict_t*, int, float*, float*),
	void             (*_pfnCRC32_Init)                      (CRC32_t*),
	void             (*_pfnCRC32_ProcessBuffer)             (CRC32_t*, void*, int),
	void             (*_pfnCRC32_ProcessByte)               (CRC32_t*, unsigned char),
	CRC32_t          (*_pfnCRC32_Final)                     (CRC32_t),
	int32            (*_pfnRandomLong)                      (int32,  int32),
	float            (*_pfnRandomFloat)                     (float, float),
	void             (*_pfnSetView)                         (const edict_t*, const edict_t*),
	float            (*_pfnTime)                            (),
	void             (*_pfnCrosshairAngle)                  (const edict_t*, float, float),
	byte*            (*_pfnLoadFileForMe)                   (char*, int*),
	void             (*_pfnFreeFile)                        (void*),
	void             (*_pfnEndSection)                      (const char*),
	int              (*_pfnCompareFileTime)                 (char*, char*, int*),
	void             (*_pfnGetGameDir)                      (char*),
	void             (*_pfnCvar_RegisterVariable)           (cvar_t*),
	void             (*_pfnFadeClientVolume)                (const edict_t*, int, int, int, int),
	void             (*_pfnSetClientMaxspeed)               (edict_t*, float),
	edict_t*         (*_pfnCreateFakeClient)                (const char*),
	void             (*_pfnRunPlayerMove)                   (edict_t*, const float*, float, float, float, unsigned short, byte, byte),
	int              (*_pfnNumberOfEntities)                (),
	char*            (*_pfnGetInfoKeyBuffer)                (edict_t*),
	char*            (*_pfnInfoKeyValue)                    (char*, const char*),
	void             (*_pfnSetKeyValue)                     (char*, const char*, const char*),
	void             (*_pfnSetClientKeyValue)               (int, char*, const char*, const char*),
	int              (*_pfnIsMapValid)                      (char*),
	void             (*_pfnStaticDecal)                     (const float*, int, int, int),
	int              (*_pfnPrecacheGeneric)                 (char*),
	int	         (*_pfnGetPlayerUserId)                 (edict_t*),
	void             (*_pfnBuildSoundMsg)                   (edict_t*, int, const char*, float, float, int, int, int, int, const float*, edict_t*),
	int              (*_pfnIsDedicatedServer)               (),
	cvar_t*          (*_pfnCVarGetPointer)                  (const char*),
	unsigned int     (*_pfnGetPlayerWONId)                  (edict_t*),
	void             (*_pfnInfo_RemoveKey)                  (char*, const char*),
	const char*      (*_pfnGetPhysicsKeyValue)              (const edict_t*, const char*),
	void             (*_pfnSetPhysicsKeyValue)              (const edict_t*, const char*, const char*),
	const char*      (*_pfnGetPhysicsInfoString)            (const edict_t*),
	unsigned short   (*_pfnPrecacheEvent)                   (int, const char*),
	void             (*_pfnPlaybackEvent)                   (int, const edict_t*, unsigned short, float, float*, float*, float, float, int, int, int, int),
	unsigned char*   (*_pfnSetFatPVS)                       (float*),
	unsigned char*   (*_pfnSetFatPAS)                       (float*),
	int              (*_pfnCheckVisibility)                 (edict_t*, unsigned char*),
	void             (*_pfnDeltaSetField)                   (struct delta_s*, const char*),
	void             (*_pfnDeltaUnsetField)                 (struct delta_s*, const char*),
	void             (*_pfnDeltaAddEncoder)                 (char*, void (*)(struct delta_s*, const unsigned char*, const unsigned char*)),
	int              (*_pfnGetCurrentPlayer)                (),
	int              (*_pfnCanSkipPlayer)                   (const edict_t*),
	int              (*_pfnDeltaFindField)                  (struct delta_s*, const char*),
	void             (*_pfnDeltaSetFieldByIndex)            (struct delta_s*, int),
	void             (*_pfnDeltaUnsetFieldByIndex)          (struct delta_s*, int),
	void             (*_pfnSetGroupMask)                    (int, int),
	int              (*_pfnCreateInstancedBaseline)         (int, struct entity_state_s*),
	void             (*_pfnCvar_DirectSet)                  (struct cvar_s*, const char*),
	void             (*_pfnForceUnmodified)                 (FORCE_TYPE, float*, float*, const char*),
	void             (*_pfnGetPlayerStats)                  (const edict_t*, int*, int*),
	void             (*_pfnAddServerCommand)                (char*, void (*) ()),
	qboolean         (*_pfnVoice_GetClientListening)        (int, int),
	qboolean         (*_pfnVoice_SetClientListening)        (int, int, qboolean),
	const char*      (*_pfnGetPlayerAuthId)                 (edict_t*),
	sequenceEntry_s* (*_pfnSequenceGet)                     (const char*, const char*),
	sentenceEntry_s* (*_pfnSequencePickSentence)            (const char*, int, int*),
	int              (*_pfnGetFileSize)                     (char*),
	unsigned int     (*_pfnGetApproxWavePlayLen)            (const char*),
	int              (*_pfnIsCareerMatch)                   (),
	int              (*_pfnGetLocalizedStringLength)        (const char*),
	void             (*_pfnRegisterTutorMessageShown)       (int),
	int              (*_pfnGetTimesTutorMessageShown)       (int),
	void             (*_pfnProcessTutorMessageDecayBuffer)  (int*, int),
	void             (*_pfnConstructTutorMessageDecayBuffer)(int*, int),
	void             (*_pfnResetTutorMessageDecayData)      (),
	void             (*_pfnQueryClientCvarValue)            (const edict_t*, const char*),
	void             (*_pfnQueryClientCvarValue2)           (const edict_t*, const char*, int),
	int             (*_pfnEngCheckParm)           		(const char*, char**)
   )
{
	pfnPrecacheModel = _pfnPrecacheModel;
	pfnPrecacheSound = _pfnPrecacheSound;
	pfnSetModel = _pfnSetModel;
	pfnModelIndex = _pfnModelIndex;
	pfnModelFrames = _pfnModelFrames;
	pfnSetSize = _pfnSetSize;
	pfnChangeLevel = _pfnChangeLevel;
	pfnGetSpawnParms = _pfnGetSpawnParms;
	pfnSaveSpawnParms = _pfnSaveSpawnParms;
	pfnVecToYaw = _pfnVecToYaw;
	pfnVecToAngles = _pfnVecToAngles;
	pfnMoveToOrigin = _pfnMoveToOrigin;
	pfnChangeYaw = _pfnChangeYaw;
	pfnChangePitch = _pfnChangePitch;
	pfnFindEntityByString = _pfnFindEntityByString;
	pfnGetEntityIllum = _pfnGetEntityIllum;
	pfnFindEntityInSphere = _pfnFindEntityInSphere;
	pfnFindClientInPVS = _pfnFindClientInPVS;
	pfnEntitiesInPVS = _pfnEntitiesInPVS;
	pfnMakeVectors = _pfnMakeVectors;
	pfnAngleVectors = _pfnAngleVectors;
	pfnCreateEntity = _pfnCreateEntity;
	pfnRemoveEntity = _pfnRemoveEntity;
	pfnCreateNamedEntity = _pfnCreateNamedEntity;
	pfnMakeStatic = _pfnMakeStatic;
	pfnEntIsOnFloor = _pfnEntIsOnFloor;
	pfnDropToFloor = _pfnDropToFloor;
	pfnWalkMove = _pfnWalkMove;
	pfnSetOrigin = _pfnSetOrigin;
	pfnEmitSound = _pfnEmitSound;
	pfnEmitAmbientSound = _pfnEmitAmbientSound;
	pfnTraceLine = _pfnTraceLine;
	pfnTraceToss = _pfnTraceToss;
	pfnTraceMonsterHull = _pfnTraceMonsterHull;
	pfnTraceHull = _pfnTraceHull;
	pfnTraceModel = _pfnTraceModel;
	pfnTraceTexture = _pfnTraceTexture;
	pfnTraceSphere = _pfnTraceSphere;
	pfnGetAimVector = _pfnGetAimVector;
	pfnServerCommand = _pfnServerCommand;
	pfnServerExecute = _pfnServerExecute;
	pfnClientCommand = _pfnClientCommand;
	pfnParticleEffect = _pfnParticleEffect;
	pfnLightStyle = _pfnLightStyle;
	pfnDecalIndex = _pfnDecalIndex;
	pfnPointContents = _pfnPointContents;
	pfnMessageBegin = _pfnMessageBegin;
	pfnMessageEnd = _pfnMessageEnd;
	pfnWriteByte = _pfnWriteByte;
	pfnWriteChar = _pfnWriteChar;
	pfnWriteShort = _pfnWriteShort;
	pfnWriteLong = _pfnWriteLong;
	pfnWriteAngle = _pfnWriteAngle;
	pfnWriteCoord = _pfnWriteCoord;
	pfnWriteString = _pfnWriteString;
	pfnWriteEntity = _pfnWriteEntity;
	pfnCVarRegister = _pfnCVarRegister;
	pfnCVarGetFloat = _pfnCVarGetFloat;
	pfnCVarGetString = _pfnCVarGetString;
	pfnCVarSetFloat = _pfnCVarSetFloat;
	pfnCVarSetString = _pfnCVarSetString;
	pfnAlertMessage = _pfnAlertMessage;
	pfnEngineFprintf = _pfnEngineFprintf;
	pfnPvAllocEntPrivateData = _pfnPvAllocEntPrivateData;
	pfnPvEntPrivateData = _pfnPvEntPrivateData;
	pfnFreeEntPrivateData = _pfnFreeEntPrivateData;
	pfnSzFromIndex = _pfnSzFromIndex;
	pfnAllocString = _pfnAllocString;
	pfnGetVarsOfEnt = _pfnGetVarsOfEnt;
	pfnPEntityOfEntOffset = _pfnPEntityOfEntOffset;
	pfnEntOffsetOfPEntity = _pfnEntOffsetOfPEntity;
	pfnIndexOfEdict = _pfnIndexOfEdict;
	pfnPEntityOfEntIndex = _pfnPEntityOfEntIndex;
	pfnFindEntityByVars = _pfnFindEntityByVars;
	pfnGetModelPtr = _pfnGetModelPtr;
	pfnRegUserMsg = _pfnRegUserMsg;
	pfnAnimationAutomove = _pfnAnimationAutomove;
	pfnGetBonePosition = _pfnGetBonePosition;
	pfnFunctionFromName = _pfnFunctionFromName;
	pfnNameForFunction = _pfnNameForFunction;
	pfnClientPrintf = _pfnClientPrintf;
	pfnServerPrint = _pfnServerPrint;
	pfnCmd_Args = _pfnCmd_Args;
	pfnCmd_Argv = _pfnCmd_Argv;
	pfnCmd_Argc = _pfnCmd_Argc;
	pfnGetAttachment = _pfnGetAttachment;
	pfnCRC32_Init = _pfnCRC32_Init;
	pfnCRC32_ProcessBuffer = _pfnCRC32_ProcessBuffer;
	pfnCRC32_ProcessByte = _pfnCRC32_ProcessByte;
	pfnCRC32_Final = _pfnCRC32_Final;
	pfnRandomLong = _pfnRandomLong;
	pfnRandomFloat = _pfnRandomFloat;
	pfnSetView = _pfnSetView;
	pfnTime = _pfnTime;
	pfnCrosshairAngle = _pfnCrosshairAngle;
	pfnLoadFileForMe = _pfnLoadFileForMe;
	pfnFreeFile = _pfnFreeFile;
	pfnEndSection = _pfnEndSection;
	pfnCompareFileTime = _pfnCompareFileTime;
	pfnGetGameDir = _pfnGetGameDir;
	pfnCvar_RegisterVariable = _pfnCvar_RegisterVariable;
	pfnFadeClientVolume = _pfnFadeClientVolume;
	pfnSetClientMaxspeed = _pfnSetClientMaxspeed;
	pfnCreateFakeClient = _pfnCreateFakeClient;
	pfnRunPlayerMove = _pfnRunPlayerMove;
	pfnNumberOfEntities = _pfnNumberOfEntities;
	pfnGetInfoKeyBuffer = _pfnGetInfoKeyBuffer;
	pfnInfoKeyValue = _pfnInfoKeyValue;
	pfnSetKeyValue = _pfnSetKeyValue;
	pfnSetClientKeyValue = _pfnSetClientKeyValue;
	pfnIsMapValid = _pfnIsMapValid;
	pfnStaticDecal = _pfnStaticDecal;
	pfnPrecacheGeneric = _pfnPrecacheGeneric;
	pfnGetPlayerUserId = _pfnGetPlayerUserId;
	pfnBuildSoundMsg = _pfnBuildSoundMsg;
	pfnIsDedicatedServer = _pfnIsDedicatedServer;
	pfnCVarGetPointer = _pfnCVarGetPointer;
	pfnGetPlayerWONId = _pfnGetPlayerWONId;
	pfnInfo_RemoveKey = _pfnInfo_RemoveKey;
	pfnGetPhysicsKeyValue = _pfnGetPhysicsKeyValue;
	pfnSetPhysicsKeyValue = _pfnSetPhysicsKeyValue;
	pfnGetPhysicsInfoString = _pfnGetPhysicsInfoString;
	pfnPrecacheEvent = _pfnPrecacheEvent;
	pfnPlaybackEvent = _pfnPlaybackEvent;
	pfnSetFatPVS = _pfnSetFatPVS;
	pfnSetFatPAS = _pfnSetFatPAS;
	pfnCheckVisibility = _pfnCheckVisibility;
	pfnDeltaSetField = _pfnDeltaSetField;
	pfnDeltaUnsetField = _pfnDeltaUnsetField;
	pfnDeltaAddEncoder = _pfnDeltaAddEncoder;
	pfnGetCurrentPlayer = _pfnGetCurrentPlayer;
	pfnCanSkipPlayer = _pfnCanSkipPlayer;
	pfnDeltaFindField = _pfnDeltaFindField;
	pfnDeltaSetFieldByIndex = _pfnDeltaSetFieldByIndex;
	pfnDeltaUnsetFieldByIndex = _pfnDeltaUnsetFieldByIndex;
	pfnSetGroupMask = _pfnSetGroupMask;
	pfnCreateInstancedBaseline = _pfnCreateInstancedBaseline;
	pfnCvar_DirectSet = _pfnCvar_DirectSet;
	pfnForceUnmodified = _pfnForceUnmodified;
	pfnGetPlayerStats = _pfnGetPlayerStats;
	pfnAddServerCommand = _pfnAddServerCommand;
	pfnVoice_GetClientListening = _pfnVoice_GetClientListening;
	pfnVoice_SetClientListening = _pfnVoice_SetClientListening;
	pfnGetPlayerAuthId = _pfnGetPlayerAuthId;
	pfnSequenceGet = _pfnSequenceGet;
	pfnSequencePickSentence = _pfnSequencePickSentence;
	pfnGetFileSize = _pfnGetFileSize;
	pfnGetApproxWavePlayLen = _pfnGetApproxWavePlayLen;
	pfnIsCareerMatch = _pfnIsCareerMatch;
	pfnGetLocalizedStringLength = _pfnGetLocalizedStringLength;
	pfnRegisterTutorMessageShown = _pfnRegisterTutorMessageShown;
	pfnGetTimesTutorMessageShown = _pfnGetTimesTutorMessageShown;
	pfnProcessTutorMessageDecayBuffer = _pfnProcessTutorMessageDecayBuffer;
	pfnConstructTutorMessageDecayBuffer = _pfnConstructTutorMessageDecayBuffer;
	pfnResetTutorMessageDecayData = _pfnResetTutorMessageDecayData;
	pfnQueryClientCvarValue = _pfnQueryClientCvarValue;
	pfnQueryClientCvarValue2 = _pfnQueryClientCvarValue2;
	pfnEngCheckParm = _pfnEngCheckParm;
}

// HL_enginefuncs
void HL_enginefuncs_t::initialise_interface(enginefuncs_t *_pFuncs)
{
	set_from(_pFuncs);

	// Now the pfnAlertMessage is available and we trust it to be a valid
	// pointer, so flush the message buffer.
	flush_ALERT_buffer();
}
