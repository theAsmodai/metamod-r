#include "precompiled.h"

#define CDATA_ENG_H(x, p, h)		CDATA_ENTRY(enginefuncs_t, x, p, size_t(h))
#define CDATA_ENG(x)				CDATA_ENTRY(enginefuncs_t, x, P_PRE, 0u)

meta_enginefuncs_t meta_engfuncs; // static trampolines to dynamic callbacks (for gamedll)
meta_enginefuncs_t meta_engfuncs_jit; // dynamic jit callbacks

void MM_PRE_HOOK mm_QueryClientCvarValue(const edict_t* pEdict, const char* cvarName)
{
	g_players.set_player_cvar_query(pEdict, cvarName);
}

// int -> void
void MM_POST_HOOK mm_RegUserMsg(const char* pszName, int iSize)
{
	// Add the msgid, name, and size to our saved list, if we haven't already.
	auto imsgid = *(int *)(g_metaGlobals.status == MRES_OVERRIDE ? g_metaGlobals.override_ret : g_metaGlobals.orig_ret);
	auto nmsg = g_regMsgs->find(imsgid);
	
	if (nmsg) {
		if (!Q_strcmp(pszName, nmsg->getname()))
			// This name/msgid pair was already registered.
			META_DEBUG(3, "user message registered again: name=%s, msgid=%d", pszName, imsgid);
		else
			// This msgid was previously used by a different message name.
			META_ERROR("user message id reused: msgid=%d, oldname=%s, newname=%s", imsgid, nmsg->getname(), pszName);
	}
	else
		g_regMsgs->add(pszName, imsgid, iSize);
}

