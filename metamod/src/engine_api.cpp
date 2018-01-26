#include "precompiled.h"

#define CDATA_ENG_H(x, p, h)		CDATA_ENTRY(enginefuncs_t, x, p, size_t(h))
#define CDATA_ENG(x)				CDATA_ENTRY(enginefuncs_t, x, P_PRE, 0u)

enginefuncs_t g_meta_engfuncs; // static trampolines to dynamic callbacks (for gamedll)
enginefuncs_t g_meta_engfuncs_jit; // dynamic jit callbacks

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
	CDATA_ENG(pfnPrecacheModel),
	CDATA_ENG(pfnPrecacheSound),
	CDATA_ENG(pfnSetModel),
	CDATA_ENG(pfnModelIndex),
	CDATA_ENG(pfnModelFrames),
	CDATA_ENG(pfnSetSize),
	CDATA_ENG(pfnChangeLevel),
	CDATA_ENG(pfnGetSpawnParms),
	CDATA_ENG(pfnSaveSpawnParms),
	CDATA_ENG(pfnVecToYaw),
	CDATA_ENG(pfnVecToAngles),
	CDATA_ENG(pfnMoveToOrigin),
	CDATA_ENG(pfnChangeYaw),
	CDATA_ENG(pfnChangePitch),
	CDATA_ENG(pfnFindEntityByString),
	CDATA_ENG(pfnGetEntityIllum),
	CDATA_ENG(pfnFindEntityInSphere),
	CDATA_ENG(pfnFindClientInPVS),
	CDATA_ENG(pfnEntitiesInPVS),
	CDATA_ENG(pfnMakeVectors),
	CDATA_ENG(pfnAngleVectors),
	CDATA_ENG(pfnCreateEntity),
	CDATA_ENG(pfnRemoveEntity),
	CDATA_ENG(pfnCreateNamedEntity),
	CDATA_ENG(pfnMakeStatic),
	CDATA_ENG(pfnEntIsOnFloor),
	CDATA_ENG(pfnDropToFloor),
	CDATA_ENG(pfnWalkMove),
	CDATA_ENG(pfnSetOrigin),
	CDATA_ENG(pfnEmitSound),
	CDATA_ENG(pfnEmitAmbientSound),
	CDATA_ENG(pfnTraceLine),
	CDATA_ENG(pfnTraceToss),
	CDATA_ENG(pfnTraceMonsterHull),
	CDATA_ENG(pfnTraceHull),
	CDATA_ENG(pfnTraceModel),
	CDATA_ENG(pfnTraceTexture),
	CDATA_ENG(pfnTraceSphere),
	CDATA_ENG(pfnGetAimVector),
	CDATA_ENG(pfnServerCommand),
	CDATA_ENG(pfnServerExecute),
	CDATA_ENG(pfnClientCommand),		// D'oh, ClientCommand in dllapi too.
	CDATA_ENG(pfnParticleEffect),
	CDATA_ENG(pfnLightStyle),
	CDATA_ENG(pfnDecalIndex),
	CDATA_ENG(pfnPointContents),
	CDATA_ENG(pfnMessageBegin),
	CDATA_ENG(pfnMessageEnd),
	CDATA_ENG(pfnWriteByte),
	CDATA_ENG(pfnWriteChar),
	CDATA_ENG(pfnWriteShort),
	CDATA_ENG(pfnWriteLong),
	CDATA_ENG(pfnWriteAngle),
	CDATA_ENG(pfnWriteCoord),
	CDATA_ENG(pfnWriteString),
	CDATA_ENG(pfnWriteEntity),
	CDATA_ENG(pfnCVarRegister),
	CDATA_ENG(pfnCVarGetFloat),
	CDATA_ENG(pfnCVarGetString),
	CDATA_ENG(pfnCVarSetFloat),
	CDATA_ENG(pfnCVarSetString),
	CDATA_ENG(pfnAlertMessage),
	CDATA_ENG(pfnEngineFprintf),
	CDATA_ENG(pfnPvAllocEntPrivateData),
	CDATA_ENG(pfnPvEntPrivateData),
	CDATA_ENG(pfnFreeEntPrivateData),
	CDATA_ENG(pfnSzFromIndex),
	CDATA_ENG(pfnAllocString),
	CDATA_ENG(pfnGetVarsOfEnt),
	CDATA_ENG(pfnPEntityOfEntOffset),
	CDATA_ENG(pfnEntOffsetOfPEntity),
	CDATA_ENG(pfnIndexOfEdict),
	CDATA_ENG(pfnPEntityOfEntIndex),
	CDATA_ENG(pfnFindEntityByVars),
	CDATA_ENG(pfnGetModelPtr),
	CDATA_ENG_H(pfnRegUserMsg, P_POST, mm_RegUserMsg),
	CDATA_ENG(pfnAnimationAutomove),
	CDATA_ENG(pfnGetBonePosition),
	CDATA_ENG(pfnFunctionFromName),
	CDATA_ENG(pfnNameForFunction),
	CDATA_ENG(pfnClientPrintf),						// JOHN: engine callbacks so game DLL can print messages to individual clients
	CDATA_ENG(pfnServerPrint),
	CDATA_ENG(pfnCmd_Args),							// these 3 added
	CDATA_ENG(pfnCmd_Argv),							// so game DLL can easily
	CDATA_ENG(pfnCmd_Argc),							// access client 'cmd' strings
	CDATA_ENG(pfnGetAttachment),
	CDATA_ENG(pfnCRC32_Init),
	CDATA_ENG(pfnCRC32_ProcessBuffer),
	CDATA_ENG(pfnCRC32_ProcessByte),
	CDATA_ENG(pfnCRC32_Final),
	CDATA_ENG(pfnRandomLong),
	CDATA_ENG(pfnRandomFloat),
	CDATA_ENG(pfnSetView),
	CDATA_ENG(pfnTime),
	CDATA_ENG(pfnCrosshairAngle),
	CDATA_ENG(pfnLoadFileForMe),
	CDATA_ENG(pfnFreeFile),
	CDATA_ENG(pfnEndSection),
	CDATA_ENG(pfnCompareFileTime),
	CDATA_ENG(pfnGetGameDir),
	CDATA_ENG(pfnCvar_RegisterVariable),
	CDATA_ENG(pfnFadeClientVolume),
	CDATA_ENG(pfnSetClientMaxspeed),
	CDATA_ENG(pfnCreateFakeClient),					// returns NULL if fake client can't be created
	CDATA_ENG(pfnRunPlayerMove),
	CDATA_ENG(pfnNumberOfEntities),
	CDATA_ENG(pfnGetInfoKeyBuffer),					// passing in NULL gets the serverinfo
	CDATA_ENG(pfnInfoKeyValue),
	CDATA_ENG(pfnSetKeyValue),
	CDATA_ENG(pfnSetClientKeyValue),
	CDATA_ENG(pfnIsMapValid),
	CDATA_ENG(pfnStaticDecal),
	CDATA_ENG(pfnPrecacheGeneric),
	CDATA_ENG(pfnGetPlayerUserId),					// returns the server assigned userid for this player.
	CDATA_ENG(pfnBuildSoundMsg),
	CDATA_ENG(pfnIsDedicatedServer),				// is this a dedicated server?
	CDATA_ENG(pfnCVarGetPointer),
	CDATA_ENG(pfnGetPlayerWONId),					// returns the server assigned WONid for this player.
	CDATA_ENG(pfnInfo_RemoveKey),
	CDATA_ENG(pfnGetPhysicsKeyValue),
	CDATA_ENG(pfnSetPhysicsKeyValue),
	CDATA_ENG(pfnGetPhysicsInfoString),
	CDATA_ENG(pfnPrecacheEvent),
	CDATA_ENG(pfnPlaybackEvent),
	CDATA_ENG(pfnSetFatPVS),
	CDATA_ENG(pfnSetFatPAS),
	CDATA_ENG(pfnCheckVisibility),
	CDATA_ENG(pfnDeltaSetField),
	CDATA_ENG(pfnDeltaUnsetField),
	CDATA_ENG(pfnDeltaAddEncoder),
	CDATA_ENG(pfnGetCurrentPlayer),
	CDATA_ENG(pfnCanSkipPlayer),
	CDATA_ENG(pfnDeltaFindField),
	CDATA_ENG(pfnDeltaSetFieldByIndex),
	CDATA_ENG(pfnDeltaUnsetFieldByIndex),
	CDATA_ENG(pfnSetGroupMask),
	CDATA_ENG(pfnCreateInstancedBaseline),			// D'oh, CreateInstancedBaseline in dllapi too.
	CDATA_ENG(pfnCvar_DirectSet),
	CDATA_ENG(pfnForceUnmodified),
	CDATA_ENG(pfnGetPlayerStats),
	CDATA_ENG(pfnAddServerCommand),
	CDATA_ENG(pfnVoice_GetClientListening),
	CDATA_ENG(pfnVoice_SetClientListening),
	CDATA_ENG(pfnGetPlayerAuthId),
	CDATA_ENG(pfnSequenceGet),
	CDATA_ENG(pfnSequencePickSentence),
	CDATA_ENG(pfnGetFileSize),
	CDATA_ENG(pfnGetApproxWavePlayLen),
	CDATA_ENG(pfnIsCareerMatch),
	CDATA_ENG(pfnGetLocalizedStringLength),
	CDATA_ENG(pfnRegisterTutorMessageShown),
	CDATA_ENG(pfnGetTimesTutorMessageShown),
	CDATA_ENG(pfnProcessTutorMessageDecayBuffer),
	CDATA_ENG(pfnConstructTutorMessageDecayBuffer),
	CDATA_ENG(pfnResetTutorMessageDecayData),
	CDATA_ENG_H(pfnQueryClientCvarValue, P_PRE, mm_QueryClientCvarValue),
	CDATA_ENG(pfnQueryClientCvarValue2),
	CDATA_ENG(pfnEngCheckParm)
};

