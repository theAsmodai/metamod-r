
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// meta_eiface.cpp - wrapper for engine/dll interface

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

#include <extdll.h>				// always

#include <stdio.h>				// fprintf()
#include <stdlib.h>				// exit()
#include <string.h>				// memset(), memcpy()

#include "meta_eiface.h"		// me
#include "dllapi.h"				// FN_CVARVALUE, FN_CVARVALUE2
#include "engine_t.h"			// Engine


// ------------------------------------------------------------------------
// meta_new_dll_functions_t 
// ------------------------------------------------------------------------

// static member initialisation
int meta_new_dll_functions_t::sm_version DLLHIDDEN = 0;


meta_new_dll_functions_t::meta_new_dll_functions_t(
	void  (*_pfnOnFreeEntPrivateData) (edict_t*),
	void  (*_pfnGameShutdown)         (void),
	int   (*_pfnShouldCollide)        (edict_t*, edict_t*),
	void  (*_pfnCvarValue)            (const edict_t*, const char*),
	void  (*_pfnCvarValue2)           (const edict_t*, int, const char*, const char*)
	)
{
	pfnOnFreeEntPrivateData = _pfnOnFreeEntPrivateData;
	pfnGameShutdown = _pfnGameShutdown;
	pfnShouldCollide = _pfnShouldCollide;
	pfnCvarValue = _pfnCvarValue;
	pfnCvarValue2 = _pfnCvarValue2;

	memset( dummies, 0, sizeof(pdummyfunc) * c_NumDummies ); 
}



