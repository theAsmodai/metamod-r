
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// meta_eiface.h - wrapper for engine/dll interface

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

#ifndef MM_META_EIFACE_H
#define MM_META_EIFACE_H

#include <extdll.h>				// NEW_DLL_FUNCTIONS, enginefuncs_t
#include <string.h>             // memset()

#include "comp_dep.h"
#include "osdep.h"	//unlikely, OPEN_ARGS


// We use our own versions of the engine/dll interface structs. We add a
// few dummy entries to the end and set them to 0. That way we are
// protected from updates to the HL SDK adding new functions which would
// cause a) the game dll copying arbitrary values from us and b) the game
// dll overwriting our memory when using an old Metamod with a new game
// dll. 

const int c_NumDummies = 5;
typedef void (*pdummyfunc)(void);


// --------------------------------------------------------------------
// meta_new_dll_functions_t
// --------------------------------------------------------------------

struct meta_new_dll_functions_t : public NEW_DLL_FUNCTIONS {
	// Array of five dummy function pointers. Must be filled with NULL.
        pdummyfunc dummies[c_NumDummies];

	// functions :
	meta_new_dll_functions_t() DLLINTERNAL;
	
	meta_new_dll_functions_t(
		void  (*pfnOnFreeEntPrivateData) (edict_t*),
		void  (*pfnGameShutdown)         (void),
		int   (*pfnShouldCollide)        (edict_t*, edict_t*),
		void  (*pfnCvarValue)            (const edict_t*, const char*),
		void  (*pfnCvarValue2)           (const edict_t*, int, const char*, const char*)
        ) DLLINTERNAL;

        meta_new_dll_functions_t( const meta_new_dll_functions_t& ) DLLINTERNAL;
        meta_new_dll_functions_t& operator=( const meta_new_dll_functions_t& ) DLLINTERNAL;

	// Fill this object with pointers copied from a NEW_DLL_FUNCTIONS struct.
	void DLLINTERNAL set_from( NEW_DLL_FUNCTIONS* pFuncs );

	// Copy the pointers from this object to a NEW_DLL_FUNCTIONS struct.
	void DLLINTERNAL copy_to( NEW_DLL_FUNCTIONS* pFuncs );

	// return the engine's version of NEW_DLL_FUNCTIONS
	int DLLINTERNAL version( void );

	private:

	// data :

		// The NEW_DLL_FUNCTIONS struct also changed, but the version
		// number did not change. That begs the question why to have 
		// it versioned in the first place, but whaddaya know.
		// While the official version is left at 1, we internally
		// calculate a different version of the engine's NEW_DLL_FUNCTIONS
		// struct since we know that the engine lies to us about the
		// version that it uses.
		//
		// The default version is 1.
		// 
		// With the enginefuncs interface version 156 the function
		// pfnCvarValue() was added, which we call version 2.
		//
		// With the enginefuncs interface version 157 the function
		// pfnCvarValue2() was added, which we call version 3.
		//
		// If Valve ever decides to change the version of the
		// NEW_DLL_FUNCTIONS interface in the future (haha),
		// we are in trouble and will need to change our
		// internal versions.

		static int sm_version;	

	// functions :

		// Calculates our idea of the engine's version of the
		// NEW_DLL_FUNCTIONS interface. Stores this version for future
		// reference in m_version and returns it.
		int DLLINTERNAL determine_interface_version( void );

		// Comfort function to determine the size of the NEW_DLL_FUNCTIONS
		// struct for the different versions.
		// If passed a version number other than 0, the size for that
		// specific version is returned. 
		// If passed 0 as version number (default) the size for the version
		// that was determined to be the version of the currently connected
		// engine's interface. Should that version have not yet been
		// determined (via the enginefuncs_t interface), 0 is returned to
		// indicated this error state.
		size_t DLLINTERNAL get_size( int version = 0 );
};


// Inline functions

inline meta_new_dll_functions_t::meta_new_dll_functions_t() 
{ 
	memset( this, 0, sizeof(meta_new_dll_functions_t) ); 
}


inline meta_new_dll_functions_t::meta_new_dll_functions_t( const meta_new_dll_functions_t& _rhs )
{
	memcpy( this, &_rhs, sizeof(NEW_DLL_FUNCTIONS) );
	memset( dummies, 0, sizeof(pdummyfunc) * c_NumDummies ); 
}


inline meta_new_dll_functions_t& meta_new_dll_functions_t::operator=( const meta_new_dll_functions_t& _rhs)
{
	memcpy( this, &_rhs, sizeof(NEW_DLL_FUNCTIONS) );
	return *this;
}


inline void meta_new_dll_functions_t::set_from( NEW_DLL_FUNCTIONS* _pFuncs )
{
	memcpy( this, _pFuncs, sizeof(NEW_DLL_FUNCTIONS) );
}


inline int meta_new_dll_functions_t::version( void ) 
{ 
	return sm_version ? sm_version : determine_interface_version(); 
}



// No meta version of DLL_FUNCTIONS because that won't be changing anymore.



// --------------------------------------------------------------------
// meta_enginefuncs_t
// --------------------------------------------------------------------


struct meta_enginefuncs_t : public enginefuncs_t {
	// data :

	// Array of five dummy function pointers. Must be filled with NULL.
	pdummyfunc dummies[c_NumDummies];

	// functions :
	meta_enginefuncs_t() DLLINTERNAL;