void compile_engfuncs_callbacks()
{
	jitdata_t jitdata;
	jitdata.plugins = g_plugins ? g_plugins->getPlugins() : nullptr;
	jitdata.table_offset = offsetof(MPlugin, m_engine_table);
	jitdata.post_table_offset = offsetof(MPlugin, m_engine_post_table);

	for (auto& cd : g_engfuncs_cdata) {
		jitdata.pfn_original = *(size_t *)(size_t(&g_engfuncs) + cd.offset);
		jitdata.args_count = cd.args_count;
		jitdata.rettype = cd.rettype;
		jitdata.has_varargs = cd.has_varargs;
		jitdata.pfn_offset = cd.offset;
		jitdata.mm_hook_time = cd.mm_hook_time;
		jitdata.mm_hook = cd.mm_hook;

#ifdef JIT_DEBUG
		jitdata.name = cd.name;
#endif

		*(size_t *)(size_t(&g_meta_engfuncs_jit) + cd.offset) = g_jit.compile_callback(&jitdata);
	}
}

void compile_engine_tramps()
{
	// we compile simple static functions that will call dynamic callbacks
	for (auto& cd : g_engfuncs_cdata) {
		*(size_t *)(size_t(&g_meta_engfuncs) + cd.offset) = g_jit.compile_tramp(size_t(&g_meta_engfuncs_jit) + cd.offset/*, cd.mm_hook, cd.mm_hook_time*/);
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
