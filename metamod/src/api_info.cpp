#include "precompiled.h"

// trace flag, loglevel, name
dllapi_info_t dllapi_info = {
	{ mFALSE, 3,	"GameDLLInit" },		// pfnGameInit
	{ mFALSE, 10,	"DispatchSpawn" },		// pfnSpawn
	{ mFALSE, 16,	"DispatchThink" },		// pfnThink
	{ mFALSE, 9,	"DispatchUse" },		// pfnUse
	{ mFALSE, 11,	"DispatchTouch" },		// pfnTouch
	{ mFALSE, 9,	"DispatchBlocked" },		// pfnBlocked
	{ mFALSE, 10,	"DispatchKeyValue" },		// pfnKeyValue
	{ mFALSE, 9,	"DispatchSave" },		// pfnSave
	{ mFALSE, 9,	"DispatchRestore" },		// pfnRestore
	{ mFALSE, 20,	"DispatchObjectCollsionBox" },	// pfnSetAbsBox
	{ mFALSE, 9,	"SaveWriteFields" },		// pfnSaveWriteFields
	{ mFALSE, 9,	"SaveReadFields" },		// pfnSaveReadFields
	{ mFALSE, 9,	"SaveGlobalState" },		// pfnSaveGlobalState
	{ mFALSE, 9,	"RestoreGlobalState" },		// pfnRestoreGlobalState
	{ mFALSE, 9,	"ResetGlobalState" },		// pfnResetGlobalState
	{ mFALSE, 3,	"ClientConnect" },		// pfnClientConnect
	{ mFALSE, 3,	"ClientDisconnect" },		// pfnClientDisconnect
	{ mFALSE, 3,	"ClientKill" },			// pfnClientKill
	{ mFALSE, 3,	"ClientPutInServer" },		// pfnClientPutInServer
	{ mFALSE, 9,	"ClientCommand" },		// pfnClientCommand
	{ mFALSE, 11,	"ClientUserInfoChanged" },	// pfnClientUserInfoChanged
	{ mFALSE, 3,	"ServerActivate" },		// pfnServerActivate
	{ mFALSE, 3,	"ServerDeactivate" },		// pfnServerDeactivate
	{ mFALSE, 14,	"PlayerPreThink" },		// pfnPlayerPreThink
	{ mFALSE, 14,	"PlayerPostThink" },		// pfnPlayerPostThink
	{ mFALSE, 18,	"StartFrame" },			// pfnStartFrame
	{ mFALSE, 9,	"ParmsNewLevel" },		// pfnParmsNewLevel
	{ mFALSE, 9,	"ParmsChangeLevel" },		// pfnParmsChangeLevel
	{ mFALSE, 9,	"GetGameDescription" },		// pfnGetGameDescription
	{ mFALSE, 9,	"PlayerCustomization" },	// pfnPlayerCustomization
	{ mFALSE, 9,	"SpectatorConnect" },		// pfnSpectatorConnect
	{ mFALSE, 9,	"SpectatorDisconnect" },	// pfnSpectatorDisconnect
	{ mFALSE, 9,	"SpectatorThink" },		// pfnSpectatorThink
	{ mFALSE, 3,	"Sys_Error" },			// pfnSys_Error
	{ mFALSE, 13,	"PM_Move" },			// pfnPM_Move
	{ mFALSE, 9,	"PM_Init" },			// pfnPM_Init
	{ mFALSE, 9,	"PM_FindTextureType" },		// pfnPM_FindTextureType
	{ mFALSE, 12,	"SetupVisibility" },		// pfnSetupVisibility
	{ mFALSE, 12,	"UpdateClientData" },		// pfnUpdateClientData
	{ mFALSE, 16,	"AddToFullPack" },		// pfnAddToFullPack
	{ mFALSE, 9,	"CreateBaseline" },		// pfnCreateBaseline
	{ mFALSE, 9,	"RegisterEncoders" },		// pfnRegisterEncoders
	{ mFALSE, 9,	"GetWeaponData" },		// pfnGetWeaponData
	{ mFALSE, 15,	"CmdStart" },			// pfnCmdStart
	{ mFALSE, 15,	"CmdEnd" },			// pfnCmdEnd
	{ mFALSE, 9,	"ConnectionlessPacket" },	// pfnConnectionlessPacket
	{ mFALSE, 9,	"GetHullBounds" },		// pfnGetHullBounds
	{ mFALSE, 9,	"CreateInstancedBaselines" },	// pfnCreateInstancedBaselines
	{ mFALSE, 3,	"InconsistentFile" },		// pfnInconsistentFile
	{ mFALSE, 20,	"AllowLagCompensation" },	// pfnAllowLagCompensation

	{ mFALSE, 0, nullptr },
};

