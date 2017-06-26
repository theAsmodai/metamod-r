#include "precompiled.h"

#define CDATA_DLL_H(x, p, h)		CDATA_ENTRY(DLL_FUNCTIONS, x, p, size_t(h))
#define CDATA_DLL(x)				CDATA_ENTRY(DLL_FUNCTIONS, x, P_PRE, 0u)
#define CDATA_NEWDLL_H(x, p, h)		CDATA_ENTRY(NEW_DLL_FUNCTIONS, x, p, uint8(h))
#define CDATA_NEWDLL(x)				CDATA_ENTRY(NEW_DLL_FUNCTIONS, x, P_PRE, 0u)

DLL_FUNCTIONS sFunctionTable;
DLL_FUNCTIONS sFunctionTable_jit;
DLL_FUNCTIONS *pHookedDllFunctions = &sFunctionTable;
NEW_DLL_FUNCTIONS sNewFunctionTable;
NEW_DLL_FUNCTIONS sNewFunctionTable_jit;
NEW_DLL_FUNCTIONS *pHookedNewDllFunctions = &sNewFunctionTable;

void MM_PRE_HOOK EXT_FUNC mm_ClientConnect(edict_t *pEntity, const char *, const char *, char [128])
{
	g_players.clear_player_cvar_query(pEntity);
}

void MM_PRE_HOOK EXT_FUNC mm_ClientDisconnect(edict_t *pEntity)
{
	g_players.clear_player_cvar_query(pEntity);
}

// this forward can be disabled from metamod.cpp
void MM_PRE_HOOK mm_ClientCommand(edict_t *pEntity)
{
	if (!strcmp(CMD_ARGV(0), "meta")) {
		client_meta(pEntity);
	}
}

void EXT_FUNC mm_ServerDeactivate()
{
	sFunctionTable_jit.pfnServerDeactivate();

	// Update loaded plugins.  Look for new plugins in inifile, as well as
	// any plugins waiting for a changelevel to load.
	//
	// This is done in ServerDeactivate rather than Activate, as the latter
	// isn't actually the first routine to be called on a new map.  In
	// particular DispatchKeyValue and DispatchSpawn are called before
	// Activate, and we want any newly loaded plugins to be able to catch
	// these.
	//
	// So, we do this from Deactivate, which is the _last_ routine called
	// from the previous map.  It's also called right before shutdown,
	// which means whenever hlds quits, it'll reload the plugins just
	// before it exits, which is rather silly, but oh well.
	g_plugins->refresh(PT_CHANGELEVEL); // <- callbacks rebuilded (!)
	g_plugins->unpause_all();
	// g_plugins->retry_all(PT_CHANGELEVEL);
	g_players.clear_all_cvar_queries();
	g_requestid_counter = 0;

	/* RETURN TO ENGINE */
}

