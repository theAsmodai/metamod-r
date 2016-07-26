#pragma once

#include <extdll.h>		// NEW_DLL_FUNCTIONS, enginefuncs_t
#include <string.h>		// memset()

#include "comp_dep.h"

// We use our own versions of the engine/dll interface structs. We add a
// few dummy entries to the end and set them to 0. That way we are
// protected from updates to the HL SDK adding new functions which would
// cause a) the game dll copying arbitrary values from us and b) the game
// dll overwriting our memory when using an old Metamod with a new game
// dll.

// meta_new_dll_functions_t
struct meta_new_dll_functions_t: public NEW_DLL_FUNCTIONS {
public:
// functions:
	meta_new_dll_functions_t();
	meta_new_dll_functions_t(
		void (*pfnOnFreeEntPrivateData)	(edict_t *),
		void (*pfnGameShutdown)		(),
		int (*pfnShouldCollide)		(edict_t *, edict_t *),
		void (*pfnCvarValue)		(const edict_t *, const char *),
		void (*pfnCvarValue2)		(const edict_t *, int, const char *, const char *)
	);

	meta_new_dll_functions_t(const meta_new_dll_functions_t&);
	meta_new_dll_functions_t& operator=(const meta_new_dll_functions_t&);

	// Fill this object with pointers copied from a NEW_DLL_FUNCTIONS struct.
	void set_from(NEW_DLL_FUNCTIONS *pFuncs);

	// Copy the pointers from this object to a NEW_DLL_FUNCTIONS struct.
	void copy_to(NEW_DLL_FUNCTIONS *pFuncs);
};

// Inline functions
inline meta_new_dll_functions_t::meta_new_dll_functions_t()
{
	Q_memset(this, 0, sizeof(meta_new_dll_functions_t));
}

inline meta_new_dll_functions_t::meta_new_dll_functions_t(const meta_new_dll_functions_t& _rhs)
{
	Q_memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
}

inline meta_new_dll_functions_t& meta_new_dll_functions_t::operator=(const meta_new_dll_functions_t& _rhs)
{
	Q_memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
	return *this;
}

inline void meta_new_dll_functions_t::set_from(NEW_DLL_FUNCTIONS* _pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(NEW_DLL_FUNCTIONS));
}

// meta_enginefuncs_t
struct meta_enginefuncs_t: public enginefuncs_t {
public:
	// functions:
	meta_enginefuncs_t();

