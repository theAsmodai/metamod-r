#include "precompiled.h"

static hudtextparms_t g_default_csay_tparms = {
	-1, 0.25, // x, y
	2, // effect
	0, 255, 0, 0, // r, g, b,  a1
	0, 0, 0, 0, // r2, g2, b2,  a2
	0, 0, 10, 10, // fadein, fadeout, hold, fxtime
	1 // channel
};

static const char* g_engine_msg_names[] =
{
	"svc_bad",
	"svc_nop",
	"svc_disconnect",
	"svc_event",
	"svc_version",
	"svc_setview",
	"svc_sound",
	"svc_time",
	"svc_print",
	"svc_stufftext",
	"svc_setangle",
	"svc_serverinfo",
	"svc_lightstyle",
	"svc_updateuserinfo",
	"svc_deltadescription",
	"svc_clientdata",
	"svc_stopsound",
	"svc_pings",
	"svc_particle",
	"svc_damage",
	"svc_spawnstatic",
	"svc_event_reliable",
	"svc_spawnbaseline",
	"svc_temp_entity",
	"svc_setpause",
	"svc_signonnum",
	"svc_centerprint",
	"svc_killedmonster",
	"svc_foundsecret",
	"svc_spawnstaticsound",
	"svc_intermission",
	"svc_finale",
	"svc_cdtrack",
	"svc_restore",
	"svc_cutscene",
	"svc_weaponanim",
	"svc_decalname",
	"svc_roomtype",
	"svc_addangle",
	"svc_newusermsg",
	"svc_packetentities",
	"svc_deltapacketentities",
	"svc_choke",
	"svc_resourcelist",
	"svc_newmovevars",
	"svc_resourcerequest",
	"svc_customization",
	"svc_crosshairangle",
	"svc_soundfade",
	"svc_filetxferfailed",
	"svc_hltv",
	"svc_director",
	"svc_voiceinit",
	"svc_voicedata",
	"svc_sendextrainfo",
	"svc_timescale",
	"svc_resourcelocation",
	"svc_sendcvarvalue",
	"svc_sendcvarvalue2"
};

// Log to console; newline added.
void EXT_FUNC mutil_LogConsole(plid_t plid, const char* fmt, ...)
{
	char buf[MAX_LOGMSG_LEN];

	va_list ap;
	va_start(ap, fmt);
	size_t len = Q_vsnprintf(buf, sizeof buf - 1, fmt, ap);
	va_end(ap);

	buf[len] = '\n';
	buf[len + 1] = '\0';

	SERVER_PRINT(buf);
}

// Log regular message to logs; newline added.
void EXT_FUNC mutil_LogMessage(plid_t plid, const char* fmt, ...)
{
	char buf[MAX_LOGMSG_LEN];

	va_list ap;
	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);

	ALERT(at_logged, "[%s] %s\n", plid->logtag, buf);
}

// Log an error message to logs; newline added.
void EXT_FUNC mutil_LogError(plid_t plid, const char* fmt, ...)
{
	char buf[MAX_LOGMSG_LEN];

	va_list ap;
	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);

	ALERT(at_logged, "[%s] ERROR: %s\n", plid->logtag, buf);
}

// Log a message only if cvar "developer" set; newline added.
void EXT_FUNC mutil_LogDeveloper(plid_t plid, const char* fmt, ...)
{
	char buf[MAX_LOGMSG_LEN];

	if ((int)CVAR_GET_FLOAT("developer") == 0)
		return;

	va_list ap;
	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);

	ALERT(at_logged, "[%s] dev: %s\n", plid->logtag, buf);
}

// Print message on center of all player's screens.  Uses default text
// parameters (color green, 10 second fade-in).
void EXT_FUNC mutil_CenterSay(plid_t plid, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	mutil_CenterSayVarargs(plid, g_default_csay_tparms, fmt, ap);
	va_end(ap);
}

// Print a center-message, with given text parameters.
void EXT_FUNC mutil_CenterSayParms(plid_t plid, hudtextparms_t tparms, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	mutil_CenterSayVarargs(plid, tparms, fmt, ap);
	va_end(ap);
}