compile_data_t g_dllfunc_cdata[] =
{
	CDATA_DLL(pfnGameInit),				// pfnGameInit()			Initialize the game (one-time call after loading of game .dll)
	CDATA_DLL(pfnSpawn),			// pfnSpawn()
	CDATA_DLL(pfnThink),			// pfnThink()
	CDATA_DLL(pfnUse),				// pfnUse()
	CDATA_DLL(pfnTouch),			// pfnTouch()
	CDATA_DLL(pfnBlocked),			// pfnBlocked()
	CDATA_DLL(pfnKeyValue),			// pfnKeyValue()
	CDATA_DLL(pfnSave),			// pfnSave()
	CDATA_DLL(pfnRestore),			// pfnRestore()
	CDATA_DLL(pfnSetAbsBox),		// pfnSetAbsBox()

	CDATA_DLL(pfnSaveWriteFields),			// pfnSaveWriteFields()
	CDATA_DLL(pfnSaveReadFields),			// pfnSaveReadFields()

	CDATA_DLL(pfnSaveGlobalState),			// pfnSaveGlobalState()
	CDATA_DLL(pfnRestoreGlobalState),			// pfnRestoreGlobalState()
	CDATA_DLL(pfnResetGlobalState),			// pfnResetGlobalState()

	CDATA_DLL_H(pfnClientConnect, P_PRE, mm_ClientConnect),			// pfnClientConnect()			(wd) Client has connected
	CDATA_DLL_H(pfnClientDisconnect, P_PRE, mm_ClientDisconnect),			// pfnClientDisconnect()		(wd) Player has left the game
	CDATA_DLL(pfnClientKill),				// pfnClientKill()			(wd) Player has typed "kill"
	CDATA_DLL(pfnClientPutInServer),			// pfnClientPutInServer()		(wd) Client is entering the game
	CDATA_DLL_H(pfnClientCommand, P_PRE, mm_ClientCommand),
	CDATA_DLL(pfnClientUserInfoChanged),		// pfnClientUserInfoChanged()		(wd) Client has updated their setinfo structure
	CDATA_DLL(pfnServerActivate),			// pfnServerActivate()			(wd) Server is starting a new map
	CDATA_DLL(pfnServerDeactivate),			// pfnServerDeactivate()		(wd) Server is leaving the map (shutdown), or changelevel); SDK2

	CDATA_DLL(pfnPlayerPreThink),			// pfnPlayerPreThink()
	CDATA_DLL(pfnPlayerPostThink),			// pfnPlayerPostThink()

	CDATA_DLL(pfnStartFrame),				// pfnStartFrame()
	CDATA_DLL(pfnParmsNewLevel),			// pfnParmsNewLevel()
	CDATA_DLL(pfnParmsChangeLevel),			// pfnParmsChangeLevel()

	CDATA_DLL(pfnGetGameDescription),			// pfnGetGameDescription()		Returns string describing current .dll.  E.g. "TeamFotrress 2"), "Half-Life"
	CDATA_DLL(pfnPlayerCustomization),			// pfnPlayerCustomization()		Notifies .dll of new customization for player.

	CDATA_DLL(pfnSpectatorConnect),			// pfnSpectatorConnect()		Called when spectator joins server
	CDATA_DLL(pfnSpectatorDisconnect),			// pfnSpectatorDisconnect()		Called when spectator leaves the server
	CDATA_DLL(pfnSpectatorThink),			// pfnSpectatorThink()			Called when spectator sends a command packet (usercmd_t)

	CDATA_DLL(pfnSys_Error),				// pfnSys_Error()			Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.  SDK2

	CDATA_DLL(pfnPM_Move),				// pfnPM_Move()				(wd) SDK2
	CDATA_DLL(pfnPM_Init),				// pfnPM_Init()				Server version of player movement initialization; (wd) SDK2
	CDATA_DLL(pfnPM_FindTextureType),			// pfnPM_FindTextureType()		(wd) SDK2

	CDATA_DLL(pfnSetupVisibility),			// pfnSetupVisibility()			Set up PVS and PAS for networking for this client; (wd) SDK2
	CDATA_DLL(pfnUpdateClientData),			// pfnUpdateClientData()		Set up data sent only to specific client; (wd) SDK2
	CDATA_DLL(pfnAddToFullPack),			// pfnAddToFullPack()			(wd) SDK2
	CDATA_DLL(pfnCreateBaseline),			// pfnCreateBaseline()			Tweak entity baseline for network encoding), allows setup of player baselines), too.; (wd) SDK2
	CDATA_DLL(pfnRegisterEncoders),			// pfnRegisterEncoders()		Callbacks for network encoding; (wd) SDK2
	CDATA_DLL(pfnGetWeaponData),			// pfnGetWeaponData()			(wd) SDK2
	CDATA_DLL(pfnCmdStart),				// pfnCmdStart()			(wd) SDK2
	CDATA_DLL(pfnCmdEnd),				// pfnCmdEnd()				(wd) SDK2
	CDATA_DLL(pfnConnectionlessPacket),		// pfnConnectionlessPacket()		(wd) SDK2
	CDATA_DLL(pfnGetHullBounds),			// pfnGetHullBounds()			(wd) SDK2
	CDATA_DLL(pfnCreateInstancedBaselines),		// pfnCreateInstancedBaselines()	(wd) SDK2
	CDATA_DLL(pfnInconsistentFile),			// pfnInconsistentFile()		(wd) SDK2
	CDATA_DLL(pfnAllowLagCompensation),		// pfnAllowLagCompensation()		(wd) SDK2
};

compile_data_t g_newdllfunc_cdata[] =
{
	CDATA_NEWDLL(pfnOnFreeEntPrivateData),	// pfnOnFreeEntPrivateData() Called right before the object's memory is freed. Calls its destructor.
	CDATA_NEWDLL(pfnGameShutdown),		// pfnGameShutdown()
	CDATA_NEWDLL(pfnShouldCollide),		// pfnShouldCollide()

	CDATA_NEWDLL(pfnCvarValue),			// pfnCvarValue()  (fz) Use mm_CvarValue2 instead
	CDATA_NEWDLL(pfnCvarValue2)			// pfnCvarValue2() (fz) When pfnQueryClientCvarValue2() completes it will call
	// pfnCvarValue2() with the request ID supplied earlier, the name of the cvar requested and the value of that cvar.
};