	// Spawn of the devil
	meta_enginefuncs_t(
		int (*_pfnPrecacheModel)(const char *),
		int (*_pfnPrecacheSound)(const char *),
		void (*_pfnSetModel)(edict_t *, const char *),
		int (*_pfnModelIndex)(const char *),
		int (*_pfnModelFrames)(int),
		void (*_pfnSetSize)(edict_t *, const float *, const float *),
		void (*_pfnChangeLevel)(const char *, const char *),
		void (*_pfnGetSpawnParms)(edict_t *),
		void (*_pfnSaveSpawnParms)(edict_t *),
		float (*_pfnVecToYaw)(const float *),
		void (*_pfnVecToAngles)(const float *, float *),
		void (*_pfnMoveToOrigin)(edict_t *, const float *, float, int),
		void (*_pfnChangeYaw)(edict_t *),
		void (*_pfnChangePitch)(edict_t *),
		edict_t *(*_pfnFindEntityByString)(edict_t *, const char *, const char *),
		int (*_pfnGetEntityIllum)(edict_t *),
		edict_t *(*_pfnFindEntityInSphere)(edict_t *, const float *, float),
		edict_t *(*_pfnFindClientInPVS)(edict_t *),
		edict_t *(*_pfnEntitiesInPVS)(edict_t *),
		void (*_pfnMakeVectors)(const float *),
		void (*_pfnAngleVectors)(const float *, float *, float *, float *),
		edict_t *(*_pfnCreateEntity)(),
		void (*_pfnRemoveEntity)(edict_t *),
		edict_t *(*_pfnCreateNamedEntity)(int),

		void (*_pfnMakeStatic)(edict_t *),
		int (*_pfnEntIsOnFloor)(edict_t *),
		int (*_pfnDropToFloor)(edict_t *),
		int (*_pfnWalkMove)(edict_t *, float, float, int),
		void (*_pfnSetOrigin)(edict_t *, const float *),
		void (*_pfnEmitSound)(edict_t *, int, const char *, float, float, int, int),
		void (*_pfnEmitAmbientSound)(edict_t *, float *, const char *, float, float, int, int),
		void (*_pfnTraceLine)(const float *, const float *, int, edict_t *, TraceResult*),
		void (*_pfnTraceToss)(edict_t*, edict_t*, TraceResult *),
		int (*_pfnTraceMonsterHull)(edict_t *, const float*, const float*, int, edict_t *, TraceResult *),
		void (*_pfnTraceHull)(const float *, const float*, int, int, edict_t*, TraceResult *),
		void (*_pfnTraceModel)(const float *, const float*, int, edict_t *, TraceResult *),
		const char *(*_pfnTraceTexture)(edict_t *, const float*, const float*),
		void (*_pfnTraceSphere)(const float *, const float*, int, float, edict_t*, TraceResult*),
		void (*_pfnGetAimVector)(edict_t *, float, float*),
		void (*_pfnServerCommand)(char*),
		void (*_pfnServerExecute)(),
		void (*_pfnClientCommand)(edict_t *, char *, ...),
		void (*_pfnParticleEffect)(const float *, const float *, float, float),
		void (*_pfnLightStyle)(int, char *),
		int (*_pfnDecalIndex)(const char *),
		int (*_pfnPointContents)(const float *),
		void (*_pfnMessageBegin)(int, int, const float*, edict_t*),
		void (*_pfnMessageEnd)(),

		void (*_pfnWriteByte)(int),
		void (*_pfnWriteChar)(int),
		void (*_pfnWriteShort)(int),
		void (*_pfnWriteLong)(int),
		void (*_pfnWriteAngle)(float),
		void (*_pfnWriteCoord)(float),
		void (*_pfnWriteString)(const char *),
		void (*_pfnWriteEntity)(int),
		void (*_pfnCVarRegister)(cvar_t *),
		float (*_pfnCVarGetFloat)(const char *),
		const char *(*_pfnCVarGetString)(const char *),
		void (*_pfnCVarSetFloat)(const char *, float),
		void (*_pfnCVarSetString)(const char *, const char*),
		void (*_pfnAlertMessage)(ALERT_TYPE, const char *, ...),
		void (*_pfnEngineFprintf)(void *, const char *, ...),

		void *(*_pfnPvAllocEntPrivateData)(edict_t *, int32),
		void *(*_pfnPvEntPrivateData)(edict_t *),
		void (*_pfnFreeEntPrivateData)(edict_t *),
		const char *(*_pfnSzFromIndex)(int),
		int (*_pfnAllocString)(const char *),
		struct entvars_s*(*_pfnGetVarsOfEnt)(edict_t *),
		edict_t *(*_pfnPEntityOfEntOffset)(int),
		int (*_pfnEntOffsetOfPEntity)(const edict_t *),
		int (*_pfnIndexOfEdict)(const edict_t *),
		edict_t *(*_pfnPEntityOfEntIndex)(int),
		edict_t *(*_pfnFindEntityByVars)(struct entvars_s *),
		void *(*_pfnGetModelPtr)(edict_t *),
		int (*_pfnRegUserMsg)(const char *, int),
		void (*_pfnAnimationAutomove)(const edict_t *, float),
		void (*_pfnGetBonePosition)(const edict_t *, int, float *, float *),
		uint32 (*_pfnFunctionFromName)(const char*),
		const char *(*_pfnNameForFunction)(uint32),
		void (*_pfnClientPrintf)(edict_t *, PRINT_TYPE, const char *),
		void (*_pfnServerPrint)(const char *),
		const char *(*_pfnCmd_Args)(),
		const char *(*_pfnCmd_Argv)(int argc),
		int (*_pfnCmd_Argc)(),
		void (*_pfnGetAttachment)(const edict_t *, int, float *, float *),
		void (*_pfnCRC32_Init)(CRC32_t *),
		void (*_pfnCRC32_ProcessBuffer)(CRC32_t *, void *, int),
		void (*_pfnCRC32_ProcessByte)(CRC32_t *, unsigned char),
		CRC32_t (*_pfnCRC32_Final)(CRC32_t),
		int32 (*_pfnRandomLong)(int32, int32),
		float (*_pfnRandomFloat)(float, float),
		void (*_pfnSetView)(const edict_t *, const edict_t *),
		float (*_pfnTime)(),
		void (*_pfnCrosshairAngle)(const edict_t *, float, float),
		byte *(*_pfnLoadFileForMe)(char *, int *),
		void (*_pfnFreeFile)(void *),

		void (*_pfnEndSection)(const char *),
		int (*_pfnCompareFileTime)(char *, char *, int *),
		void (*_pfnGetGameDir)(char *),
		void (*_pfnCvar_RegisterVariable)(cvar_t *),
		void (*_pfnFadeClientVolume)(const edict_t *, int, int, int, int),
		void (*_pfnSetClientMaxspeed)(edict_t *, float),
		edict_t *(*_pfnCreateFakeClient)(const char *),
		void (*_pfnRunPlayerMove)(edict_t *, const float *, float, float, float, unsigned short, byte, byte),
		int (*_pfnNumberOfEntities)(),
		char *(*_pfnGetInfoKeyBuffer)(edict_t *),
		char *(*_pfnInfoKeyValue)(char *, const char *),
		void (*_pfnSetKeyValue)(char *, const char *, const char *),
		void (*_pfnSetClientKeyValue)(int, char *, const char *, const char *),
		int (*_pfnIsMapValid)(char *),
		void (*_pfnStaticDecal)(const float *, int, int, int),
		int (*_pfnPrecacheGeneric)(char *),
		int (*_pfnGetPlayerUserId)(edict_t *),
		void (*_pfnBuildSoundMsg)(edict_t *, int, const char*, float, float, int, int, int, int, const float *, edict_t *),
		int (*_pfnIsDedicatedServer)(),
		cvar_t *(*_pfnCVarGetPointer)(const char *),
		unsigned int (*_pfnGetPlayerWONId)(edict_t *),

		void (*_pfnInfo_RemoveKey)(char *, const char *),
		const char *(*_pfnGetPhysicsKeyValue)(const edict_t *, const char *),
		void (*_pfnSetPhysicsKeyValue)(const edict_t *, const char *, const char *),
		const char *(*_pfnGetPhysicsInfoString)(const edict_t *),
		unsigned short (*_pfnPrecacheEvent)(int, const char *),
		void (*_pfnPlaybackEvent)(int, const edict_t *, unsigned short, float, float *, float *, float, float, int, int, int, int),
		unsigned char *(*_pfnSetFatPVS)(float *),
		unsigned char *(*_pfnSetFatPAS)(float *),
		int (*_pfnCheckVisibility)(edict_t *, unsigned char *),

		void (*_pfnDeltaSetField)(struct delta_s *, const char *),
		void (*_pfnDeltaUnsetField)(struct delta_s *, const char *),
		void (*_pfnDeltaAddEncoder)(char *, void (*)(struct delta_s *, const unsigned char *, const unsigned char *)),
		int (*_pfnGetCurrentPlayer)(),
		int (*_pfnCanSkipPlayer)(const edict_t *),
		int (*_pfnDeltaFindField)(struct delta_s *, const char *),
		void (*_pfnDeltaSetFieldByIndex)(struct delta_s *, int),
		void (*_pfnDeltaUnsetFieldByIndex)(struct delta_s *, int),
		void (*_pfnSetGroupMask)(int, int),
		int (*_pfnCreateInstancedBaseline)(int, struct entity_state_s *),
		void (*_pfnCvar_DirectSet)(struct cvar_s *, const char *),

		void (*_pfnForceUnmodified)(FORCE_TYPE, float *, float *, const char *),
		void (*_pfnGetPlayerStats)(const edict_t *, int *, int *),
		void (*_pfnAddServerCommand)(char*, void (*)()),

		qboolean (*_pfnVoice_GetClientListening)(int, int),
		qboolean (*_pfnVoice_SetClientListening)(int, int, qboolean),
		const char *(*_pfnGetPlayerAuthId)(edict_t *),
		sequenceEntry_s *(*_pfnSequenceGet)(const char *, const char *),
		sentenceEntry_s *(*_pfnSequencePickSentence)(const char *, int, int *),
		int (*_pfnGetFileSize)(char *),
		unsigned int (*_pfnGetApproxWavePlayLen)(const char *),
		int (*_pfnIsCareerMatch)(),
		int (*_pfnGetLocalizedStringLength)(const char *),
		void (*_pfnRegisterTutorMessageShown)(int),
		int (*_pfnGetTimesTutorMessageShown)(int),
		void (*_pfnProcessTutorMessageDecayBuffer)(int*, int),
		void (*_pfnConstructTutorMessageDecayBuffer)(int*, int),
		void (*_pfnResetTutorMessageDecayData)(),
		void (*_pfnQueryClientCvarValue)(const edict_t *, const char *),
		void (*_pfnQueryClientCvarValue2)(const edict_t *, const char *, int),
		int (*_pfnEngCheckParm)(const char *, char**)
	);