	// Spawn of the devil
	meta_enginefuncs_t(
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
		int	             (*_pfnWalkMove)                        (edict_t*, float, float, int),
		void             (*_pfnSetOrigin)                       (edict_t*, const float*),
		void             (*_pfnEmitSound)                       (edict_t*, int, const char*, float, float, int, int),
		void             (*_pfnEmitAmbientSound)                (edict_t*, float*, const char*, float, float, int, int),
		void             (*_pfnTraceLine)                       (const float*, const float*, int, edict_t*, TraceResult*),
		void             (*_pfnTraceToss)                       (edict_t*, edict_t*, TraceResult*),
		int	             (*_pfnTraceMonsterHull)                (edict_t*, const float*, const float*, int, edict_t*, TraceResult*),
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
		int	             (*_pfnDecalIndex)                      (const char*),
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
		int              (*_pfnAllocString)	                    (const char*),
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
		int	             (*_pfnCmd_Argc)                        (void),
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
		int	             (*_pfnGetPlayerUserId)                 (edict_t*),
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
		int              (*_pfnEngCheckParm)                    (const char *, char**)
        ) DLLINTERNAL;

        meta_enginefuncs_t( const meta_enginefuncs_t& ) DLLINTERNAL;
        meta_enginefuncs_t& operator=( const meta_enginefuncs_t& ) DLLINTERNAL;

	// Fill this object with pointers copied from an enginefuncs_t struct.
	void DLLINTERNAL set_from( enginefuncs_t *pFuncs );

	// Copy the pointers from this object to an enginefuncs_t struct.
	void DLLINTERNAL copy_to( enginefuncs_t *pFuncs );

	// return the engine interface version
	static int DLLINTERNAL version( void );

	protected:

	// data :

	// The version of the engine functions interface. It is frozen at 138. But no one knows
	// when that was and what it looked like then. So we simply interprete it as the
	// number of functions that the enginefuncs struct contains.
	// 
	// That means we get gaps inbetween versions and right now we can detect only
	// about five different versions anyway, but that suffices for the current itches
	// to get scratched.
	//
	// The default is hence 138.
	// A value of 0 means "not yet determined".
	// Other possible versions currently detectable:
	// 144: engine versions after 1.1.0.9 build 1996
	// 147: engine versions after build 2384 with pfnGetFileSize()
	// 155: all versions between build 2384 and the one 
	//      including pfnQueryClientCvarValue()
	// 156: includes pfnQueryClientCvarValue()
	// 157: includes pfnQueryClientCvarValue2()
	// 158: includes pfnEngCheckParm()
	static int sm_version DLLHIDDEN;	

};

//
// Inline functions
//

inline meta_enginefuncs_t::meta_enginefuncs_t() 
{ 
	memset( this, 0, sizeof(meta_enginefuncs_t) ); 
}


inline meta_enginefuncs_t::meta_enginefuncs_t( const meta_enginefuncs_t& _rhs )
{
	memcpy( this, &_rhs, sizeof(enginefuncs_t) );
	memset( dummies, 0, sizeof(pdummyfunc) * c_NumDummies ); 
}


inline meta_enginefuncs_t& meta_enginefuncs_t::operator=( const meta_enginefuncs_t& _rhs)
{
	memcpy( this, &_rhs, sizeof(enginefuncs_t) );
	return *this;
}


inline void meta_enginefuncs_t::set_from( enginefuncs_t* _pFuncs )
{
	memcpy( this, _pFuncs, sizeof(enginefuncs_t) );
}


inline void meta_enginefuncs_t::copy_to( enginefuncs_t* _pFuncs )
{
	memcpy( _pFuncs, this, sizeof(enginefuncs_t) );
}


inline int meta_enginefuncs_t::version( void ) 
{ 
	return sm_version; 
}



// --------------------------------------------------------------------
// HL_enginefuncs_t
// --------------------------------------------------------------------

//
// This is a specialisation of the meta_enginefuncs_t struct which is only
// used for the initial copy of the engine functions, i.e. those we get
// passed from the HL engine right at the beginning. 
// This specialisation does some extra initialisation when getting set up
// like calculating the engine interface version and fixing up any invalid
// pointers.
// Since there is only one master copy of engine functions this could be
// implemented as a singleton. This is left as an option for later.
//
struct HL_enginefuncs_t : public meta_enginefuncs_t {

	// functions :
	HL_enginefuncs_t() DLLINTERNAL;

	// Fill this object with pointers copied from an enginefuncs_t struct
	// and fixup the interface.
	// For this class this happens in the GiveFptrsToDll() function
	// with the pointers passed from the HL engine.
	void initialise_interface( enginefuncs_t *pFuncs ) DLLINTERNAL;

	private:

	// functions :

	// Moving copy_to() and set_from() to the private space.
	void DLLINTERNAL set_from( enginefuncs_t *pFuncs ) { meta_enginefuncs_t::set_from( pFuncs ); };
	void DLLINTERNAL copy_to( enginefuncs_t *pFuncs ) { meta_enginefuncs_t::copy_to( pFuncs ); };	

	// Determine the version of the engine interface from the
	// enginefuncs signature.
	void DLLINTERNAL determine_engine_interface_version( void );

	// Fixup the enginefuncs pointers according to the determined
	// version as some pointers may be invalid.
	void DLLINTERNAL fixup_engine_interface( void );
};


inline HL_enginefuncs_t :: HL_enginefuncs_t() : meta_enginefuncs_t() { };


#endif /* META_EIFACE_H */

