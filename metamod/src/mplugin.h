#pragma once

#include "api_info.h"
#include "support_meta.h"

// Flags to indicate current "load" state of plugin.
// NOTE: order is important, as greater/less comparisons are made.
enum PLUG_STATUS : uint8
{
	PL_EMPTY = 0,		// empty slot
	PL_VALID,			// has valid info in it
	PL_BADFILE,			// nonexistent file (open failed), or not a valid plugin file (query failed)
	PL_OPENED,			// dlopened and queried
	PL_FAILED,			// opened, but failed to attach or unattach
	PL_RUNNING,			// attached and running
	PL_PAUSED,			// attached but paused
};

// Action to take for plugin at next opportunity.
enum PLUG_ACTION : uint8
{
	PA_NULL = 0,
	PA_NONE,			// no action needed right now
	PA_KEEP,			// keep, after ini refresh
	PA_LOAD,			// load (dlopen, query) and try to attach
	PA_ATTACH,			// attach
	PA_UNLOAD,			// unload (detach, dlclose)
	PA_RELOAD,			// unload and load again
};

// Flags to indicate from where the plugin was loaded.
enum PLOAD_SOURCE : uint8
{
	PS_INI = 0,			// was loaded from the plugins.ini
	PS_CMD,				// was loaded via a server command
	PS_PLUGIN,			// was loaded by other plugin
};

// Flags for how to word description of plugin loadtime.
enum STR_LOADTIME : uint8
{
	SL_SIMPLE = 0,			// single word
	SL_SHOW,			// for "show" output, 5 chars
	SL_ALLOWED,			// when plugin is allowed to load/unload
	SL_NOW,				// current situation
};

// Flags for how to format description of status.
enum STR_STATUS : uint8
{
	ST_SIMPLE = 0,			// single word
	ST_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of action.
enum STR_ACTION : uint8
{
	SA_SIMPLE = 0,			// single word
	SA_SHOW,			// for "show" output, 4 chars
};

// Flags for how to format description of source.
enum STR_SOURCE : uint8
{
	SO_SIMPLE = 0,			// two words
	SO_SHOW,			// for "list" output, 3 chars
};

// An individual plugin.
class MPlugin {
public:
	PLUG_STATUS status;				// current status of plugin (loaded, etc)
	PLUG_ACTION action;				// what to do with plugin (load, unload, etc)
	PLOAD_SOURCE source;				// source of the request to load the plugin
	int index;					// 1-based
	plugin_info_t *info;				// information plugin provides about itself
	CSysModule sys_module;
	time_t time_loaded;				// when plugin was loaded
	int source_plugin_index;			// who loaded this plugin
	int unloader_index;
	bool is_unloader;				// fix to prevent other plugins unload active unloader.

	DLL_FUNCTIONS *dllapi_table;
	DLL_FUNCTIONS *dllapi_post_table;
	NEW_DLL_FUNCTIONS *newapi_table;
	NEW_DLL_FUNCTIONS *newapi_post_table;
	enginefuncs_t *engine_table;
	enginefuncs_t *engine_post_table;

	gamedll_funcs_t gamedll_funcs;
	mutil_funcs_t mutil_funcs;

	char filename[PATH_MAX];			// ie "dlls/mm_test_i386.so", from inifile
	char *file;					// ie "mm_test_i386.so", ptr from filename
	char desc[MAX_DESC_LEN];			// ie "Test metamod plugin", from inifile
	char pathname[PATH_MAX];			// UNIQUE, ie "/home/willday/half-life/cstrike/dlls/mm_test_i386.so", built with GameDLL.gamedir
	int pfspecific;					// level of specific platform affinity, used during load time

	bool ini_parseline(char *line);				// parse line from inifile
	bool cmd_parseline(const char *line);				// parse from console command
	bool plugin_parseline(const char *fname, int loader_index); 	// parse from plugin
	bool check_input();

	bool resolve();						// find a matching file on disk
	char *resolve_dirs(char *path);
	char *resolve_prefix(char *path) const;
	char *resolve_suffix(char *path) const;