	meta_enginefuncs_t(const meta_enginefuncs_t &);
	meta_enginefuncs_t& operator=(const meta_enginefuncs_t &);

	// Fill this object with pointers copied from an enginefuncs_t struct.
	void set_from(enginefuncs_t *pFuncs);

	// Copy the pointers from this object to an enginefuncs_t struct.
	void copy_to(enginefuncs_t *pFuncs);
};

inline meta_enginefuncs_t::meta_enginefuncs_t()
{
	Q_memset(this, 0, sizeof(meta_enginefuncs_t));
}

inline meta_enginefuncs_t::meta_enginefuncs_t(const meta_enginefuncs_t &_rhs)
{
	Q_memcpy(this, &_rhs, sizeof(enginefuncs_t));
}

inline meta_enginefuncs_t &meta_enginefuncs_t::operator=(const meta_enginefuncs_t &_rhs)
{
	Q_memcpy(this, &_rhs, sizeof(enginefuncs_t));
	return *this;
}

inline void meta_enginefuncs_t::set_from(enginefuncs_t *_pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(enginefuncs_t));
}

inline void meta_enginefuncs_t::copy_to(enginefuncs_t *_pFuncs)
{
	Q_memcpy(_pFuncs, this, sizeof(enginefuncs_t));
}

// HL_enginefuncs_t
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
struct HL_enginefuncs_t: public meta_enginefuncs_t {
public:
// functions:
	HL_enginefuncs_t();

	// Fill this object with pointers copied from an enginefuncs_t struct
	// and fixup the interface.
	// For this class this happens in the GiveFptrsToDll() function
	// with the pointers passed from the HL engine.
	void initialise_interface(enginefuncs_t *pFuncs);

private:
// functions:
public:
	// Moving copy_to() and set_from() to the private space.
	void set_from(enginefuncs_t *pFuncs) { meta_enginefuncs_t::set_from(pFuncs); };
	void copy_to(enginefuncs_t *pFuncs) { meta_enginefuncs_t::copy_to(pFuncs); };
};

inline HL_enginefuncs_t::HL_enginefuncs_t() : meta_enginefuncs_t() {};