compile_data_t g_engfuncs_cdata[] =
{
	CDATA_ENG(pfnPrecacheModel),			// pfnPrecacheModel()
	CDATA_ENG(pfnPrecacheSound),			// pfnPrecacheSound()
	CDATA_ENG(pfnSetModel),				// pfnSetModel()
	CDATA_ENG(pfnModelIndex),				// pfnModelIndex()
	CDATA_ENG(pfnModelFrames),			// pfnModelFrames()

	CDATA_ENG(pfnSetSize),				// pfnSetSize()
	CDATA_ENG(pfnChangeLevel),			// pfnChangeLevel()
	CDATA_ENG(pfnGetSpawnParms),			// pfnGetSpawnParms()
	CDATA_ENG(pfnSaveSpawnParms),			// pfnSaveSpawnParms()

	CDATA_ENG(pfnVecToYaw),				// pfnVecToYaw()
	CDATA_ENG(pfnVecToAngles),			// pfnVecToAngles()
	CDATA_ENG(pfnMoveToOrigin),			// pfnMoveToOrigin()
	CDATA_ENG(pfnChangeYaw),				// pfnChangeYaw()
	CDATA_ENG(pfnChangePitch),			// pfnChangePitch()

	CDATA_ENG(pfnFindEntityByString),			// pfnFindEntityByString()
	CDATA_ENG(pfnGetEntityIllum),			// pfnGetEntityIllum()
	CDATA_ENG(pfnFindEntityInSphere),			// pfnFindEntityInSphere()
	CDATA_ENG(pfnFindClientInPVS),			// pfnFindClientInPVS()
	CDATA_ENG(pfnEntitiesInPVS),			// pfnEntitiesInPVS()

	CDATA_ENG(pfnMakeVectors),			// pfnMakeVectors()
	CDATA_ENG(pfnAngleVectors),			// pfnAngleVectors()

	CDATA_ENG(pfnCreateEntity),			// pfnCreateEntity()
	CDATA_ENG(pfnRemoveEntity),			// pfnRemoveEntity()
	CDATA_ENG(pfnCreateNamedEntity),			// pfnCreateNamedEntity()

	CDATA_ENG(pfnMakeStatic),				// pfnMakeStatic()
	CDATA_ENG(pfnEntIsOnFloor),			// pfnEntIsOnFloor()
	CDATA_ENG(pfnDropToFloor),			// pfnDropToFloor()

	CDATA_ENG(pfnWalkMove),				// pfnWalkMove()
	CDATA_ENG(pfnSetOrigin),				// pfnSetOrigin()

	CDATA_ENG(pfnEmitSound),				// pfnEmitSound()
	CDATA_ENG(pfnEmitAmbientSound),			// pfnEmitAmbientSound()

	CDATA_ENG(pfnTraceLine),				// pfnTraceLine()
	CDATA_ENG(pfnTraceToss),				// pfnTraceToss()
	CDATA_ENG(pfnTraceMonsterHull),			// pfnTraceMonsterHull()
	CDATA_ENG(pfnTraceHull),				// pfnTraceHull()
	CDATA_ENG(pfnTraceModel),				// pfnTraceModel()
	CDATA_ENG(pfnTraceTexture),			// pfnTraceTexture()
	CDATA_ENG(pfnTraceSphere),			// pfnTraceSphere()
	CDATA_ENG(pfnGetAimVector),			// pfnGetAimVector()

	CDATA_ENG(pfnServerCommand),			// pfnServerCommand()
	CDATA_ENG(pfnServerExecute),			// pfnServerExecute()
	CDATA_ENG(pfnClientCommand),			// pfnClientCommand()	// D'oh, ClientCommand in dllapi too.

	CDATA_ENG(pfnParticleEffect),			// pfnParticleEffect()
	CDATA_ENG(pfnLightStyle),				// pfnLightStyle()
	CDATA_ENG(pfnDecalIndex),				// pfnDecalIndex()
	CDATA_ENG(pfnPointContents),			// pfnPointContents()

	CDATA_ENG(pfnMessageBegin),			// pfnMessageBegin()
	CDATA_ENG(pfnMessageEnd),				// pfnMessageEnd()

	CDATA_ENG(pfnWriteByte),				// pfnWriteByte()
	CDATA_ENG(pfnWriteChar),				// pfnWriteChar()
	CDATA_ENG(pfnWriteShort),				// pfnWriteShort()
	CDATA_ENG(pfnWriteLong),				// pfnWriteLong()
	CDATA_ENG(pfnWriteAngle),				// pfnWriteAngle()
	CDATA_ENG(pfnWriteCoord),				// pfnWriteCoord()
	CDATA_ENG(pfnWriteString),			// pfnWriteString()
	CDATA_ENG(pfnWriteEntity),			// pfnWriteEntity()

	CDATA_ENG(pfnCVarRegister),			// pfnCVarRegister()
	CDATA_ENG(pfnCVarGetFloat),			// pfnCVarGetFloat()
	CDATA_ENG(pfnCVarGetString),			// pfnCVarGetString()
	CDATA_ENG(pfnCVarSetFloat),			// pfnCVarSetFloat()
	CDATA_ENG(pfnCVarSetString),			// pfnCVarSetString()

	CDATA_ENG(pfnAlertMessage),			// pfnAlertMessage()
	CDATA_ENG(pfnEngineFprintf),			// pfnEngineFprintf()

	CDATA_ENG(pfnPvAllocEntPrivateData),		// pfnPvAllocEntPrivateData()
	CDATA_ENG(pfnPvEntPrivateData),			// pfnPvEntPrivateData()
	CDATA_ENG(pfnFreeEntPrivateData),			// pfnFreeEntPrivateData()

	CDATA_ENG(pfnSzFromIndex),			// pfnSzFromIndex()
	CDATA_ENG(pfnAllocString),			// pfnAllocString()

	CDATA_ENG(pfnGetVarsOfEnt),			// pfnGetVarsOfEnt()
	CDATA_ENG(pfnPEntityOfEntOffset),			// pfnPEntityOfEntOffset()
	CDATA_ENG(pfnEntOffsetOfPEntity),			// pfnEntOffsetOfPEntity()
	CDATA_ENG(pfnIndexOfEdict),			// pfnIndexOfEdict()
	CDATA_ENG(pfnPEntityOfEntIndex),			// pfnPEntityOfEntIndex()
	CDATA_ENG(pfnFindEntityByVars),			// pfnFindEntityByVars()
	CDATA_ENG(pfnGetModelPtr),			// pfnGetModelPtr()

	CDATA_ENG_H(pfnRegUserMsg, P_POST, mm_RegUserMsg),				// pfnRegUserMsg()

	CDATA_ENG(pfnAnimationAutomove),			// pfnAnimationAutomove()
	CDATA_ENG(pfnGetBonePosition),			// pfnGetBonePosition()

	CDATA_ENG(pfnFunctionFromName),			// pfnFunctionFromName()
	CDATA_ENG(pfnNameForFunction),			// pfnNameForFunction()

	CDATA_ENG(pfnClientPrintf),			// pfnClientPrintf()			// JOHN: engine callbacks so game DLL can print messages to individual clients
	CDATA_ENG(pfnServerPrint),			// pfnServerPrint()

	CDATA_ENG(pfnCmd_Args),				// pfnCmd_Args()			// these 3 added
	CDATA_ENG(pfnCmd_Argv),				// pfnCmd_Argv()			// so game DLL can easily
	CDATA_ENG(pfnCmd_Argc),				// pfnCmd_Argc()			// access client 'cmd' strings

	CDATA_ENG(pfnGetAttachment),			// pfnGetAttachment()

	CDATA_ENG(pfnCRC32_Init),				// pfnCRC32_Init()
	CDATA_ENG(pfnCRC32_ProcessBuffer),		// pfnCRC32_ProcessBuffer()
	CDATA_ENG(pfnCRC32_ProcessByte),			// pfnCRC32_ProcessByte()
	CDATA_ENG(pfnCRC32_Final),			// pfnCRC32_Final()

	CDATA_ENG(pfnRandomLong),				// pfnRandomLong()
	CDATA_ENG(pfnRandomFloat),			// pfnRandomFloat()

	CDATA_ENG(pfnSetView),				// pfnSetView()
	CDATA_ENG(pfnTime),					// pfnTime()
	CDATA_ENG(pfnCrosshairAngle),			// pfnCrosshairAngle()

	CDATA_ENG(pfnLoadFileForMe),			// pfnLoadFileForMe()
	CDATA_ENG(pfnFreeFile),				// pfnFreeFile()

	CDATA_ENG(pfnEndSection),				// pfnEndSection()			// trigger_endsection
	CDATA_ENG(pfnCompareFileTime),			// pfnCompareFileTime()
	CDATA_ENG(pfnGetGameDir),				// pfnGetGameDir()
	CDATA_ENG(pfnCvar_RegisterVariable),		// pfnCvar_RegisterVariable()
	CDATA_ENG(pfnFadeClientVolume),			// pfnFadeClientVolume()
	CDATA_ENG(pfnSetClientMaxspeed),			// pfnSetClientMaxspeed()
	CDATA_ENG(pfnCreateFakeClient),			// pfnCreateFakeClient() 		// returns NULL if fake client can't be created
	CDATA_ENG(pfnRunPlayerMove),			// pfnRunPlayerMove()
	CDATA_ENG(pfnNumberOfEntities),			// pfnNumberOfEntities()

	CDATA_ENG(pfnGetInfoKeyBuffer),			// pfnGetInfoKeyBuffer()		// passing in NULL gets the serverinfo
	CDATA_ENG(pfnInfoKeyValue),			// pfnInfoKeyValue()
	CDATA_ENG(pfnSetKeyValue),			// pfnSetKeyValue()
	CDATA_ENG(pfnSetClientKeyValue),			// pfnSetClientKeyValue()

	CDATA_ENG(pfnIsMapValid),				// pfnIsMapValid()
	CDATA_ENG(pfnStaticDecal),			// pfnStaticDecal()
	CDATA_ENG(pfnPrecacheGeneric),			// pfnPrecacheGeneric()
	CDATA_ENG(pfnGetPlayerUserId),			// pfnGetPlayerUserId()			// returns the server assigned userid for this player.
	CDATA_ENG(pfnBuildSoundMsg),			// pfnBuildSoundMsg()
	CDATA_ENG(pfnIsDedicatedServer),			// pfnIsDedicatedServer()		// is this a dedicated server?
	CDATA_ENG(pfnCVarGetPointer),			// pfnCVarGetPointer()
	CDATA_ENG(pfnGetPlayerWONId),			// pfnGetPlayerWONId()			// returns the server assigned WONid for this player.

	CDATA_ENG(pfnInfo_RemoveKey),			// pfnInfo_RemoveKey()
	CDATA_ENG(pfnGetPhysicsKeyValue),			// pfnGetPhysicsKeyValue()
	CDATA_ENG(pfnSetPhysicsKeyValue),			// pfnSetPhysicsKeyValue()
	CDATA_ENG(pfnGetPhysicsInfoString),		// pfnGetPhysicsInfoString()
	CDATA_ENG(pfnPrecacheEvent),			// pfnPrecacheEvent()
	CDATA_ENG(pfnPlaybackEvent),			// pfnPlaybackEvent()

	CDATA_ENG(pfnSetFatPVS),				// pfnSetFatPVS()
	CDATA_ENG(pfnSetFatPAS),				// pfnSetFatPAS()

	CDATA_ENG(pfnCheckVisibility),			// pfnCheckVisibility()

	CDATA_ENG(pfnDeltaSetField),			// pfnDeltaSetField()
	CDATA_ENG(pfnDeltaUnsetField),			// pfnDeltaUnsetField()
	CDATA_ENG(pfnDeltaAddEncoder),			// pfnDeltaAddEncoder()
	CDATA_ENG(pfnGetCurrentPlayer),			// pfnGetCurrentPlayer()
	CDATA_ENG(pfnCanSkipPlayer),			// pfnCanSkipPlayer()
	CDATA_ENG(pfnDeltaFindField),			// pfnDeltaFindField()
	CDATA_ENG(pfnDeltaSetFieldByIndex),		// pfnDeltaSetFieldByIndex()
	CDATA_ENG(pfnDeltaUnsetFieldByIndex),		// pfnDeltaUnsetFieldByIndex()

	CDATA_ENG(pfnSetGroupMask),			// pfnSetGroupMask()

	CDATA_ENG(pfnCreateInstancedBaseline),		// pfnCreateInstancedBaseline()		// D'oh, CreateInstancedBaseline in dllapi too.
	CDATA_ENG(pfnCvar_DirectSet),			// pfnCvar_DirectSet()

	CDATA_ENG(pfnForceUnmodified),			// pfnForceUnmodified()

	CDATA_ENG(pfnGetPlayerStats),			// pfnGetPlayerStats()

	CDATA_ENG(pfnAddServerCommand),			// pfnAddServerCommand()

	CDATA_ENG(pfnVoice_GetClientListening),		// pfnVoice_GetClientListening()
	CDATA_ENG(pfnVoice_SetClientListening),		// pfnVoice_SetClientListening()

	CDATA_ENG(pfnGetPlayerAuthId),			// pfnGetPlayerAuthId()

	CDATA_ENG(pfnSequenceGet),			// pfnSequenceGet()
	CDATA_ENG(pfnSequencePickSentence),		// pfnSequencePickSentence()
	CDATA_ENG(pfnGetFileSize),			// pfnGetFileSize()
	CDATA_ENG(pfnGetApproxWavePlayLen),		// pfnGetApproxWavePlayLen()
	CDATA_ENG(pfnIsCareerMatch),			// pfnIsCareerMatch()
	CDATA_ENG(pfnGetLocalizedStringLength),		// pfnGetLocalizedStringLength()
	CDATA_ENG(pfnRegisterTutorMessageShown),		// pfnRegisterTutorMessageShown()
	CDATA_ENG(pfnGetTimesTutorMessageShown),		// pfnGetTimesTutorMessageShown()
	CDATA_ENG(pfnProcessTutorMessageDecayBuffer),	// pfnProcessTutorMessageDecayBuffer()
	CDATA_ENG(pfnConstructTutorMessageDecayBuffer),	// pfnConstructTutorMessageDecayBuffer()
	CDATA_ENG(pfnResetTutorMessageDecayData),		// pfnResetTutorMessageDecayData()

	CDATA_ENG_H(pfnQueryClientCvarValue, P_PRE, mm_QueryClientCvarValue),		// pfnQueryClientCvarValue()
	CDATA_ENG(pfnQueryClientCvarValue2),		// pfnQueryClientCvarValue2()
	CDATA_ENG(pfnEngCheckParm)		// pfnCheckParm()*/
};

