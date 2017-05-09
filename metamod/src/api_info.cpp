#include "precompiled.h"

#define API_ENTRY(table, name, loglevel)	{ offsetof(table, pfn##name), #table "::" #name, loglevel }
#define DLLAPI_ENTRY(name, loglevel)		API_ENTRY(DLL_FUNCTIONS, name, loglevel)
#define NEWAPI_ENTRY(name, loglevel)		API_ENTRY(NEW_DLL_FUNCTIONS, name, loglevel)
#define ENGAPI_ENTRY(name, loglevel)		API_ENTRY(enginefuncs_t, name, loglevel)

// trace flag, loglevel, name
dllapi_info_t g_dllapi_info =
{
	DLLAPI_ENTRY(GameInit, 1),					// pfnGameInit
	DLLAPI_ENTRY(Spawn, 2),						// pfnSpawn
	DLLAPI_ENTRY(Think, 2),						// pfnThink
	DLLAPI_ENTRY(Use, 2),						// pfnUse
	DLLAPI_ENTRY(Touch, 2),						// pfnTouch
	DLLAPI_ENTRY(Blocked, 2),					// pfnBlocked
	DLLAPI_ENTRY(KeyValue, 4),					// pfnKeyValue
	DLLAPI_ENTRY(Save, 4),						// pfnSave
	DLLAPI_ENTRY(Restore, 4),					// pfnRestore
	DLLAPI_ENTRY(SetAbsBox, 2),					// pfnSetAbsBox
	DLLAPI_ENTRY(SaveWriteFields, 4),			// pfnSaveWriteFields
	DLLAPI_ENTRY(SaveReadFields, 4),			// pfnSaveReadFields
	DLLAPI_ENTRY(SaveGlobalState, 4),			// pfnSaveGlobalState
	DLLAPI_ENTRY(RestoreGlobalState, 4),		// pfnRestoreGlobalState
	DLLAPI_ENTRY(ResetGlobalState, 4),			// pfnResetGlobalState
	DLLAPI_ENTRY(ClientConnect, 1),				// pfnClientConnect
	DLLAPI_ENTRY(ClientDisconnect, 1),			// pfnClientDisconnect
	DLLAPI_ENTRY(ClientKill, 2),				// pfnClientKill
	DLLAPI_ENTRY(ClientPutInServer, 1),			// pfnClientPutInServer
	DLLAPI_ENTRY(ClientCommand, 2),				// pfnClientCommand
	DLLAPI_ENTRY(ClientUserInfoChanged, 2),		// pfnClientUserInfoChanged
	DLLAPI_ENTRY(ServerActivate, 1),			// pfnServerActivate
	DLLAPI_ENTRY(ServerDeactivate, 1),			// pfnServerDeactivate
	DLLAPI_ENTRY(PlayerPreThink, 3),			// pfnPlayerPreThink
	DLLAPI_ENTRY(PlayerPostThink, 3),			// pfnPlayerPostThink
	DLLAPI_ENTRY(StartFrame, 4),				// pfnStartFrame
	DLLAPI_ENTRY(ParmsNewLevel, 4),				// pfnParmsNewLevel
	DLLAPI_ENTRY(ParmsChangeLevel, 4),			// pfnParmsChangeLevel
	DLLAPI_ENTRY(GetGameDescription, 3),		// pfnGetGameDescription
	DLLAPI_ENTRY(PlayerCustomization, 4),		// pfnPlayerCustomization
	DLLAPI_ENTRY(SpectatorConnect, 3),			// pfnSpectatorConnect
	DLLAPI_ENTRY(SpectatorDisconnect, 3),		// pfnSpectatorDisconnect
	DLLAPI_ENTRY(SpectatorThink, 3),			// pfnSpectatorThink
	DLLAPI_ENTRY(Sys_Error, 4),					// pfnSys_Error
	DLLAPI_ENTRY(PM_Move, 3),					// pfnPM_Move
	DLLAPI_ENTRY(PM_Init, 1),					// pfnPM_Init
	DLLAPI_ENTRY(PM_FindTextureType, 3),		// pfnPM_FindTextureType
	DLLAPI_ENTRY(SetupVisibility, 3),			// pfnSetupVisibility
	DLLAPI_ENTRY(UpdateClientData, 3),			// pfnUpdateClientData
	DLLAPI_ENTRY(AddToFullPack, 3),				// pfnAddToFullPack
	DLLAPI_ENTRY(CreateBaseline, 4),			// pfnCreateBaseline
	DLLAPI_ENTRY(RegisterEncoders, 4),			// pfnRegisterEncoders
	DLLAPI_ENTRY(GetWeaponData, 3),				// pfnGetWeaponData
	DLLAPI_ENTRY(CmdStart, 3),					// pfnCmdStart
	DLLAPI_ENTRY(CmdEnd, 3),					// pfnCmdEnd
	DLLAPI_ENTRY(ConnectionlessPacket, 4),		// pfnConnectionlessPacket
	DLLAPI_ENTRY(GetHullBounds, 3),				// pfnGetHullBounds
	DLLAPI_ENTRY(CreateInstancedBaselines, 4),	// pfnCreateInstancedBaselines
	DLLAPI_ENTRY(InconsistentFile, 3),			// pfnInconsistentFile
	DLLAPI_ENTRY(AllowLagCompensation, 4),		// pfnAllowLagCompensation

	{ 0, "", 0 },
};