newapi_info_t newapi_info = {
	{ mFALSE, 16,	"OnFreeEntPrivateData" },	// pfnOnFreeEntPrivateData
	{ mFALSE, 3,	"GameShutdown" },		// pfnGameShutdown
	{ mFALSE, 14,	"ShouldCollide" },		// pfnShouldCollide
	{ mFALSE, 3,	"CvarValue" },			// pfnCvarValue
	{ mFALSE, 3,	"CvarValue2" },			// pfnCvarValue2

	{ mFALSE, 0, nullptr },
};

engine_info_t engine_info = {
	{ mFALSE, 13,	"PrecacheModel" },		// pfnPrecacheModel
	{ mFALSE, 13,	"PrecacheSound" },		// pfnPrecacheSound
	{ mFALSE, 18,	"SetModel" },			// pfnSetModel
	{ mFALSE, 34,	"ModelIndex" },			// pfnModelIndex
	{ mFALSE, 10,	"ModelFrames" },		// pfnModelFrames
	{ mFALSE, 14,	"SetSize" },			// pfnSetSize
	{ mFALSE, 9,	"ChangeLevel" },		// pfnChangeLevel
	{ mFALSE, 9,	"GetSpawnParms" },		// pfnGetSpawnParms
	{ mFALSE, 9,	"SaveSpawnParms" },		// pfnSaveSpawnParms
	{ mFALSE, 9,	"VecToYaw" },			// pfnVecToYaw
	{ mFALSE, 14,	"VecToAngles" },		// pfnVecToAngles
	{ mFALSE, 9,	"MoveToOrigin" },		// pfnMoveToOrigin
	{ mFALSE, 9,	"ChangeYaw" },			// pfnChangeYaw
	{ mFALSE, 9,	"ChangePitch" },		// pfnChangePitch
	{ mFALSE, 32,	"FindEntityByString" },		// pfnFindEntityByString
	{ mFALSE, 9,	"GetEntityIllum" },		// pfnGetEntityIllum
	{ mFALSE, 9,	"FindEntityInSphere" },		// pfnFindEntityInSphere
	{ mFALSE, 19,	"FindClientInPVS" },		// pfnFindClientInPVS
	{ mFALSE, 9,	"EntitiesInPVS" },		// pfnEntitiesInPVS
	{ mFALSE, 40,	"MakeVectors" },		// pfnMakeVectors
	{ mFALSE, 9,	"AngleVectors" },		// pfnAngleVectors
	{ mFALSE, 13,	"CreateEntity" },		// pfnCreateEntity
	{ mFALSE, 13,	"RemoveEntity" },		// pfnRemoveEntity
	{ mFALSE, 13,	"CreateNamedEntity" },		// pfnCreateNamedEntity
	{ mFALSE, 9,	"MakeStatic" },			// pfnMakeStatic
	{ mFALSE, 9,	"EntIsOnFloor" },		// pfnEntIsOnFloor
	{ mFALSE, 9,	"DropToFloor" },		// pfnDropToFloor
	{ mFALSE, 9,	"WalkMove" },			// pfnWalkMove
	{ mFALSE, 14,	"SetOrigin" },			// pfnSetOrigin
	{ mFALSE, 12,	"EmitSound" },			// pfnEmitSound
	{ mFALSE, 12,	"EmitAmbientSound" },		// pfnEmitAmbientSound
	{ mFALSE, 20,	"TraceLine" },			// pfnTraceLine
	{ mFALSE, 9,	"TraceToss" },			// pfnTraceToss
	{ mFALSE, 9,	"TraceMonsterHull" },		// pfnTraceMonsterHull
	{ mFALSE, 9,	"TraceHull" },			// pfnTraceHull
	{ mFALSE, 9,	"TraceModel" },			// pfnTraceModel
	{ mFALSE, 15,	"TraceTexture" },		// pfnTraceTexture		// CS: when moving
	{ mFALSE, 9,	"TraceSphere" },		// pfnTraceSphere
	{ mFALSE, 9,	"GetAimVector" },		// pfnGetAimVector
	{ mFALSE, 9,	"ServerCommand" },		// pfnServerCommand
	{ mFALSE, 9,	"ServerExecute" },		// pfnServerExecute
	{ mFALSE, 11,	"engClientCommand" },		// pfnClientCommand		// d'oh, ClientCommand in dllapi too
	{ mFALSE, 9,	"ParticleEffect" },		// pfnParticleEffect
	{ mFALSE, 9,	"LightStyle" },			// pfnLightStyle
	{ mFALSE, 9,	"DecalIndex" },			// pfnDecalIndex
	{ mFALSE, 15,	"PointContents" },		// pfnPointContents		// CS: when moving
	{ mFALSE, 22,	"MessageBegin" },		// pfnMessageBegin
	{ mFALSE, 22,	"MessageEnd" },			// pfnMessageEnd
	{ mFALSE, 30,	"WriteByte" },			// pfnWriteByte
	{ mFALSE, 23,	"WriteChar" },			// pfnWriteChar
	{ mFALSE, 24,	"WriteShort" },			// pfnWriteShort
	{ mFALSE, 23,	"WriteLong" },			// pfnWriteLong
	{ mFALSE, 23,	"WriteAngle" },			// pfnWriteAngle
	{ mFALSE, 23,	"WriteCoord" },			// pfnWriteCoord
	{ mFALSE, 25,	"WriteString" },		// pfnWriteString
	{ mFALSE, 23,	"WriteEntity" },		// pfnWriteEntity
	{ mFALSE, 9,	"CVarRegister" },		// pfnCVarRegister
	{ mFALSE, 21,	"CVarGetFloat" },		// pfnCVarGetFloat
	{ mFALSE, 9,	"CVarGetString" },		// pfnCVarGetString
	{ mFALSE, 10,	"CVarSetFloat" },		// pfnCVarSetFloat
	{ mFALSE, 9,	"CVarSetString" },		// pfnCVarSetString
	{ mFALSE, 15,	"AlertMessage" },		// pfnAlertMessage
	{ mFALSE, 17,	"EngineFprintf" },		// pfnEngineFprintf
	{ mFALSE, 14,	"PvAllocEntPrivateData" },	// pfnPvAllocEntPrivateData
	{ mFALSE, 9,	"PvEntPrivateData" },		// pfnPvEntPrivateData
	{ mFALSE, 9,	"FreeEntPrivateData" },		// pfnFreeEntPrivateData
	{ mFALSE, 9,	"SzFromIndex" },		// pfnSzFromIndex
	{ mFALSE, 10,	"AllocString" },		// pfnAllocString
	{ mFALSE, 9,	"GetVarsOfEnt" },		// pfnGetVarsOfEnt
	{ mFALSE, 14,	"PEntityOfEntOffset" },		// pfnPEntityOfEntOffset
	{ mFALSE, 19,	"EntOffsetOfPEntity" },		// pfnEntOffsetOfPEntity
	{ mFALSE, 14,	"IndexOfEdict" },		// pfnIndexOfEdict
	{ mFALSE, 17,	"PEntityOfEntIndex" },		// pfnPEntityOfEntIndex
	{ mFALSE, 9,	"FindEntityByVars" },		// pfnFindEntityByVars
	{ mFALSE, 14,	"GetModelPtr" },		// pfnGetModelPtr
	{ mFALSE, 9,	"RegUserMsg" },			// pfnRegUserMsg
	{ mFALSE, 9,	"AnimationAutomove" },		// pfnAnimationAutomove
	{ mFALSE, 9,	"GetBonePosition" },		// pfnGetBonePosition
	{ mFALSE, 9,	"FunctionFromName" },		// pfnFunctionFromName
	{ mFALSE, 9,	"NameForFunction" },		// pfnNameForFunction
	{ mFALSE, 9,	"ClientPrintf" },		// pfnClientPrintf
	{ mFALSE, 9,	"ServerPrint" },		// pfnServerPrint
	{ mFALSE, 13,	"Cmd_Args" },			// pfnCmd_Args
	{ mFALSE, 13,	"Cmd_Argv" },			// pfnCmd_Argv
	{ mFALSE, 13,	"Cmd_Argc" },			// pfnCmd_Argc
	{ mFALSE, 9,	"GetAttachment" },		// pfnGetAttachment
	{ mFALSE, 9,	"CRC32_Init" },			// pfnCRC32_Init
	{ mFALSE, 9,	"CRC32_ProcessBuffer" },	// pfnCRC32_ProcessBuffer
	{ mFALSE, 9,	"CRC32_ProcessByte" },		// pfnCRC32_ProcessByte
	{ mFALSE, 9,	"CRC32_Final" },		// pfnCRC32_Final
	{ mFALSE, 16,	"RandomLong" },			// pfnRandomLong
	{ mFALSE, 14,	"RandomFloat" },		// pfnRandomFloat		// CS: when firing
	{ mFALSE, 14,	"SetView" },			// pfnSetView
	{ mFALSE, 9,	"Time" },			// pfnTime
	{ mFALSE, 9,	"CrosshairAngle" },		// pfnCrosshairAngle
	{ mFALSE, 10,	"LoadFileForMe" },		// pfnLoadFileForMe
	{ mFALSE, 10,	"FreeFile" },			// pfnFreeFile
	{ mFALSE, 9,	"EndSection" },			// pfnEndSection
	{ mFALSE, 9,	"CompareFileTime" },		// pfnCompareFileTime
	{ mFALSE, 9,	"GetGameDir" },			// pfnGetGameDir
	{ mFALSE, 9,	"Cvar_RegisterVariable" },	// pfnCvar_RegisterVariable
	{ mFALSE, 9,	"FadeClientVolume" },		// pfnFadeClientVolume
	{ mFALSE, 14,	"SetClientMaxspeed" },		// pfnSetClientMaxspeed
	{ mFALSE, 9,	"CreateFakeClient" },		// pfnCreateFakeClient
	{ mFALSE, 9,	"RunPlayerMove" },		// pfnRunPlayerMove
	{ mFALSE, 9,	"NumberOfEntities" },		// pfnNumberOfEntities
	{ mFALSE, 17,	"GetInfoKeyBuffer" },		// pfnGetInfoKeyBuffer
	{ mFALSE, 13,	"InfoKeyValue" },		// pfnInfoKeyValue
	{ mFALSE, 9,	"SetKeyValue" },		// pfnSetKeyValue
	{ mFALSE, 12,	"SetClientKeyValue" },		// pfnSetClientKeyValue
	{ mFALSE, 9,	"IsMapValid" },			// pfnIsMapValid
	{ mFALSE, 9,	"StaticDecal" },		// pfnStaticDecal
	{ mFALSE, 9,	"PrecacheGeneric" },		// pfnPrecacheGeneric
	{ mFALSE, 10,	"GetPlayerUserId" },		// pfnGetPlayerUserId
	{ mFALSE, 9,	"BuildSoundMsg" },		// pfnBuildSoundMsg
	{ mFALSE, 9,	"IsDedicatedServer" },		// pfnIsDedicatedServer
	{ mFALSE, 9,	"CVarGetPointer" },		// pfnCVarGetPointer
	{ mFALSE, 9,	"GetPlayerWONId" },		// pfnGetPlayerWONId
	{ mFALSE, 9,	"Info_RemoveKey" },		// pfnInfo_RemoveKey
	{ mFALSE, 15,	"GetPhysicsKeyValue" },		// pfnGetPhysicsKeyValue
	{ mFALSE, 14,	"SetPhysicsKeyValue" },		// pfnSetPhysicsKeyValue
	{ mFALSE, 15,	"GetPhysicsInfoString" },	// pfnGetPhysicsInfoString
	{ mFALSE, 13,	"PrecacheEvent" },		// pfnPrecacheEvent
	{ mFALSE, 9,	"PlaybackEvent" },		// pfnPlaybackEvent
	{ mFALSE, 31,	"SetFatPVS" },			// pfnSetFatPVS
	{ mFALSE, 31,	"SetFatPAS" },			// pfnSetFatPAS
	{ mFALSE, 50,	"CheckVisibility" },		// pfnCheckVisibility
	{ mFALSE, 37,	"DeltaSetField" },		// pfnDeltaSetField
	{ mFALSE, 38,	"DeltaUnsetField" },		// pfnDeltaUnsetField
	{ mFALSE, 9,	"DeltaAddEncoder" },		// pfnDeltaAddEncoder
	{ mFALSE, 45,	"GetCurrentPlayer" },		// pfnGetCurrentPlayer
	{ mFALSE, 14,	"CanSkipPlayer" },		// pfnCanSkipPlayer
	{ mFALSE, 9,	"DeltaFindField" },		// pfnDeltaFindField
	{ mFALSE, 37,	"DeltaSetFieldByIndex" },	// pfnDeltaSetFieldByIndex
	{ mFALSE, 38,	"DeltaUnsetFieldByIndex" },	// pfnDeltaUnsetFieldByIndex
	{ mFALSE, 9,	"SetGroupMask" },		// pfnSetGroupMask
	{ mFALSE, 9,	"engCreateInstancedBaseline" },	// pfnCreateInstancedBaseline		// d'oh, CreateInstancedBaseline in dllapi too
	{ mFALSE, 9,	"Cvar_DirectSet" },		// pfnCvar_DirectSet
	{ mFALSE, 9,	"ForceUnmodified" },		// pfnForceUnmodified
	{ mFALSE, 9,	"GetPlayerStats" },		// pfnGetPlayerStats
	{ mFALSE, 3,	"AddServerCommand" },		// pfnAddServerCommand

	{ mFALSE, 9,	"Voice_GetClientListening" },	// Voice_GetClientListening
	{ mFALSE, 9,	"Voice_SetClientListening" },	// Voice_SetClientListening
	{ mFALSE, 9,	"GetPlayerAuthId" },		// pfnGetPlayerAuthId
	{ mFALSE, 30,	"SequenceGet" },		// pfnSequenceGet

	{ mFALSE, 30,	"SequencePickSentence" },		// pfnSequencePickSentence
	{ mFALSE, 30,	"GetFileSize" },			// pfnGetFileSize
	{ mFALSE, 30,	"GetApproxWavePlayLen" },		// pfnGetApproxWavePlayLen
	{ mFALSE, 30,	"IsCareerMatch" },			// pfnIsCareerMatch
	{ mFALSE, 30,	"GetLocalizedStringLength" },		// pfnGetLocalizedStringLength
	{ mFALSE, 30,	"RegisterTutorMessageShown" },		// pfnRegisterTutorMessageShown
	{ mFALSE, 30,	"GetTimesTutorMessageShown" },		// pfnGetTimesTutorMessageShown
	{ mFALSE, 30,	"ProcessTutorMessageDecayBuffer" },	// pfnProcessTutorMessageDecayBuffer
	{ mFALSE, 30,	"ConstructTutorMessageDecayBuffer" },	// pfnConstructTutorMessageDecayBuffer
	{ mFALSE, 9,	"ResetTutorMessageDecayData" },		// pfnResetTutorMessageDecayData

	{ mFALSE, 3,	"QueryClientCvarValue" },	//pfnQueryClientCvarValue
	{ mFALSE, 3,	"QueryClientCvarValue2" },	//pfnQueryClientCvarValue2
	{ mFALSE, 8,	"CheckParm" },			//pfnCheckParm

	// end
	{ mFALSE, 0,	NULL },
};