// It's not clear what the difference is between GetAPI and GetAPI2; they
// both appear to return the exact same function table.
//
// Only one of them appears to be ever called, though.  If the DLL provides
// GetAPI2, the engine/hlds will call that, and will not call GetAPI.  If
// the engine couldn't find GetAPI2 in the DLL, it appears to fall back to
// GetAPI.
//
// So, GetAPI2 appears to replace GetAPI, and appears to have been added
// with SDK 2.0.  My best guess is that, with the new SDK, interface
// version checking became important, and without the int ptr used in
// GetAPI2, the engine can't find out the version of the DLL via GetAPI.
//
// It's unclear whether a DLL coded under SDK2 needs to provide the older
// GetAPI or not..
C_DLLEXPORT int GetEntityAPI(DLL_FUNCTIONS *pFunctionTable, int interfaceVersion)
{
	META_DEBUG(3, "called: GetEntityAPI; version=%d", interfaceVersion);
	if (!pFunctionTable) {
		META_ERROR("GetEntityAPI called with null pFunctionTable");
		return FALSE;
	}
	if (interfaceVersion != INTERFACE_VERSION) {
		META_ERROR("GetEntityAPI version mismatch; requested=%d ours=%d", interfaceVersion, INTERFACE_VERSION);
		return FALSE;
	}

	Q_memcpy(pFunctionTable, &sFunctionTable, sizeof(DLL_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	META_DEBUG(3, "called: GetEntityAPI2; version=%d", *interfaceVersion);

	if (!pFunctionTable) {
		META_ERROR("GetEntityAPI2 called with null pFunctionTable");
		return FALSE;
	}
	if (*interfaceVersion != INTERFACE_VERSION) {
		META_ERROR("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return FALSE;
	}

	memcpy(pFunctionTable, &sFunctionTable, sizeof(DLL_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
	META_DEBUG(6, "called: GetNewDLLFunctions; version=%d", *interfaceVersion);
#if 0 // ~dvander - but then you can't use cvar querying on many mods...
	// Don't provide these functions to engine if gamedll doesn't provide
	// them. Otherwise, we're in the position of having to provide answers
	// we can't necessarily provide (for instance, ShouldCollide())...
	if (!GameDLL.funcs.newapi_table)
		return FALSE;
#endif

	if (!pNewFunctionTable) {
		META_ERROR("GetNewDLLFunctions called with null pNewFunctionTable");
		return FALSE;
	}
	if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		META_ERROR("GetNewDLLFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return FALSE;
	}
	memcpy(pNewFunctionTable, &sNewFunctionTable, sizeof(NEW_DLL_FUNCTIONS));
	return TRUE;
}

void compile_dllfunc_callbacks()
{
	jitdata_t jitdata;
	jitdata.plugins = g_plugins ? g_plugins->getPlugins() : nullptr;
	jitdata.table_offset = offsetof(MPlugin, m_dllapi_table);
	jitdata.post_table_offset = offsetof(MPlugin, m_dllapi_post_table);

	for (auto& cd : g_dllfunc_cdata) {
		jitdata.pfn_original = *(size_t *)(size_t(g_GameDLL.funcs.dllapi_table) + cd.offset);
		jitdata.args_count = cd.args_count;
		jitdata.has_ret = cd.has_ret;
		jitdata.has_varargs = cd.has_varargs;
		jitdata.pfn_offset = cd.offset;
		jitdata.mm_hook_time = cd.mm_hook_time;
		jitdata.mm_hook = cd.mm_hook;

#ifdef JIT_DEBUG
		jitdata.name = cd.name;
#endif

		*(size_t *)(size_t(&sFunctionTable_jit) + cd.offset) = g_jit.compile_callback(&jitdata);
	}
}

void compile_newdllfunc_callbacks()
{
	jitdata_t jitdata;
	jitdata.plugins = g_plugins ? g_plugins->getPlugins() : nullptr;
	jitdata.table_offset = offsetof(MPlugin, m_newapi_table);
	jitdata.post_table_offset = offsetof(MPlugin, m_newapi_post_table);

	for (auto& cd : g_newdllfunc_cdata) {
		jitdata.pfn_original = *(size_t *)(size_t(g_GameDLL.funcs.newapi_table) + cd.offset);
		jitdata.args_count = cd.args_count;
		jitdata.has_ret = cd.has_ret;
		jitdata.has_varargs = cd.has_varargs;
		jitdata.pfn_offset = cd.offset;
		jitdata.mm_hook_time = cd.mm_hook_time;
		jitdata.mm_hook = cd.mm_hook;

#ifdef JIT_DEBUG
		jitdata.name = cd.name;
#endif

		*(size_t *)(size_t(&sNewFunctionTable_jit) + cd.offset) = g_jit.compile_callback(&jitdata);
	}
}

void compile_gamedll_tramps()
{
	// we compile simple static functions that will call dynamic callbacks
	for (auto& cd : g_dllfunc_cdata) {
		*(size_t *)(size_t(&sFunctionTable) + cd.offset) = g_jit.compile_tramp(size_t(&sFunctionTable_jit) + cd.offset);
	}

	// use direct hook to prevent crash after callbacks rebuilding
	sFunctionTable.pfnServerDeactivate = mm_ServerDeactivate;

	for (auto& cd : g_newdllfunc_cdata) {
		*(size_t *)(size_t(&sNewFunctionTable) + cd.offset) = g_jit.compile_tramp(size_t(&sNewFunctionTable_jit) + cd.offset);
	}
}

void compile_gamedll_callbacks()
{
	static bool initialized = false;

	if (!initialized) {
		compile_gamedll_tramps();
		initialized = true;
	}

	compile_dllfunc_callbacks();
	compile_newdllfunc_callbacks();
}

void disable_clientcommand_fwd()
{
	g_dllfunc_cdata[offsetof(DLL_FUNCTIONS, pfnClientCommand) / sizeof(int)].mm_hook = 0;
}