	bool platform_match(MPlugin* plugin);

	bool load(PLUG_LOADTIME now);
	bool unload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason, PL_UNLOAD_REASON real_reason);
	bool reload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
	bool pause();
	bool unpause();
	bool retry(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);			// if previously failed
	bool clear();
	bool plugin_unload(plid_t plid, PLUG_LOADTIME now, PL_UNLOAD_REASON reason);	// other plugin unloading
	void show();									// print info about plugin to console

	bool newer_file();								// check for newer file on disk

	const char *str_status(STR_STATUS fmt);
	const char *str_action(STR_ACTION fmt);
	const char *str_source(STR_SOURCE fmt);

	const char *str_reason(PL_UNLOAD_REASON preason, PL_UNLOAD_REASON preal_reason);
	const char *str_loadtime(PLUG_LOADTIME pallow, STR_LOADTIME fmt);

	const char *str_status()		{ return str_status(ST_SIMPLE); };
	const char *str_action()		{ return str_action(SA_SIMPLE); };
	const char *str_source()		{ return str_source(SO_SIMPLE); };

	const char *str_loadable() {
		if (info) return str_loadtime(info->loadable, SL_SIMPLE);
		else return " -";
	};
	const char *str_unloadable() {
		if (info) return str_loadtime(info->unloadable, SL_SIMPLE);
		else return " -";
	};
	const char *str_loadable(STR_LOADTIME fmt) {
		if (info) return str_loadtime(info->loadable, fmt);
		else return " -";
	};
	const char *str_unloadable(STR_LOADTIME fmt) {
		if (info) return str_loadtime(info->unloadable, fmt);
		else return " -";
	};

private:
	bool query();
	bool attach(PLUG_LOADTIME now);
	bool detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
};

// Macros used by MPlugin::show(), to list the functions that the plugin
// catches.
#define SHOW_IFDEF(api_table, info_table, pfnName, pre_str, post_str) \
	if (api_table->pfnName) { META_CONS("%s%s%s", pre_str, info_table.pfnName.name, post_str); n++;}

#define SHOW_DEF_DLLAPI(api_table, pre_str, post_str) \
	n = 0; \
	SHOW_IFDEF(api_table, dllapi_info, pfnGameInit, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSpawn, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnThink, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnUse, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnTouch, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnBlocked, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnKeyValue, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSave, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnRestore, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSetAbsBox, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSaveWriteFields, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSaveReadFields, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSaveGlobalState, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnRestoreGlobalState, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnResetGlobalState, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientConnect, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientDisconnect, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientKill, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientPutInServer, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientCommand, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnClientUserInfoChanged,	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnServerActivate, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnServerDeactivate, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPlayerPreThink, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPlayerPostThink, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnStartFrame, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnParmsNewLevel, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnParmsChangeLevel, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnGetGameDescription, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPlayerCustomization, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSpectatorConnect, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSpectatorDisconnect, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSpectatorThink, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSys_Error, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPM_Move, 				pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPM_Init, 				pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnPM_FindTextureType, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnSetupVisibility, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnUpdateClientData, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnAddToFullPack, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnCreateBaseline, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnRegisterEncoders, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnGetWeaponData, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnCmdStart, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnCmdEnd, 				pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnConnectionlessPacket, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnGetHullBounds, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnCreateInstancedBaselines, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnInconsistentFile, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, dllapi_info, pfnAllowLagCompensation, 		pre_str, post_str);

#define SHOW_DEF_NEWAPI(api_table, pre_str, post_str) \
	n = 0; \
	SHOW_IFDEF(api_table, newapi_info, pfnOnFreeEntPrivateData,	pre_str, post_str); \
	SHOW_IFDEF(api_table, newapi_info, pfnGameShutdown,		pre_str, post_str); \
	SHOW_IFDEF(api_table, newapi_info, pfnShouldCollide,	pre_str, post_str); \
	SHOW_IFDEF(api_table, newapi_info, pfnCvarValue,		pre_str, post_str);

