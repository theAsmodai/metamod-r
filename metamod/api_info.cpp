// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// api_info.cpp - info for api routines

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

#include <extdll.h>			// always

#include "api_info.h"		// me
#include "api_hook.h"

// trace flag, loglevel, name
const dllapi_info_t dllapi_info = {
	{ mFALSE,  3,	api_caller_void_args_void, 	"GameDLLInit" },		// pfnGameInit
	{ mFALSE,  10,	api_caller_int_args_p, 		"DispatchSpawn" },		// pfnSpawn
	{ mFALSE,  16,	api_caller_void_args_p,		"DispatchThink" },		// pfnThink
	{ mFALSE,  9,	api_caller_void_args_2p,	"DispatchUse" },		// pfnUse
	{ mFALSE,  11,	api_caller_void_args_2p,	"DispatchTouch" },		// pfnTouch
	{ mFALSE,  9,	api_caller_void_args_2p,	"DispatchBlocked" },		// pfnBlocked
	{ mFALSE,  10,	api_caller_void_args_2p,	"DispatchKeyValue" },		// pfnKeyValue
	{ mFALSE,  9,	api_caller_void_args_2p,	"DispatchSave" },		// pfnSave
	{ mFALSE,  9,	api_caller_int_args_2pi,	"DispatchRestore" },		// pfnRestore
	{ mFALSE,  20,	api_caller_void_args_p,		"DispatchObjectCollsionBox" },	// pfnSetAbsBox
	{ mFALSE,  9,	api_caller_void_args_4pi,	"SaveWriteFields" },		// pfnSaveWriteFields
	{ mFALSE,  9,	api_caller_void_args_4pi,	"SaveReadFields" },		// pfnSaveReadFields
	{ mFALSE,  9,	api_caller_void_args_p,		"SaveGlobalState" },		// pfnSaveGlobalState
	{ mFALSE,  9,	api_caller_void_args_p,		"RestoreGlobalState" },		// pfnRestoreGlobalState
	{ mFALSE,  9,	api_caller_void_args_void, 	"ResetGlobalState" },	// pfnResetGlobalState
	{ mFALSE,  3,	api_caller_int_args_4p, 	"ClientConnect" },		// pfnClientConnect
	{ mFALSE,  3,	api_caller_void_args_p,		"ClientDisconnect" },	// pfnClientDisconnect
	{ mFALSE,  3,	api_caller_void_args_p,		"ClientKill" },			// pfnClientKill
	{ mFALSE,  3,	api_caller_void_args_p,		"ClientPutInServer" },	// pfnClientPutInServer
	{ mFALSE,  9,	api_caller_void_args_p,		"ClientCommand" },		// pfnClientCommand
	{ mFALSE,  11,	api_caller_void_args_2p,	"ClientUserInfoChanged" },	// pfnClientUserInfoChanged
	{ mFALSE,  3,	api_caller_void_args_p2i,	"ServerActivate" },		// pfnServerActivate
	{ mFALSE,  3,	api_caller_void_args_void,	"ServerDeactivate" },	// pfnServerDeactivate
	{ mFALSE,  14,	api_caller_void_args_p,		"PlayerPreThink" },		// pfnPlayerPreThink
	{ mFALSE,  14,	api_caller_void_args_p,		"PlayerPostThink" },	// pfnPlayerPostThink
	{ mFALSE,  18,	api_caller_void_args_void,	"StartFrame" },			// pfnStartFrame
	{ mFALSE,  9,	api_caller_void_args_void,	"ParmsNewLevel" },		// pfnParmsNewLevel
	{ mFALSE,  9,	api_caller_void_args_void,	"ParmsChangeLevel" },	// pfnParmsChangeLevel
	{ mFALSE,  9,	api_caller_ptr_args_void,	"GetGameDescription" },	// pfnGetGameDescription
	{ mFALSE,  9,	api_caller_void_args_2p,	"PlayerCustomization" },	// pfnPlayerCustomization
	{ mFALSE,  9,	api_caller_void_args_p,		"SpectatorConnect" },	// pfnSpectatorConnect
	{ mFALSE,  9,	api_caller_void_args_p,		"SpectatorDisconnect" },	// pfnSpectatorDisconnect
	{ mFALSE,  9,	api_caller_void_args_p,		"SpectatorThink" },		// pfnSpectatorThink
	{ mFALSE,  3,	api_caller_void_args_p,		"Sys_Error" },			// pfnSys_Error
	{ mFALSE,  13,	api_caller_void_args_pi,	"PM_Move" },			// pfnPM_Move
	{ mFALSE,  9,	api_caller_void_args_p,		"PM_Init" },			// pfnPM_Init
	{ mFALSE,  9,	api_caller_char_args_p,		"PM_FindTextureType" },	// pfnPM_FindTextureType
	{ mFALSE,  12,	api_caller_void_args_4p,	"SetupVisibility" },	// pfnSetupVisibility
	{ mFALSE,  12,	api_caller_void_args_pip,	"UpdateClientData" },	// pfnUpdateClientData
	{ mFALSE,  16,	api_caller_int_args_pi2p2ip,	"AddToFullPack" },		// pfnAddToFullPack
	{ mFALSE,  9,	api_caller_void_args_2i2pi2p,	"CreateBaseline" },		// pfnCreateBaseline
	{ mFALSE,  9,	api_caller_void_args_void,	"RegisterEncoders" },	// pfnRegisterEncoders
	{ mFALSE,  9,	api_caller_int_args_2p,		"GetWeaponData" },		// pfnGetWeaponData
	{ mFALSE,  15,	api_caller_void_args_2pui,	"CmdStart" },			// pfnCmdStart
	{ mFALSE,  15,	api_caller_void_args_p,		"CmdEnd" },				// pfnCmdEnd
	{ mFALSE,  9,	api_caller_int_args_4p,		"ConnectionlessPacket" },	// pfnConnectionlessPacket
	{ mFALSE,  9,	api_caller_int_args_i2p,	"GetHullBounds" },		// pfnGetHullBounds
	{ mFALSE,  9,	api_caller_void_args_void,	"CreateInstancedBaselines" },	// pfnCreateInstancedBaselines
	{ mFALSE,  3,	api_caller_int_args_3p,		"InconsistentFile" },	// pfnInconsistentFile
	{ mFALSE,  20,	api_caller_int_args_void,	"AllowLagCompensation" },	// pfnAllowLagCompensation
	{ mFALSE,  0,	NULL, 	NULL },
};