// Print a center-message, with text parameters and varargs.  Provides
// functionality to the above center_say interfaces.
void EXT_FUNC mutil_CenterSayVarargs(plid_t plid, hudtextparms_t tparms, const char* fmt, va_list ap)
{
	char buf[MAX_LOGMSG_LEN];
	Q_vsnprintf(buf, sizeof buf, fmt, ap);

	mutil_LogMessage(plid, "(centersay) %s", buf);

	for (int n = 1; n <= gpGlobals->maxClients; n++) {
		auto pEntity = INDEXENT(n);

		if (FNullEnt(pEntity) || pEntity->free)
			continue;

		UTIL_HudMessage(pEntity, tparms, buf);
	}
}

// Allow plugins to call the entity functions in the GameDLL.  In
// particular, calling "player()" as needed by most Bots.  Suggested by
// Jussi Kivilinna.
qboolean EXT_FUNC mutil_CallGameEntity(plid_t plid, const char* entStr, entvars_t* pev)
{
	META_DEBUG(8, "Looking up game entity '%s' for plugin '%s'", entStr, plid->name);
	ENTITY_FN pfnEntity = (ENTITY_FN)g_GameDLL.sys_module.getsym(entStr);

	if (!pfnEntity) {
		META_ERROR("Couldn't find game entity '%s' in game DLL '%s' for plugin '%s'", entStr, g_GameDLL.name, plid->name);
		return false;
	}

	META_DEBUG(7, "Calling game entity '%s' for plugin '%s'", entStr, plid->name);
	(*pfnEntity)(pev);
	return true;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgname, and return remaining info about it (msgid, size).
int EXT_FUNC mutil_GetUserMsgID(plid_t plid, const char* msgname, int* size)
{
	META_DEBUG(8, "Looking up usermsg name '%s' for plugin '%s'", msgname, plid->name);

	MRegMsg* umsg = g_regMsgs->find(msgname);

	if (umsg) {
		if (size) *size = umsg->getsize();
		return umsg->getid();
	}

	for (int n = 1; n < arraysize(g_engine_msg_names); n++) {
		if (!strcmp(msgname, g_engine_msg_names[n])) {
			if (size) *size = -1;
			return n;
		}
	}

	return 0;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgid, and return remaining info about it (msgname, size).
const char* EXT_FUNC mutil_GetUserMsgName(plid_t plid, int msgid, int* size)
{
	plugin_info_t* plinfo = (plugin_info_t *)plid;
	META_DEBUG(8, "Looking up usermsg id '%d' for plugin '%s'", msgid, plinfo->name);

	// Guess names for any built-in g_engine messages mentioned in the SDK;
	// from dlls/util.h.
	if (msgid < arraysize(g_engine_msg_names)) {
		if (size) *size = -1;
		return g_engine_msg_names[msgid];
	}

	MRegMsg* umsg = g_regMsgs->find(msgid);

	if (umsg) {
		if (size) *size = umsg->getsize();
		// 'name' is assumed to be a constant string, allocated in the
		// gamedll.
		return umsg->getname();
	}

	return nullptr;
}

// Return the full path of the plugin's loaded dll/so file.
const char* EXT_FUNC mutil_GetPluginPath(plid_t plid)
{
	static char buf[PATH_MAX];

	auto plug = g_plugins->find(plid);
	if (!plug) {
		META_ERROR("GetPluginPath: couldn't find plugin '%s'", plid->name);
		return nullptr;
	}

	Q_strncpy(buf, plug->pathname(), sizeof buf - 1);
	buf[sizeof buf - 1] = '\0';
	return buf;
}

// Return various string-based info about the game/MOD/gamedll.
const char* EXT_FUNC mutil_GetGameInfo(plid_t plid, ginfo_t type)
{
	static char buf[MAX_STRBUF_LEN];
	const char* cp;

	switch (type) {
	case GINFO_NAME:
		cp = g_GameDLL.name;
		break;
	case GINFO_DESC:
		cp = g_GameDLL.desc;
		break;
	case GINFO_GAMEDIR:
		cp = g_GameDLL.gamedir;
		break;
	case GINFO_DLL_FULLPATH:
		cp = g_GameDLL.pathname;
		break;
	case GINFO_DLL_FILENAME:
		cp = g_GameDLL.file;
		break;
	case GINFO_REALDLL_FULLPATH:
		cp = g_GameDLL.real_pathname;
		break;
	default:
		META_ERROR("GetGameInfo: invalid request '%d' from plugin '%s'", type, plid->name);
		return nullptr;
	}

	Q_strncpy(buf, cp, sizeof buf - 1);
	buf[sizeof buf - 1] = '\0';
	return buf;
}

int EXT_FUNC mutil_LoadMetaPlugin(plid_t plid, const char* fname, PLUG_LOADTIME now, void** plugin_handle)
{
	if (!fname) {
		return 1;
	}

	auto pl_loaded = g_plugins->plugin_addload(plid, fname, now);
	if (!pl_loaded) {
		if (plugin_handle)
			*plugin_handle = nullptr;

		return 1; // TODO: WTF
	}

	meta_rebuild_callbacks();

	if (plugin_handle)
		*plugin_handle = (void *)pl_loaded->sys_module().gethandle();

	return 0;
}

int EXT_FUNC mutil_UnloadMetaPlugin(plid_t plid, const char* fname, PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	MPlugin* findp;

	if (!fname) {
		return 1;
	}

	char* endptr;
	int pindex = strtol(fname, &endptr, 10);
	bool unique = true;

	if (*fname != '\0' && *endptr == '\0')
		findp = g_plugins->find(pindex);
	else
		findp = g_plugins->find_match(fname, unique);

	if (!findp || !unique)
		return 1;

	if (findp->plugin_unload(plid, now, reason)) {
		meta_rebuild_callbacks();
		return 0;
	}

	return 1;
}

int EXT_FUNC mutil_UnloadMetaPluginByHandle(plid_t plid, void* plugin_handle, PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	if (!plugin_handle) {
		return 1;
	}

	auto findp = g_plugins->find((module_handle_t)plugin_handle);

	if (!findp)
		return 1;

	if (findp->plugin_unload(plid, now, reason)) {
		meta_rebuild_callbacks();
		return 0;
	}

	return 1;
}

const char* EXT_FUNC mutil_IsQueryingClientCvar(plid_t plid, const edict_t* pEdict)
{
	return g_players.is_querying_cvar(pEdict);
}

int EXT_FUNC mutil_MakeRequestId(plid_t plid)
{
	//the offset is to distinguish from gamedll requests, if any
	return abs(0xbeef << 16) + (++g_requestid_counter);
}

void EXT_FUNC mutil_GetHookTables(plid_t plid, enginefuncs_t** peng, DLL_FUNCTIONS** pdll, NEW_DLL_FUNCTIONS** pnewdll)
{
	if (peng)
		*peng = &g_meta_engfuncs;

	if (pdll)
		*pdll = pHookedDllFunctions;

	if (pnewdll)
		*pnewdll = pHookedNewDllFunctions;
}

// Meta Utility Function table.
mutil_funcs_t g_MetaUtilFunctions = {
	mutil_LogConsole,				// pfnLogConsole
	mutil_LogMessage,				// pfnLogMessage
	mutil_LogError,					// pfnLogError
	mutil_LogDeveloper,				// pfnLogDeveloper
	mutil_CenterSay,				// pfnCenterSay
	mutil_CenterSayParms,			// pfnCenterSayParms
	mutil_CenterSayVarargs,			// pfnCenterSayVarargs
	mutil_CallGameEntity,			// pfnCallGameEntity
	mutil_GetUserMsgID,				// pfnGetUserMsgID
	mutil_GetUserMsgName,			// pfnGetUserMsgName
	mutil_GetPluginPath,			// pfnGetPluginPath
	mutil_GetGameInfo,				// pfnGetGameInfo
	mutil_LoadMetaPlugin,			// pfnLoadPlugin
	mutil_UnloadMetaPlugin,			// pfnUnloadPlugin
	mutil_UnloadMetaPluginByHandle,	// pfnUnloadPluginByHandle
	mutil_IsQueryingClientCvar,		// pfnIsQueryingClientCvar
	mutil_MakeRequestId,			// pfnMakeRequestId
	mutil_GetHookTables,			// pfnGetHookTables
};