#define SHOW_DEF_ENGINE(api_table, pre_str, post_str) \
	n = 0; \
	SHOW_IFDEF(api_table, engine_info, pfnPrecacheModel, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPrecacheSound, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetModel, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnModelIndex, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnModelFrames, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetSize, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnChangeLevel, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetSpawnParms, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSaveSpawnParms, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnVecToYaw, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnVecToAngles, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnMoveToOrigin, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnChangeYaw, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnChangePitch, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFindEntityByString, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetEntityIllum, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFindEntityInSphere, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFindClientInPVS, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEntitiesInPVS, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnMakeVectors, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnAngleVectors, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCreateEntity, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRemoveEntity, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCreateNamedEntity, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnMakeStatic, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEntIsOnFloor, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDropToFloor, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWalkMove, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetOrigin, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEmitSound, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEmitAmbientSound, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceLine, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceToss, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceMonsterHull, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceHull, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceModel, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceTexture, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTraceSphere, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetAimVector, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnServerCommand, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnServerExecute, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnClientCommand, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnParticleEffect, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnLightStyle, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDecalIndex, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPointContents, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnMessageBegin, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnMessageEnd, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteByte, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteChar, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteShort, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteLong, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteAngle, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteCoord, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteString, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnWriteEntity, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarRegister, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarGetFloat, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarGetString, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarSetFloat, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarSetString, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnAlertMessage, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEngineFprintf, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPvAllocEntPrivateData, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPvEntPrivateData, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFreeEntPrivateData, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSzFromIndex, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnAllocString, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetVarsOfEnt, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPEntityOfEntOffset, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEntOffsetOfPEntity, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnIndexOfEdict, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPEntityOfEntIndex, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFindEntityByVars, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetModelPtr, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRegUserMsg, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnAnimationAutomove, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetBonePosition, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFunctionFromName, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnNameForFunction, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnClientPrintf, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnServerPrint, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCmd_Args, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCmd_Argv, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCmd_Argc, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetAttachment, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCRC32_Init, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCRC32_ProcessBuffer, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCRC32_ProcessByte, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCRC32_Final, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRandomLong, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRandomFloat, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetView, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnTime, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCrosshairAngle, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnLoadFileForMe, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFreeFile, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEndSection, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCompareFileTime, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetGameDir, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCvar_RegisterVariable, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnFadeClientVolume, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetClientMaxspeed, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCreateFakeClient, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRunPlayerMove, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnNumberOfEntities, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetInfoKeyBuffer, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnInfoKeyValue, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetKeyValue, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetClientKeyValue, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnIsMapValid, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnStaticDecal, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPrecacheGeneric, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPlayerUserId, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnBuildSoundMsg, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnIsDedicatedServer, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCVarGetPointer, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPlayerWONId, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnInfo_RemoveKey, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPhysicsKeyValue, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetPhysicsKeyValue, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPhysicsInfoString,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPrecacheEvent, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnPlaybackEvent, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetFatPVS, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetFatPAS, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCheckVisibility, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaSetField, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaUnsetField, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaAddEncoder, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetCurrentPlayer, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCanSkipPlayer, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaFindField, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaSetFieldByIndex, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnDeltaUnsetFieldByIndex, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSetGroupMask, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCreateInstancedBaseline, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnCvar_DirectSet, 			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnForceUnmodified, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPlayerStats, 		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnAddServerCommand, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnVoice_GetClientListening, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnVoice_SetClientListening, 	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetPlayerAuthId,			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSequenceGet,				pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnSequencePickSentence,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetFileSize,				pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetApproxWavePlayLen,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnIsCareerMatch,			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetLocalizedStringLength,			pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnRegisterTutorMessageShown,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnGetTimesTutorMessageShown,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnProcessTutorMessageDecayBuffer,	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnConstructTutorMessageDecayBuffer,	pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnResetTutorMessageDecayData,		pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnQueryClientCvarValue,				pre_str, post_str); \
	SHOW_IFDEF(api_table, engine_info, pfnEngCheckParm,			pre_str, post_str);