const newapi_info_t newapi_info = {
	{ mFALSE,  16,	api_caller_void_args_p,		"OnFreeEntPrivateData" },	// pfnOnFreeEntPrivateData
	{ mFALSE,  3,	api_caller_void_args_void,	"GameShutdown" },			// pfnGameShutdown
	{ mFALSE,  14,	api_caller_int_args_2p,		"ShouldCollide" },			// pfnShouldCollide
	// Added 2005/08/11 (no SDK update):
	{ mFALSE,  3,	api_caller_void_args_2p,	"CvarValue" },			// pfnCvarValue
	// Added 2005/11/21 (no SDK update):
	{ mFALSE,  3,	api_caller_void_args_pi2p,	"CvarValue2" },			// pfnCvarValue2
	{ mFALSE,  0,	NULL, 	NULL },
};

const engine_info_t engine_info = {
	{ mFALSE,  13,	api_caller_int_args_p,		"PrecacheModel" },		// pfnPrecacheModel
	{ mFALSE,  13,	api_caller_int_args_p,		"PrecacheSound" },		// pfnPrecacheSound
	{ mFALSE,  18,	api_caller_void_args_2p,	"SetModel" },			// pfnSetModel
	{ mFALSE,  34,	api_caller_int_args_p,		"ModelIndex" },			// pfnModelIndex
	{ mFALSE,  10,	api_caller_int_args_i,		"ModelFrames" },		// pfnModelFrames
	{ mFALSE,  14,	api_caller_void_args_3p,	"SetSize" },			// pfnSetSize
	{ mFALSE,  9,	api_caller_void_args_2p,	"ChangeLevel" },		// pfnChangeLevel
	{ mFALSE,  9,	api_caller_void_args_p,		"GetSpawnParms" },		// pfnGetSpawnParms
	{ mFALSE,  9,	api_caller_void_args_p,		"SaveSpawnParms" },		// pfnSaveSpawnParms
	{ mFALSE,  9,	api_caller_float_args_p,	"VecToYaw" },			// pfnVecToYaw
	{ mFALSE,  14,	api_caller_void_args_2p,	"VecToAngles" },		// pfnVecToAngles
	{ mFALSE,  9,	api_caller_void_args_2pfi,	"MoveToOrigin" },		// pfnMoveToOrigin
	{ mFALSE,  9,	api_caller_void_args_p,		"ChangeYaw" },			// pfnChangeYaw
	{ mFALSE,  9,	api_caller_void_args_p,		"ChangePitch" },		// pfnChangePitch
	{ mFALSE,  32,	api_caller_ptr_args_3p,		"FindEntityByString" },		// pfnFindEntityByString
	{ mFALSE,  9,	api_caller_int_args_p,		"GetEntityIllum" },		// pfnGetEntityIllum
	{ mFALSE,  9,	api_caller_ptr_args_2pf,	"FindEntityInSphere" },		// pfnFindEntityInSphere
	{ mFALSE,  19,	api_caller_ptr_args_p,		"FindClientInPVS" },		// pfnFindClientInPVS
	{ mFALSE,  9,	api_caller_ptr_args_p,		"EntitiesInPVS" },		// pfnEntitiesInPVS
	{ mFALSE,  40,	api_caller_void_args_p,		"MakeVectors" },		// pfnMakeVectors
	{ mFALSE,  9,	api_caller_void_args_4p,	"AngleVectors" },		// pfnAngleVectors
	{ mFALSE,  13,	api_caller_ptr_args_void,	"CreateEntity" },		// pfnCreateEntity
	{ mFALSE,  13,	api_caller_void_args_p,		"RemoveEntity" },		// pfnRemoveEntity
	{ mFALSE,  13,	api_caller_ptr_args_i,		"CreateNamedEntity" },		// pfnCreateNamedEntity
	{ mFALSE,  9,	api_caller_void_args_p,		"MakeStatic" },			// pfnMakeStatic
	{ mFALSE,  9,	api_caller_int_args_p,		"EntIsOnFloor" },		// pfnEntIsOnFloor
	{ mFALSE,  9,	api_caller_int_args_p,		"DropToFloor" },		// pfnDropToFloor
	{ mFALSE,  9,	api_caller_int_args_p2fi,	"WalkMove" },			// pfnWalkMove
	{ mFALSE,  14,	api_caller_void_args_2p,	"SetOrigin" },			// pfnSetOrigin
	{ mFALSE,  12,	api_caller_void_args_pip2f2i,	"EmitSound" },			// pfnEmitSound
	{ mFALSE,  12,	api_caller_void_args_3p2f2i,	"EmitAmbientSound" },		// pfnEmitAmbientSound
	{ mFALSE,  20,	api_caller_void_args_2pi2p,	"TraceLine" },			// pfnTraceLine
	{ mFALSE,  9,	api_caller_void_args_3p,	"TraceToss" },			// pfnTraceToss
	{ mFALSE,  9,	api_caller_int_args_3pi2p,	"TraceMonsterHull" },		// pfnTraceMonsterHull
	{ mFALSE,  9,	api_caller_void_args_2p2i2p,	"TraceHull" },			// pfnTraceHull
	{ mFALSE,  9,	api_caller_void_args_2pi2p,	"TraceModel" },			// pfnTraceModel
	{ mFALSE,  15,	api_caller_ptr_args_3p,		"TraceTexture" },		// pfnTraceTexture		// CS: when moving
	{ mFALSE,  9,	api_caller_void_args_2pif2p,	"TraceSphere" },		// pfnTraceSphere
	{ mFALSE,  9,	api_caller_void_args_pfp,	"GetAimVector" },		// pfnGetAimVector
	{ mFALSE,  9,	api_caller_void_args_p,		"ServerCommand" },		// pfnServerCommand
	{ mFALSE,  9,	api_caller_void_args_void,	"ServerExecute" },		// pfnServerExecute
	{ mFALSE,  11,	api_caller_void_args_2pV,	"engClientCommand" },		// pfnClientCommand		// d'oh, ClientCommand in dllapi too
	{ mFALSE,  9,	api_caller_void_args_2p2f,	"ParticleEffect" },		// pfnParticleEffect
	{ mFALSE,  9,	api_caller_void_args_ip,	"LightStyle" },			// pfnLightStyle
	{ mFALSE,  9,	api_caller_int_args_p,		"DecalIndex" },			// pfnDecalIndex
	{ mFALSE,  15,	api_caller_int_args_p,		"PointContents" },		// pfnPointContents		// CS: when moving
	{ mFALSE,  22,	api_caller_void_args_2i2p,	"MessageBegin" },		// pfnMessageBegin
	{ mFALSE,  22,	api_caller_void_args_void,	"MessageEnd" },			// pfnMessageEnd
	{ mFALSE,  30,	api_caller_void_args_i,		"WriteByte" },			// pfnWriteByte
	{ mFALSE,  23,	api_caller_void_args_i,		"WriteChar" },			// pfnWriteChar
	{ mFALSE,  24,	api_caller_void_args_i,		"WriteShort" },			// pfnWriteShort
	{ mFALSE,  23,	api_caller_void_args_i,		"WriteLong" },			// pfnWriteLong
	{ mFALSE,  23,	api_caller_void_args_f,		"WriteAngle" },			// pfnWriteAngle
	{ mFALSE,  23,	api_caller_void_args_f,		"WriteCoord" },			// pfnWriteCoord
	{ mFALSE,  25,	api_caller_void_args_p,		"WriteString" },		// pfnWriteString
	{ mFALSE,  23,	api_caller_void_args_i,		"WriteEntity" },		// pfnWriteEntity
	{ mFALSE,  9,	api_caller_void_args_p,		"CVarRegister" },		// pfnCVarRegister
	{ mFALSE,  21,	api_caller_float_args_p,	"CVarGetFloat" },		// pfnCVarGetFloat
	{ mFALSE,  9,	api_caller_ptr_args_p,		"CVarGetString" },		// pfnCVarGetString
	{ mFALSE,  10,	api_caller_void_args_pf,	"CVarSetFloat" },		// pfnCVarSetFloat
	{ mFALSE,  9,	api_caller_void_args_2p,	"CVarSetString" },		// pfnCVarSetString
	{ mFALSE,  15,	api_caller_void_args_ipV,	"AlertMessage" },		// pfnAlertMessage
	{ mFALSE,  17,	api_caller_void_args_2pV,	"EngineFprintf" },		// pfnEngineFprintf
	{ mFALSE,  14,	api_caller_ptr_args_pi,		"PvAllocEntPrivateData" },	// pfnPvAllocEntPrivateData
	{ mFALSE,  9,	api_caller_ptr_args_p,		"PvEntPrivateData" },		// pfnPvEntPrivateData
	{ mFALSE,  9,	api_caller_void_args_p,		"FreeEntPrivateData" },		// pfnFreeEntPrivateData
	{ mFALSE,  9,	api_caller_ptr_args_i,		"SzFromIndex" },		// pfnSzFromIndex
	{ mFALSE,  10,	api_caller_int_args_p,		"AllocString" },		// pfnAllocString
	{ mFALSE,  9,	api_caller_ptr_args_p,		"GetVarsOfEnt" },		// pfnGetVarsOfEnt
	{ mFALSE,  14,	api_caller_ptr_args_i,		"PEntityOfEntOffset" },		// pfnPEntityOfEntOffset
	{ mFALSE,  19,	api_caller_int_args_p,		"EntOffsetOfPEntity" },		// pfnEntOffsetOfPEntity
	{ mFALSE,  14,	api_caller_int_args_p,		"IndexOfEdict" },		// pfnIndexOfEdict
	{ mFALSE,  17,	api_caller_ptr_args_i,		"PEntityOfEntIndex" },		// pfnPEntityOfEntIndex
	{ mFALSE,  9,	api_caller_ptr_args_p,		"FindEntityByVars" },		// pfnFindEntityByVars
	{ mFALSE,  14,	api_caller_ptr_args_p,		"GetModelPtr" },		// pfnGetModelPtr
	{ mFALSE,  9,	api_caller_int_args_pi,		"RegUserMsg" },			// pfnRegUserMsg
	{ mFALSE,  9,	api_caller_void_args_pf,	"AnimationAutomove" },		// pfnAnimationAutomove
	{ mFALSE,  9,	api_caller_void_args_pi2p,	"GetBonePosition" },		// pfnGetBonePosition
	{ mFALSE,  9,	api_caller_uint_args_p,		"FunctionFromName" },		// pfnFunctionFromName
	{ mFALSE,  9,	api_caller_ptr_args_ui,		"NameForFunction" },		// pfnNameForFunction
	{ mFALSE,  9,	api_caller_void_args_pip,	"ClientPrintf" },		// pfnClientPrintf
	{ mFALSE,  9,	api_caller_void_args_p,		"ServerPrint" },		// pfnServerPrint
	{ mFALSE,  13,	api_caller_ptr_args_void,	"Cmd_Args" },			// pfnCmd_Args
	{ mFALSE,  13,	api_caller_ptr_args_i,		"Cmd_Argv" },			// pfnCmd_Argv
	{ mFALSE,  13,	api_caller_int_args_void,	"Cmd_Argc" },			// pfnCmd_Argc
	{ mFALSE,  9,	api_caller_void_args_pi2p,	"GetAttachment" },		// pfnGetAttachment
	{ mFALSE,  9,	api_caller_void_args_p,		"CRC32_Init" },			// pfnCRC32_Init
	{ mFALSE,  9,	api_caller_void_args_2pi,	"CRC32_ProcessBuffer" },	// pfnCRC32_ProcessBuffer
	{ mFALSE,  9,	api_caller_void_args_puc,	"CRC32_ProcessByte" },		// pfnCRC32_ProcessByte
	{ mFALSE,  9,	api_caller_ulong_args_ul,	"CRC32_Final" },		// pfnCRC32_Final
	{ mFALSE,  16,	api_caller_int_args_2i,		"RandomLong" },			// pfnRandomLong
	{ mFALSE,  14,	api_caller_float_args_2f,	"RandomFloat" },		// pfnRandomFloat		// CS: when firing
	{ mFALSE,  14,	api_caller_void_args_2p,	"SetView" },			// pfnSetView
	{ mFALSE,  9,	api_caller_float_args_void,	"Time" },			// pfnTime
	{ mFALSE,  9,	api_caller_void_args_p2f,	"CrosshairAngle" },		// pfnCrosshairAngle
	{ mFALSE,  10,	api_caller_ptr_args_2p,		"LoadFileForMe" },		// pfnLoadFileForMe
	{ mFALSE,  10,	api_caller_void_args_p,		"FreeFile" },			// pfnFreeFile
	{ mFALSE,  9,	api_caller_void_args_p,		"EndSection" },			// pfnEndSection
	{ mFALSE,  9,	api_caller_int_args_3p,		"CompareFileTime" },		// pfnCompareFileTime
	{ mFALSE,  9,	api_caller_void_args_p,		"GetGameDir" },			// pfnGetGameDir
	{ mFALSE,  9,	api_caller_void_args_p,		"Cvar_RegisterVariable" },	// pfnCvar_RegisterVariable
	{ mFALSE,  9,	api_caller_void_args_p4i,	"FadeClientVolume" },		// pfnFadeClientVolume
	{ mFALSE,  14,	api_caller_void_args_pf,	"SetClientMaxspeed" },		// pfnSetClientMaxspeed
	{ mFALSE,  9,	api_caller_ptr_args_p,		"CreateFakeClient" },		// pfnCreateFakeClient
	{ mFALSE,  9,	api_caller_void_args_2p3fus2uc,	"RunPlayerMove" },		// pfnRunPlayerMove
	{ mFALSE,  9,	api_caller_int_args_void,	"NumberOfEntities" },		// pfnNumberOfEntities
	{ mFALSE,  17,	api_caller_ptr_args_p,		"GetInfoKeyBuffer" },		// pfnGetInfoKeyBuffer
	{ mFALSE,  13,	api_caller_ptr_args_2p,		"InfoKeyValue" },		// pfnInfoKeyValue
	{ mFALSE,  9,	api_caller_void_args_3p,	"SetKeyValue" },		// pfnSetKeyValue
	{ mFALSE,  12,	api_caller_void_args_i3p,	"SetClientKeyValue" },		// pfnSetClientKeyValue
	{ mFALSE,  9,	api_caller_int_args_p,		"IsMapValid" },			// pfnIsMapValid
	{ mFALSE,  9,	api_caller_void_args_p3i,	"StaticDecal" },		// pfnStaticDecal
	{ mFALSE,  9,	api_caller_int_args_p,		"PrecacheGeneric" },		// pfnPrecacheGeneric
	{ mFALSE,  10,	api_caller_int_args_p,		"GetPlayerUserId" },		// pfnGetPlayerUserId
	{ mFALSE,  9,	api_caller_void_args_pip2f4i2p,	"BuildSoundMsg" },		// pfnBuildSoundMsg
	{ mFALSE,  9,	api_caller_int_args_void,	"IsDedicatedServer" },		// pfnIsDedicatedServer
	{ mFALSE,  9,	api_caller_ptr_args_p,		"CVarGetPointer" },		// pfnCVarGetPointer
	{ mFALSE,  9,	api_caller_uint_args_p,		"GetPlayerWONId" },		// pfnGetPlayerWONId
	{ mFALSE,  9,	api_caller_void_args_2p,	"Info_RemoveKey" },		// pfnInfo_RemoveKey
	{ mFALSE,  15,	api_caller_ptr_args_2p,		"GetPhysicsKeyValue" },		// pfnGetPhysicsKeyValue
	{ mFALSE,  14,	api_caller_void_args_3p,	"SetPhysicsKeyValue" },		// pfnSetPhysicsKeyValue
	{ mFALSE,  15,	api_caller_ptr_args_p,		"GetPhysicsInfoString" },	// pfnGetPhysicsInfoString
	{ mFALSE,  13,	api_caller_ushort_args_ip,	"PrecacheEvent" },		// pfnPrecacheEvent
	{ mFALSE,  9,	api_caller_void_args_ipusf2p2f4i,"PlaybackEvent" },		// pfnPlaybackEvent
	{ mFALSE,  31,	api_caller_ptr_args_p,		"SetFatPVS" },			// pfnSetFatPVS
	{ mFALSE,  31,	api_caller_ptr_args_p,		"SetFatPAS" },			// pfnSetFatPAS
	{ mFALSE,  50,	api_caller_int_args_2p,		"CheckVisibility" },		// pfnCheckVisibility
	{ mFALSE,  37,	api_caller_void_args_2p,	"DeltaSetField" },		// pfnDeltaSetField
	{ mFALSE,  38,	api_caller_void_args_2p,	"DeltaUnsetField" },		// pfnDeltaUnsetField
	{ mFALSE,  9,	api_caller_void_args_2p,	"DeltaAddEncoder" },		// pfnDeltaAddEncoder
	{ mFALSE,  45,	api_caller_int_args_void,	"GetCurrentPlayer" },		// pfnGetCurrentPlayer
	{ mFALSE,  14,	api_caller_int_args_p,		"CanSkipPlayer" },		// pfnCanSkipPlayer
	{ mFALSE,  9,	api_caller_int_args_2p,		"DeltaFindField" },		// pfnDeltaFindField
	{ mFALSE,  37,	api_caller_void_args_pi,	"DeltaSetFieldByIndex" },	// pfnDeltaSetFieldByIndex
	{ mFALSE,  38,	api_caller_void_args_pi,	"DeltaUnsetFieldByIndex" },	// pfnDeltaUnsetFieldByIndex
	{ mFALSE,  9,	api_caller_void_args_2i,	"SetGroupMask" },		// pfnSetGroupMask
	{ mFALSE,  9,	api_caller_int_args_ip,		"engCreateInstancedBaseline" },	// pfnCreateInstancedBaseline		// d'oh, CreateInstancedBaseline in dllapi too
	{ mFALSE,  9,	api_caller_void_args_2p,	"Cvar_DirectSet" },		// pfnCvar_DirectSet
	{ mFALSE,  9,	api_caller_void_args_i3p,	"ForceUnmodified" },		// pfnForceUnmodified
	{ mFALSE,  9,	api_caller_void_args_3p,	"GetPlayerStats" },		// pfnGetPlayerStats
	{ mFALSE,  3,	api_caller_void_args_2p,	"AddServerCommand" },		// pfnAddServerCommand
	// Added in SDK 2.2:
	{ mFALSE,  9,	api_caller_int_args_2i,		"Voice_GetClientListening" },	// Voice_GetClientListening
	{ mFALSE,  9,	api_caller_int_args_3i,		"Voice_SetClientListening" },	// Voice_SetClientListening
	// Added for HL 1109 (no SDK update):
	{ mFALSE,  9,	api_caller_ptr_args_p,		"GetPlayerAuthId" },		// pfnGetPlayerAuthId
	// Added 2003/11/10 (no SDK update):
	{ mFALSE,  30,	api_caller_ptr_args_2p,		"SequenceGet" },		// pfnSequenceGet
	{ mFALSE,  30,	api_caller_ptr_args_pip,	"SequencePickSentence" },	// pfnSequencePickSentence
	{ mFALSE,  30,	api_caller_int_args_p,		"GetFileSize" },		// pfnGetFileSize
	{ mFALSE,  30,	api_caller_uint_args_p,		"GetApproxWavePlayLen" },	// pfnGetApproxWavePlayLen
	{ mFALSE,  30,	api_caller_int_args_void,	"IsCareerMatch" },		// pfnIsCareerMatch
	{ mFALSE,  30,	api_caller_int_args_p,		"GetLocalizedStringLength" },	// pfnGetLocalizedStringLength
	{ mFALSE,  30,	api_caller_void_args_i,		"RegisterTutorMessageShown" },	// pfnRegisterTutorMessageShown
	{ mFALSE,  30,	api_caller_int_args_i,		"GetTimesTutorMessageShown" },	// pfnGetTimesTutorMessageShown
	{ mFALSE,  30,	api_caller_void_args_pi,	"ProcessTutorMessageDecayBuffer" },	// pfnProcessTutorMessageDecayBuffer
	{ mFALSE,  30,	api_caller_void_args_pi,	"ConstructTutorMessageDecayBuffer" },	// pfnConstructTutorMessageDecayBuffer
	{ mFALSE,  9,	api_caller_void_args_void,	"ResetTutorMessageDecayData" },	// pfnResetTutorMessageDecayData
	// Added 2005/08/11 (no SDK update):
	{ mFALSE,  3,	api_caller_void_args_2p,	"QueryClientCvarValue" },	// pfnQueryClientCvarValue
	// Added 2005/11/21 (no SDK update):
	{ mFALSE,  3,	api_caller_void_args_2pi,	"QueryClientCvarValue2" },	// pfnQueryClientCvarValue2
	// Added 2009-06-17 (no SDK update):
	{ mFALSE,  8,	api_caller_int_args_2p,		"EngCheckParm" },		// pfnEngCheckParm
	// end
	{ mFALSE,  0,   NULL,	NULL },
};