void compile_engfuncs_callbacks()
{
	jitdata_t jitdata;
	jitdata.plugins = g_plugins ? g_plugins->getlist() : nullptr;
	jitdata.plugins_count = g_plugins ? g_plugins->getmaxcount() : 0;
	jitdata.table_offset = offsetof(MPlugin, m_engine_table);
	jitdata.post_table_offset = offsetof(MPlugin, m_engine_post_table);

	for (auto& cd : g_engfuncs_cdata) {
		jitdata.pfn_original = *(size_t *)(size_t(&g_engfuncs) + cd.offset);
		jitdata.args_count = cd.args_count;
		jitdata.has_ret = cd.has_ret;
		jitdata.has_varargs = cd.has_varargs;
		jitdata.pfn_offset = cd.offset;
		jitdata.mm_hook_time = cd.mm_hook_time;
		jitdata.mm_hook = cd.mm_hook;

		*(size_t *)(size_t(&meta_engfuncs_jit) + cd.offset) = g_jit.compile_callback(&jitdata);
	}
}

void compile_engine_tramps()
{
	// we compile simple static functions that will call dynamic callbacks
	for (auto& cd : g_engfuncs_cdata) {
		*(size_t *)(size_t(&meta_engfuncs) + cd.offset) = g_jit.compile_tramp(size_t(&meta_engfuncs_jit) + cd.offset/*, cd.mm_hook, cd.mm_hook_time*/);
	}
}

void compile_engine_callbacks()
{
	static bool initialized = false;

	if (!initialized) {
		compile_engine_tramps();
		initialized = true;
	}

	compile_engfuncs_callbacks();
}