void DLLINTERNAL meta_new_dll_functions_t::copy_to( NEW_DLL_FUNCTIONS *_pFuncs )
{
	// This is where the magic happens. We check what version of the
	// NEW_DLL_FUNCTIONS interface the engine has and calculate the size of
	// that interface. Then we only copy the function pointers present in
	// that version over to the receiver, so that we do not overwrite his
	// memory with functions that he doesn't know of in his copy of the
	// struct.
	size_t size = get_size();

	if ( 0 == size ) {
		// Ok, this is a real problem and should *not* happen.
		// We try to work with NEW_DLL_FUNCTIONS without knowing what
		// interface the attached engine uses. This means that the classes
		// defined herein are not used in the way they are meant to because
		// someone forgot to first create a HL_enginefuncs_t object and
		// initialise it with the pointers passed from the engine.
		// We treat this as a major developer error and bluntly exit the
		// whole process, assuming that this will never happen on a
		// production server as it should have been caught by the developer
		// during testing.
		//
		// We use a printf() to complain since we do not know if we have
		// already attached to the engine and can use its alerting
		// functions. This should be augemnted with a windows version
		// popping open a message box.
		fprintf( stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
		fprintf( stderr, "ERROR: INTERNAL ERROR.\n" );
		fprintf( stderr, "       Attempt to use meta_new_dll_functions_t without initialised engine interface version!\n" );
		fprintf( stderr, "       %s at %d\n", __FILE__, __LINE__ );
		exit(1);
	}

	memcpy( _pFuncs, this, size );
}




int DLLINTERNAL meta_new_dll_functions_t::determine_interface_version( void )
{
	// If the meta_enginefuncs_t::version is 0, i.e. has not yet been
	// determined, that is a problem and an error. We should probably throw
	// a fit here or something.
	// For now we just return 0 and leave it to the caller to complain.
	if (meta_enginefuncs_t::version() == 0) return 0;

	// The default version is 1.
	sm_version = 1;
	 
	// With the enginefuncs interface version 156 the function
	// pfnCvarValue() was added, which we call version 2.
	if (meta_enginefuncs_t::version() >= 156) sm_version = 2;
	
	// With the enginefuncs interface version 157 the function
	// pfnCvarValue2() was added, which we call version 3.
	if (meta_enginefuncs_t::version() >= 157) sm_version = 3;
	
	return sm_version;
}


size_t DLLINTERNAL meta_new_dll_functions_t::get_size( int _version )
{
	size_t size = sizeof(NEW_DLL_FUNCTIONS);

	if ( 0 == _version ) {
		// Use the current engine's interface version
		_version = version();

		// Error: meta_enginefuncs_t::version probably not yet set up.
		if ( 0 == _version ) return 0;
	}

	switch( _version ) {
	case 1:
		// Version 1 is missing all functions from CvarValue() on.
		size -= sizeof(FN_CVARVALUE);
	case 2:
		// Version 2 is missing all functions from CvarValue2() on.
		size -= sizeof(FN_CVARVALUE2);
	}

	return size;
}




// --------------------------------------------------------------
// meta_enginefuncs_t
// --------------------------------------------------------------

// static member initialisation
int meta_enginefuncs_t::sm_version = 0;



meta_enginefuncs_t::meta_enginefuncs_t(
	int              (*_pfnPrecacheModel)                   (char*),
	int              (*_pfnPrecacheSound)                   (char*),
	void             (*_pfnSetModel)                        (edict_t*, const char*),
	int              (*_pfnModelIndex)                      (const char*),
	int              (*_pfnModelFrames)                     (int),
	void             (*_pfnSetSize)                         (edict_t*, const float*, const float*),
	void             (*_pfnChangeLevel)                     (char*, char*),
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
	edict_t*         (*_pfnCreateEntity)                    (void),
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
	void             (*_pfnServerExecute)                   (void),
	void             (*_pfnClientCommand)                   (edict_t*, char*, ...),
	void             (*_pfnParticleEffect)                  (const float*, const float*, float, float),
	void             (*_pfnLightStyle)                      (int, char*),
	int	         (*_pfnDecalIndex)                      (const char*),
	int              (*_pfnPointContents)                   (const float*),
	void             (*_pfnMessageBegin)                    (int, int, const float*, edict_t*),
	void             (*_pfnMessageEnd)                      (void),
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
	void             (*_pfnAlertMessage)                    (ALERT_TYPE, char*, ...),
	void             (*_pfnEngineFprintf)                   (void*, char*, ...),
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
	void             (*_pfnGetBonePosition)                 (const edict_t*, int, float*, float* ),
	uint32           (*_pfnFunctionFromName)                (const char*),
	const char*      (*_pfnNameForFunction)                 (uint32),
	void             (*_pfnClientPrintf)                    (edict_t*, PRINT_TYPE, const char*),
	void             (*_pfnServerPrint)                     (const char*),
	const char*      (*_pfnCmd_Args)                        (void),
	const char*      (*_pfnCmd_Argv)                        (int argc), 
	int	         (*_pfnCmd_Argc)                        (void),
	void             (*_pfnGetAttachment)                   (const edict_t*, int, float*, float*),
	void             (*_pfnCRC32_Init)                      (CRC32_t*),
	void             (*_pfnCRC32_ProcessBuffer)             (CRC32_t*, void*, int),
	void             (*_pfnCRC32_ProcessByte)               (CRC32_t*, unsigned char),
	CRC32_t          (*_pfnCRC32_Final)                     (CRC32_t),
	int32            (*_pfnRandomLong)                      (int32,  int32),
	float            (*_pfnRandomFloat)                     (float, float),
	void             (*_pfnSetView)                         (const edict_t*, const edict_t*),
	float            (*_pfnTime)                            (void),
	void             (*_pfnCrosshairAngle)                  (const edict_t*, float, float),
	byte*            (*_pfnLoadFileForMe)                   (char*, int*),
	void             (*_pfnFreeFile)                        (void*),
	void             (*_pfnEndSection)                      (const char*),
	int              (*_pfnCompareFileTime)                 (char*, char*, int*),
	void             (*_pfnGetGameDir)                      (char*),
	void             (*_pfnCvar_RegisterVariable)           (cvar_t*),
	void             (*_pfnFadeClientVolume)                (const edict_t*, int, int, int, int),
	void             (*_pfnSetClientMaxspeed)               (const edict_t*, float),
	edict_t*         (*_pfnCreateFakeClient)                (const char*),
	void             (*_pfnRunPlayerMove)                   (edict_t*, const float*, float, float, float, unsigned short, byte, byte),
	int              (*_pfnNumberOfEntities)                (void),
	char*            (*_pfnGetInfoKeyBuffer)                (edict_t*),
	char*            (*_pfnInfoKeyValue)                    (char*, char*),
	void             (*_pfnSetKeyValue)                     (char*, char*, char*),
	void             (*_pfnSetClientKeyValue)               (int, char*, char*, char*),
	int              (*_pfnIsMapValid)                      (char*),
	void             (*_pfnStaticDecal)                     (const float*, int, int, int),
	int              (*_pfnPrecacheGeneric)                 (char*),
	int	         (*_pfnGetPlayerUserId)                 (edict_t*),
	void             (*_pfnBuildSoundMsg)                   (edict_t*, int, const char*, float, float, int, int, int, int, const float*, edict_t*),
	int              (*_pfnIsDedicatedServer)               (void),
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
	int              (*_pfnCheckVisibility)                 (const edict_t*, unsigned char*),
	void             (*_pfnDeltaSetField)                   (struct delta_s*, const char*),
	void             (*_pfnDeltaUnsetField)                 (struct delta_s*, const char*),
	void             (*_pfnDeltaAddEncoder)                 (char*, void (*)(struct delta_s*, const unsigned char*, const unsigned char*)),
	int              (*_pfnGetCurrentPlayer)                (void),
	int              (*_pfnCanSkipPlayer)                   (const edict_t*),
	int              (*_pfnDeltaFindField)                  (struct delta_s*, const char*),
	void             (*_pfnDeltaSetFieldByIndex)            (struct delta_s*, int),
	void             (*_pfnDeltaUnsetFieldByIndex)          (struct delta_s*, int),
	void             (*_pfnSetGroupMask)                    (int, int),
	int              (*_pfnCreateInstancedBaseline)         (int, struct entity_state_s*),
	void             (*_pfnCvar_DirectSet)                  (struct cvar_s*, char*),
	void             (*_pfnForceUnmodified)                 (FORCE_TYPE, float*, float*, const char*),
	void             (*_pfnGetPlayerStats)                  (const edict_t*, int*, int*),
	void             (*_pfnAddServerCommand)                (char*, void (*) (void)),
	qboolean         (*_pfnVoice_GetClientListening)        (int, int),
	qboolean         (*_pfnVoice_SetClientListening)        (int, int, qboolean),
	const char*      (*_pfnGetPlayerAuthId)                 (edict_t*),
	sequenceEntry_s* (*_pfnSequenceGet)                     (const char*, const char*),
	sentenceEntry_s* (*_pfnSequencePickSentence)            (const char*, int, int*),
	int              (*_pfnGetFileSize)                     (char*),
	unsigned int     (*_pfnGetApproxWavePlayLen)            (const char*),
	int              (*_pfnIsCareerMatch)                   (void),
	int              (*_pfnGetLocalizedStringLength)        (const char*),
	void             (*_pfnRegisterTutorMessageShown)       (int),
	int              (*_pfnGetTimesTutorMessageShown)       (int),
	void             (*_pfnProcessTutorMessageDecayBuffer)  (int*, int),
	void             (*_pfnConstructTutorMessageDecayBuffer)(int*, int),
	void             (*_pfnResetTutorMessageDecayData)      (void),
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

	memset( extra_functions, 0, sizeof(extra_functions));

	memset( dummies, 0, sizeof(pdummyfunc) * c_NumDummies ); 
}
 


// -----------------------------------------------------------------
// HL_enginefuncs
// -----------------------------------------------------------------

void HL_enginefuncs_t::initialise_interface( enginefuncs_t *_pFuncs )
{
	set_from( _pFuncs );

	// Now the pfnAlertMessage is available and we trust it to be a valid
	// pointer, so flush the message buffer.
	flush_ALERT_buffer();

	determine_engine_interface_version();
	fixup_engine_interface();
}



// The following part (i.e. the end) of the enginefuncs_t struct  is 
// used to determine the engine interface version since it is the one
// that changed since SDK 212 engines. We call this the "signature" of
// the enginefuncs interface.
//
// Default version is 138. That's what the SDK says.
//
// 144:	const char *(*pfnGetPlayerAuthId)					( edict_t *e );
//
//		// PSV: Added for CZ training map
//	//	const char *(*pfnKeyNameForBinding)					( const char* pBinding );
//	
//		sequenceEntry_s*	(*pfnSequenceGet)				( const char* fileName, const char* entryName );
//  	sentenceEntry_s*	(*pfnSequencePickSentence)		( const char* groupName, int pickMethod, int *picked );
//	
//		// LH: Give access to filesize via filesystem
// 147:	int			(*pfnGetFileSize)						( char *filename );
//
//		unsigned int (*pfnGetApproxWavePlayLen)				(const char *filepath);
//		// MDC: Added for CZ career-mode
//		int			(*pfnIsCareerMatch)						( void );
//
//		// BGC: return the number of characters of the localized string referenced by using "label"
//		int			(*pfnGetLocalizedStringLength)			(const char *label);
//
//		// BGC: added to facilitate persistent storage of tutor message decay values for
//		// different career game profiles.  Also needs to persist regardless of mp.dll being
//		// destroyed and recreated.
//		void		(*pfnRegisterTutorMessageShown)			(int mid);
//		int			(*pfnGetTimesTutorMessageShown)			(int mid);
//		void		(*pfnProcessTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
//		void		(*pfnConstructTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
// 155:	void		(*pfnResetTutorMessageDecayData)		( void );

// 156:	void		(*pfnQueryClientCvarValue)		( const edict_t *player, const char *cvarName );
// 157:	void		(*pfnQueryClientCvarValue2)             ( const edict_t *player, const char *cvarName, int requestID );
// 158: int		(*pfnEngCheckParm)			( const char *pchCmdLineToke, char **pchNextValue );

void HL_enginefuncs_t::determine_engine_interface_version( void )
{
	// We only need to do this once.
	if (0 != sm_version) {
		return;
	}

	// Test every pointer that is part of the signature if it is a valid
	// pointer. If it is not, we set it explicitly to NULL.
	if ( ! Engine.info.is_valid_code_pointer(pfnGetPlayerAuthId) ) {
		pfnGetPlayerAuthId = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnSequenceGet) ) {
		pfnSequenceGet = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnSequencePickSentence) ) {
		pfnSequencePickSentence = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnGetFileSize) ) {
		pfnGetFileSize = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnGetApproxWavePlayLen) ) {
		pfnGetApproxWavePlayLen = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnIsCareerMatch) ) {
		pfnIsCareerMatch = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnGetLocalizedStringLength) ) {
		pfnGetLocalizedStringLength = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnRegisterTutorMessageShown) ) {
		pfnRegisterTutorMessageShown = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnGetTimesTutorMessageShown) ) {
		pfnGetTimesTutorMessageShown = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnProcessTutorMessageDecayBuffer) ) {
		pfnProcessTutorMessageDecayBuffer = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnConstructTutorMessageDecayBuffer) ) {
		pfnConstructTutorMessageDecayBuffer = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnResetTutorMessageDecayData) ) {
		pfnResetTutorMessageDecayData = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnQueryClientCvarValue) ) {
		pfnQueryClientCvarValue = NULL;
	}
	if ( ! Engine.info.is_valid_code_pointer(pfnQueryClientCvarValue2) ) {
		pfnQueryClientCvarValue2 = NULL;
	}

	// Now begins our heuristic, where we try to determine the engine
	// interface version.
	// As alluded to above we are currently only interested, and thus
	// only detect, versions 144, 147, 156 or 157 (defined by us).
	
	// The minimal default is 138.
	sm_version = 138;

	// If GetPlayerAuthId() is present, it is at least 144,
	// otherwise leave it at the default 138.
	// This may give incorrect results for *really* old engine versions,
	// i.e. pre 1.1.0.8. We live with that risk. No one uses them anymore.
	// Really.
	if ( pfnGetPlayerAuthId == NULL ) {
		return;
	}
	sm_version = 144;

	// The two function pointers for pfnSequenceGet() and
	// pfnSequencePickSentence() are only valid in a few engine versions
	// and are set to NULL in most other version, so they don't get
	// checked.

	// If pfnGetFileSize() is present, it is at least 147,
	// otherwise leave it at the so far determined value.
	if ( pfnGetFileSize == NULL ) {
		return;
	}
	sm_version = 147;

	// Now it gets a bit fuzzy. If all of the functions following GetFileSize()
	// but before QueryClientCvarValue() are valid, it is at least 155.
	// If even one of them is NULL, then our version can't be higher than 147,
	// so use 147. Actually, it could be that there exist engine
	// versions where one of them is NULL but the interface is still at
	// least 155. If such an engine is found in use, adaptions need to be
	// made.
	// (Yes, I know this could be done with a little hacky for() loop. We
	// don't need to do hacky here.)
	int cntInvals = 0;
	if ( pfnGetApproxWavePlayLen == NULL ) cntInvals++;
	if ( pfnIsCareerMatch == NULL ) cntInvals++;
	if ( pfnGetLocalizedStringLength == NULL ) cntInvals++;
	if ( pfnRegisterTutorMessageShown == NULL ) cntInvals++;
	if ( pfnGetTimesTutorMessageShown == NULL ) cntInvals++;
	if ( pfnProcessTutorMessageDecayBuffer == NULL ) cntInvals++;
	if ( pfnConstructTutorMessageDecayBuffer == NULL ) cntInvals++;
	if ( pfnResetTutorMessageDecayData == NULL ) cntInvals++;

	if ( cntInvals > 0 ) {
		return;
	}
	sm_version = 155;

	// All functions up to QueryClientCvarValue() are valid.
	// If QueryClientCvarValue() is not valid, leave it at the so far 
	// determined version. Otherwise the version is at least 156.
	if ( pfnQueryClientCvarValue == NULL) {
		return;
	}
	sm_version = 156;

	// All functions up to QueryClientCvarValue2() are valid.
	// If QueryClientCvarValue2() is not valid, leave it at the so far 
	// determined version.  Otherwise the version is at least 157.
	if ( pfnQueryClientCvarValue2 == NULL) {
		return;
	}
	sm_version = 157;

	// All functions up to EngCheckParm() are valid.
	// If EngCheckParm() is not valid, leave it at the so far determined
	// version. Otherwise the version is at least 158.
	if ( pfnEngCheckParm == NULL) {
		return;
	}
	sm_version = 158;
}





void HL_enginefuncs_t::fixup_engine_interface( void )
{
	// This function will make sure that all function pointers that aren't
	// valid are set to NULL, depending on the engine interface version.
	// Sometimes a pointer has a valid value although the function doesn't
	// exist in the interface version.

	switch ( version() ) {
	case 138:
		pfnGetPlayerAuthId = NULL;
	case 144:
		pfnSequenceGet = NULL;
		pfnSequencePickSentence = NULL;
		pfnGetFileSize = NULL;
	case 147:
		pfnGetApproxWavePlayLen = NULL;
		pfnIsCareerMatch = NULL;
		pfnGetLocalizedStringLength = NULL;
		pfnRegisterTutorMessageShown = NULL;
		pfnGetTimesTutorMessageShown = NULL;
		pfnProcessTutorMessageDecayBuffer = NULL;
		pfnConstructTutorMessageDecayBuffer = NULL;
		pfnResetTutorMessageDecayData = NULL;
	case 155:
		pfnQueryClientCvarValue = NULL;
	case 156:
		pfnQueryClientCvarValue2 = NULL;
	case 157:
		pfnEngCheckParm = NULL;
	}
}