newapi_info_t g_newapi_info = {
	NEWAPI_ENTRY(OnFreeEntPrivateData, 3),	// pfnOnFreeEntPrivateData
	NEWAPI_ENTRY(GameShutdown, 3),			// pfnGameShutdown
	NEWAPI_ENTRY(ShouldCollide, 3),			// pfnShouldCollide
	NEWAPI_ENTRY(CvarValue, 2),				// pfnCvarValue
	NEWAPI_ENTRY(CvarValue2, 2),			// pfnCvarValue2

	{ 0, "", 0 },
};

engine_info_t g_engineapi_info =
{
	ENGAPI_ENTRY(PrecacheModel, 2),						// pfnPrecacheModel
	ENGAPI_ENTRY(PrecacheSound, 2),						// pfnPrecacheSound
	ENGAPI_ENTRY(SetModel, 2),							// pfnSetModel
	ENGAPI_ENTRY(ModelIndex, 2),						// pfnModelIndex
	ENGAPI_ENTRY(ModelFrames, 3),						// pfnModelFrames
	ENGAPI_ENTRY(SetSize, 2),							// pfnSetSize
	ENGAPI_ENTRY(ChangeLevel, 2),						// pfnChangeLevel
	ENGAPI_ENTRY(GetSpawnParms, 4),						// pfnGetSpawnParms
	ENGAPI_ENTRY(SaveSpawnParms, 4),					// pfnSaveSpawnParms
	ENGAPI_ENTRY(VecToYaw, 3),							// pfnVecToYaw
	ENGAPI_ENTRY(VecToAngles, 3),						// pfnVecToAngles
	ENGAPI_ENTRY(MoveToOrigin, 3),						// pfnMoveToOrigin
	ENGAPI_ENTRY(ChangeYaw, 3),							// pfnChangeYaw
	ENGAPI_ENTRY(ChangePitch, 3),						// pfnChangePitch
	ENGAPI_ENTRY(FindEntityByString, 2),				// pfnFindEntityByString
	ENGAPI_ENTRY(GetEntityIllum, 4),					// pfnGetEntityIllum
	ENGAPI_ENTRY(FindEntityInSphere, 2),				// pfnFindEntityInSphere
	ENGAPI_ENTRY(FindClientInPVS, 2),					// pfnFindClientInPVS
	ENGAPI_ENTRY(EntitiesInPVS, 2),						// pfnEntitiesInPVS
	ENGAPI_ENTRY(MakeVectors, 3),						// pfnMakeVectors
	ENGAPI_ENTRY(AngleVectors, 3),						// pfnAngleVectors
	ENGAPI_ENTRY(CreateEntity, 2),						// pfnCreateEntity
	ENGAPI_ENTRY(RemoveEntity, 2),						// pfnRemoveEntity
	ENGAPI_ENTRY(CreateNamedEntity, 2),					// pfnCreateNamedEntity
	ENGAPI_ENTRY(MakeStatic, 2),						// pfnMakeStatic
	ENGAPI_ENTRY(EntIsOnFloor, 2),						// pfnEntIsOnFloor
	ENGAPI_ENTRY(DropToFloor, 2),						// pfnDropToFloor
	ENGAPI_ENTRY(WalkMove, 2),							// pfnWalkMove
	ENGAPI_ENTRY(SetOrigin, 2),							// pfnSetOrigin
	ENGAPI_ENTRY(EmitSound, 2),							// pfnEmitSound
	ENGAPI_ENTRY(EmitAmbientSound, 2),					// pfnEmitAmbientSound
	ENGAPI_ENTRY(TraceLine, 2),							// pfnTraceLine
	ENGAPI_ENTRY(TraceToss, 2),							// pfnTraceToss
	ENGAPI_ENTRY(TraceMonsterHull, 2),					// pfnTraceMonsterHull
	ENGAPI_ENTRY(TraceHull, 2),							// pfnTraceHull
	ENGAPI_ENTRY(TraceModel, 2),						// pfnTraceModel
	ENGAPI_ENTRY(TraceTexture, 2),						// pfnTraceTexture
	ENGAPI_ENTRY(TraceSphere, 2),						// pfnTraceSphere
	ENGAPI_ENTRY(GetAimVector, 2),						// pfnGetAimVector
	ENGAPI_ENTRY(ServerCommand, 3),						// pfnServerCommand
	ENGAPI_ENTRY(ServerExecute, 3),						// pfnServerExecute
	ENGAPI_ENTRY(ClientCommand, 3),						// pfnClientCommand	
	ENGAPI_ENTRY(ParticleEffect, 4),					// pfnParticleEffect
	ENGAPI_ENTRY(LightStyle, 2),						// pfnLightStyle
	ENGAPI_ENTRY(DecalIndex, 2),						// pfnDecalIndex
	ENGAPI_ENTRY(PointContents, 2),						// pfnPointContents
	ENGAPI_ENTRY(MessageBegin, 3),						// pfnMessageBegin
	ENGAPI_ENTRY(MessageEnd, 3),						// pfnMessageEnd
	ENGAPI_ENTRY(WriteByte, 3),							// pfnWriteByte
	ENGAPI_ENTRY(WriteChar, 3),							// pfnWriteChar
	ENGAPI_ENTRY(WriteShort, 3),						// pfnWriteShort
	ENGAPI_ENTRY(WriteLong, 3),							// pfnWriteLong
	ENGAPI_ENTRY(WriteAngle, 3),						// pfnWriteAngle
	ENGAPI_ENTRY(WriteCoord, 3),						// pfnWriteCoord
	ENGAPI_ENTRY(WriteString, 3),						// pfnWriteString
	ENGAPI_ENTRY(WriteEntity, 3),						// pfnWriteEntity
	ENGAPI_ENTRY(CVarRegister, 2),						// pfnCVarRegister
	ENGAPI_ENTRY(CVarGetFloat, 2),						// pfnCVarGetFloat
	ENGAPI_ENTRY(CVarGetString, 2),						// pfnCVarGetString
	ENGAPI_ENTRY(CVarSetFloat, 2),						// pfnCVarSetFloat
	ENGAPI_ENTRY(CVarSetString, 2),						// pfnCVarSetString
	ENGAPI_ENTRY(AlertMessage, 3),						// pfnAlertMessage
	ENGAPI_ENTRY(EngineFprintf, 3),						// pfnEngineFprintf
	ENGAPI_ENTRY(PvAllocEntPrivateData, 2),				// pfnPvAllocEntPrivateData
	ENGAPI_ENTRY(PvEntPrivateData, 2),					// pfnPvEntPrivateData
	ENGAPI_ENTRY(FreeEntPrivateData, 2),				// pfnFreeEntPrivateData
	ENGAPI_ENTRY(SzFromIndex, 3),						// pfnSzFromIndex
	ENGAPI_ENTRY(AllocString, 3),						// pfnAllocString
	ENGAPI_ENTRY(GetVarsOfEnt, 2),						// pfnGetVarsOfEnt
	ENGAPI_ENTRY(PEntityOfEntOffset, 3),				// pfnPEntityOfEntOffset
	ENGAPI_ENTRY(EntOffsetOfPEntity, 3),				// pfnEntOffsetOfPEntity
	ENGAPI_ENTRY(IndexOfEdict, 3),						// pfnIndexOfEdict
	ENGAPI_ENTRY(PEntityOfEntIndex, 3),					// pfnPEntityOfEntIndex
	ENGAPI_ENTRY(FindEntityByVars, 3),					// pfnFindEntityByVars
	ENGAPI_ENTRY(GetModelPtr, 3),						// pfnGetModelPtr
	ENGAPI_ENTRY(RegUserMsg, 3),						// pfnRegUserMsg
	ENGAPI_ENTRY(AnimationAutomove, 2),					// pfnAnimationAutomove
	ENGAPI_ENTRY(GetBonePosition, 2),					// pfnGetBonePosition
	ENGAPI_ENTRY(FunctionFromName, 4),					// pfnFunctionFromName
	ENGAPI_ENTRY(NameForFunction, 4),					// pfnNameForFunction
	ENGAPI_ENTRY(ClientPrintf, 3),						// pfnClientPrintf
	ENGAPI_ENTRY(ServerPrint, 3),						// pfnServerPrint
	ENGAPI_ENTRY(Cmd_Args, 3),							// pfnCmd_Args
	ENGAPI_ENTRY(Cmd_Argv, 3),							// pfnCmd_Argv
	ENGAPI_ENTRY(Cmd_Argc, 3),							// pfnCmd_Argc
	ENGAPI_ENTRY(GetAttachment, 2),						// pfnGetAttachment
	ENGAPI_ENTRY(CRC32_Init, 4),						// pfnCRC32_Init
	ENGAPI_ENTRY(CRC32_ProcessBuffer, 4),				// pfnCRC32_ProcessBuffer
	ENGAPI_ENTRY(CRC32_ProcessByte, 4),					// pfnCRC32_ProcessByte
	ENGAPI_ENTRY(CRC32_Final, 4),						// pfnCRC32_Final
	ENGAPI_ENTRY(RandomLong, 3),						// pfnRandomLong
	ENGAPI_ENTRY(RandomFloat, 3),						// pfnRandomFloat
	ENGAPI_ENTRY(SetView, 2),							// pfnSetView
	ENGAPI_ENTRY(Time, 2),								// pfnTime
	ENGAPI_ENTRY(CrosshairAngle, 2),					// pfnCrosshairAngle
	ENGAPI_ENTRY(LoadFileForMe, 4),						// pfnLoadFileForMe
	ENGAPI_ENTRY(FreeFile, 4),							// pfnFreeFile
	ENGAPI_ENTRY(EndSection, 4),						// pfnEndSection
	ENGAPI_ENTRY(CompareFileTime, 4),					// pfnCompareFileTime
	ENGAPI_ENTRY(GetGameDir, 3),						// pfnGetGameDir
	ENGAPI_ENTRY(Cvar_RegisterVariable, 2),				// pfnCvar_RegisterVariable
	ENGAPI_ENTRY(FadeClientVolume, 2),					// pfnFadeClientVolume
	ENGAPI_ENTRY(SetClientMaxspeed, 3),					// pfnSetClientMaxspeed
	ENGAPI_ENTRY(CreateFakeClient, 2),					// pfnCreateFakeClient
	ENGAPI_ENTRY(RunPlayerMove, 3),						// pfnRunPlayerMove
	ENGAPI_ENTRY(NumberOfEntities, 2),					// pfnNumberOfEntities
	ENGAPI_ENTRY(GetInfoKeyBuffer, 2),					// pfnGetInfoKeyBuffer
	ENGAPI_ENTRY(InfoKeyValue, 2),						// pfnInfoKeyValue
	ENGAPI_ENTRY(SetKeyValue, 2),						// pfnSetKeyValue
	ENGAPI_ENTRY(SetClientKeyValue, 2),					// pfnSetClientKeyValue
	ENGAPI_ENTRY(IsMapValid, 2),						// pfnIsMapValid
	ENGAPI_ENTRY(StaticDecal, 2),						// pfnStaticDecal
	ENGAPI_ENTRY(PrecacheGeneric, 2),					// pfnPrecacheGeneric
	ENGAPI_ENTRY(GetPlayerUserId, 2),					// pfnGetPlayerUserId
	ENGAPI_ENTRY(BuildSoundMsg, 2),						// pfnBuildSoundMsg
	ENGAPI_ENTRY(IsDedicatedServer, 2),					// pfnIsDedicatedServer
	ENGAPI_ENTRY(CVarGetPointer, 3),					// pfnCVarGetPointer
	ENGAPI_ENTRY(GetPlayerWONId, 4),					// pfnGetPlayerWONId
	ENGAPI_ENTRY(Info_RemoveKey, 2),					// pfnInfo_RemoveKey
	ENGAPI_ENTRY(GetPhysicsKeyValue, 2),				// pfnGetPhysicsKeyValue
	ENGAPI_ENTRY(SetPhysicsKeyValue, 2),				// pfnSetPhysicsKeyValue
	ENGAPI_ENTRY(GetPhysicsInfoString, 2),				// pfnGetPhysicsInfoString
	ENGAPI_ENTRY(PrecacheEvent, 2),						// pfnPrecacheEvent
	ENGAPI_ENTRY(PlaybackEvent, 2),						// pfnPlaybackEvent
	ENGAPI_ENTRY(SetFatPVS, 2),							// pfnSetFatPVS
	ENGAPI_ENTRY(SetFatPAS, 2),							// pfnSetFatPAS
	ENGAPI_ENTRY(CheckVisibility, 2),					// pfnCheckVisibility
	ENGAPI_ENTRY(DeltaSetField, 3),						// pfnDeltaSetField
	ENGAPI_ENTRY(DeltaUnsetField, 3),					// pfnDeltaUnsetField
	ENGAPI_ENTRY(DeltaAddEncoder, 3),					// pfnDeltaAddEncoder
	ENGAPI_ENTRY(GetCurrentPlayer, 2),					// pfnGetCurrentPlayer
	ENGAPI_ENTRY(CanSkipPlayer, 2),						// pfnCanSkipPlayer
	ENGAPI_ENTRY(DeltaFindField, 3),					// pfnDeltaFindField
	ENGAPI_ENTRY(DeltaSetFieldByIndex, 3),				// pfnDeltaSetFieldByIndex
	ENGAPI_ENTRY(DeltaUnsetFieldByIndex, 3),			// pfnDeltaUnsetFieldByIndex
	ENGAPI_ENTRY(SetGroupMask, 2),						// pfnSetGroupMask
	ENGAPI_ENTRY(CreateInstancedBaseline, 3),			// pfnCreateInstancedBaseline
	ENGAPI_ENTRY(Cvar_DirectSet, 2),					// pfnCvar_DirectSet
	ENGAPI_ENTRY(ForceUnmodified, 2),					// pfnForceUnmodified
	ENGAPI_ENTRY(GetPlayerStats, 2),					// pfnGetPlayerStats
	ENGAPI_ENTRY(AddServerCommand, 2),					// pfnAddServerCommand

	ENGAPI_ENTRY(Voice_GetClientListening, 2),			// Voice_GetClientListening
	ENGAPI_ENTRY(Voice_SetClientListening, 2),			// Voice_SetClientListening
	ENGAPI_ENTRY(GetPlayerAuthId, 2),					// pfnGetPlayerAuthId
	ENGAPI_ENTRY(SequenceGet, 2),						// pfnSequenceGet

	ENGAPI_ENTRY(SequencePickSentence, 2),				// pfnSequencePickSentence
	ENGAPI_ENTRY(GetFileSize, 4),						// pfnGetFileSize
	ENGAPI_ENTRY(GetApproxWavePlayLen, 4),				// pfnGetApproxWavePlayLen
	ENGAPI_ENTRY(IsCareerMatch, 4),						// pfnIsCareerMatch
	ENGAPI_ENTRY(GetLocalizedStringLength, 4),			// pfnGetLocalizedStringLength
	ENGAPI_ENTRY(RegisterTutorMessageShown, 4),			// pfnRegisterTutorMessageShown
	ENGAPI_ENTRY(GetTimesTutorMessageShown, 4),			// pfnGetTimesTutorMessageShown
	ENGAPI_ENTRY(ProcessTutorMessageDecayBuffer, 4),	// pfnProcessTutorMessageDecayBuffer
	ENGAPI_ENTRY(ConstructTutorMessageDecayBuffer, 4),	// pfnConstructTutorMessageDecayBuffer
	ENGAPI_ENTRY(ResetTutorMessageDecayData, 4),		// pfnResetTutorMessageDecayData

	ENGAPI_ENTRY(QueryClientCvarValue, 2),				//pfnQueryClientCvarValue
	ENGAPI_ENTRY(QueryClientCvarValue2, 2),				//pfnQueryClientCvarValue2
	ENGAPI_ENTRY(EngCheckParm, 2),						//pfnCheckParm

	// end
	{ 0, "", 0 },
};
