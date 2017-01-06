#include "precompiled.h"

hudtextparms_t default_csay_tparms = {
	-1, 0.25, // x, y
	2, // effect
	0, 255, 0, 0, // r, g, b,  a1
	0, 0, 0, 0, // r2, g2, b2,  a2
	0, 0, 10, 10, // fadein, fadeout, hold, fxtime
	1 // channel
};

// Log to console; newline added.
void EXT_FUNC mutil_LogConsole(plid_t plid, const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	unsigned int len;

	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	// end msg with newline
	len = Q_strlen(buf);
	if (len < sizeof(buf) - 2) // -1 null, -1 for newline
		Q_strcat(buf, "\n");
	else
		buf[len - 1] = '\n';

	SERVER_PRINT(buf);
}

// Log regular message to logs; newline added.
void EXT_FUNC mutil_LogMessage(plid_t plid, const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	plugin_info_t *plinfo = (plugin_info_t *)plid;

	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	ALERT(at_logged, "[%s] %s\n", plinfo->logtag, buf);
}

// Log an error message to logs; newline added.
void EXT_FUNC mutil_LogError(plid_t plid, const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	plugin_info_t *plinfo = (plugin_info_t *)plid;

	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	ALERT(at_logged, "[%s] ERROR: %s\n", plinfo->logtag, buf);
}

// Log a message only if cvar "developer" set; newline added.
void EXT_FUNC mutil_LogDeveloper(plid_t plid, const char* fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	plugin_info_t* plinfo;

	if ((int) CVAR_GET_FLOAT("developer") == 0)
		return;

	plinfo = (plugin_info_t *)plid;
	va_start(ap, fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	ALERT(at_logged, "[%s] dev: %s\n", plinfo->logtag, buf);
}

// Print message on center of all player's screens.  Uses default text
// parameters (color green, 10 second fade-in).
void EXT_FUNC mutil_CenterSay(plid_t plid, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	mutil_CenterSayVarargs(plid, default_csay_tparms, fmt, ap);
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
	int n;
	edict_t *pEntity;

	Q_vsnprintf(buf, sizeof(buf), fmt, ap);

	mutil_LogMessage(plid, "(centersay) %s", buf);
	for (n = 1; n <= gpGlobals->maxClients; n++)
	{
		pEntity = INDEXENT(n);
		if (FNullEnt(pEntity) || pEntity->free)
			continue;
		//META_UTIL_HudMessage(pEntity, tparms, buf); // TODO
	}
}

// Allow plugins to call the entity functions in the GameDLL.  In
// particular, calling "player()" as needed by most Bots.  Suggested by
// Jussi Kivilinna.
qboolean EXT_FUNC mutil_CallGameEntity(plid_t plid, const char *entStr, entvars_t *pev)
{
	plugin_info_t *plinfo = (plugin_info_t *)plid;
	META_DEBUG(8, ("Looking up game entity '%s' for plugin '%s'", entStr, plinfo->name));
	ENTITY_FN pfnEntity = (ENTITY_FN) DLSYM(GameDLL.handle, entStr);
	if (!pfnEntity)
	{
		META_ERROR("Couldn't find game entity '%s' in game DLL '%s' for plugin '%s'", entStr, GameDLL.name, plinfo->name);
		return false;
	}

	META_DEBUG(7, ("Calling game entity '%s' for plugin '%s'", entStr, plinfo->name));
	(*pfnEntity)(pev);
	return true;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgname, and return remaining info about it (msgid, size).
int EXT_FUNC mutil_GetUserMsgID(plid_t plid, const char* msgname, int* size)
{
	plugin_info_t *plinfo = (plugin_info_t *)plid;
	META_DEBUG(8, ("Looking up usermsg name '%s' for plugin '%s'", msgname, plinfo->name));

	MRegMsg *umsg = g_regMsgs->find(msgname);
	if (umsg)
	{
		if (size)
			*size = umsg->size;
		return umsg->msgid;
	}
	else
		return 0;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgid, and return remaining info about it (msgname, size).
const char* EXT_FUNC mutil_GetUserMsgName(plid_t plid, int msgid, int *size)
{
	plugin_info_t *plinfo = (plugin_info_t *)plid;
	META_DEBUG(8, ("Looking up usermsg id '%d' for plugin '%s'", msgid, plinfo->name));

	// Guess names for any built-in g_engine messages mentioned in the SDK;
	// from dlls/util.h.
	if (msgid < 64)
	{
		switch (msgid)
		{
		case SVC_TEMPENTITY:
			if (size) *size = -1;
			return "tempentity?";
		case SVC_INTERMISSION:
			if (size) *size = -1;
			return "intermission?";
		case SVC_CDTRACK:
			if (size) *size = -1;
			return "cdtrack?";
		case SVC_WEAPONANIM:
			if (size) *size = -1;
			return "weaponanim?";
		case SVC_ROOMTYPE:
			if (size) *size = -1;
			return "roomtype?";
		case SVC_DIRECTOR:
			if (size) *size = -1;
			return "director?";
		}
	}

	MRegMsg *umsg = g_regMsgs->find(msgid);
	if (umsg)
	{
		if (size)
			*size = umsg->size;
		// 'name' is assumed to be a constant string, allocated in the
		// gamedll.
		return umsg->name;
	}
	else
		return NULL;
}

// Return the full path of the plugin's loaded dll/so file.
const char* EXT_FUNC mutil_GetPluginPath(plid_t plid)
{
	static char buf[PATH_MAX ];
	MPlugin *plug;

	plug = g_plugins->find(plid);
	if (!plug)
	{
		META_ERROR("GetPluginPath: couldn't find plugin '%s'", plid->name);
		return NULL;
	}

	Q_strncpy(buf, plug->pathname, sizeof buf - 1);
	buf[sizeof buf - 1] = '\0';
	return buf;
}

// Return various string-based info about the game/MOD/gamedll.
const char* EXT_FUNC mutil_GetGameInfo(plid_t plid, ginfo_t type)
{
	static char buf[MAX_STRBUF_LEN];
	const char *cp;
	switch (type)
	{
	case GINFO_NAME:
		cp = GameDLL.name;
		break;
	case GINFO_DESC:
		cp = GameDLL.desc;
		break;
	case GINFO_GAMEDIR:
		cp = GameDLL.gamedir;
		break;
	case GINFO_DLL_FULLPATH:
		cp = GameDLL.pathname;
		break;
	case GINFO_DLL_FILENAME:
		cp = GameDLL.file;
		break;
	case GINFO_REALDLL_FULLPATH:
		cp = GameDLL.real_pathname;
		break;
	default:
		META_ERROR("GetGameInfo: invalid request '%d' from plugin '%s'", type, plid->name);
		return NULL;
	}

	Q_strncpy(buf, cp, sizeof buf - 1);
	buf[sizeof buf - 1] = '\0';
	return buf;
}

int EXT_FUNC mutil_LoadMetaPlugin(plid_t plid, const char* fname, PLUG_LOADTIME now, void **plugin_handle)
{
	MPlugin *pl_loaded;
	if (!fname)
	{
		return ME_ARGUMENT;
	}

	meta_errno = ME_NOERROR;
	if (!(pl_loaded = g_plugins->plugin_addload(plid, fname, now)))
	{
		if (plugin_handle)
			*plugin_handle = nullptr;

		return meta_errno;
	}
	else
	{
		if (plugin_handle)
			*plugin_handle = (void *)pl_loaded->handle;
		return 0;
	}
}

int EXT_FUNC mutil_UnloadMetaPlugin(plid_t plid, const char *fname, PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	MPlugin *findp = nullptr;
	int pindex;
	char *endptr;

	if (!fname)
	{
		return ME_ARGUMENT;
	}

	pindex = strtol(fname, &endptr, 10);
	if (*fname != '\0' && *endptr == '\0')
		findp = g_plugins->find(pindex);
	else
		findp = g_plugins->find_match(fname);

	if (!findp)
		return meta_errno;

	meta_errno = ME_NOERROR;

	if (findp->plugin_unload(plid, now, reason))
		return 0;

	return meta_errno;
}

int EXT_FUNC mutil_UnloadMetaPluginByHandle(plid_t plid, void *plugin_handle, PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	MPlugin *findp;

	if (!plugin_handle)
	{
		return ME_ARGUMENT;
	}

	if (!(findp = g_plugins->find((DLHANDLE)plugin_handle)))
		return ME_NOTFOUND;

	meta_errno = ME_NOERROR;

	if (findp->plugin_unload(plid, now, reason))
		return 0;

	return meta_errno;
}

const char* EXT_FUNC mutil_IsQueryingClientCvar(plid_t plid, const edict_t* pEdict)
{
	return g_players.is_querying_cvar(pEdict);
}

int EXT_FUNC mutil_MakeRequestId(plid_t plid)
{
	//the offset is to distinguish from gamedll requests, if any
	return abs(0xbeef << 16) + (++requestid_counter);
}

void EXT_FUNC mutil_GetHookTables(plid_t plid, enginefuncs_t** peng, DLL_FUNCTIONS** pdll, NEW_DLL_FUNCTIONS** pnewdll)
{
	if (peng)
		*peng = &meta_engfuncs;

	if (pdll)
		*pdll = pHookedDllFunctions;

	if (pnewdll)
		*pnewdll = pHookedNewDllFunctions;
}

// Meta Utility Function table.
mutil_funcs_t MetaUtilFunctions = {
	mutil_LogConsole,		// pfnLogConsole
	mutil_LogMessage,		// pfnLogMessage
	mutil_LogError,			// pfnLogError
	mutil_LogDeveloper,		// pfnLogDeveloper
	mutil_CenterSay,		// pfnCenterSay
	mutil_CenterSayParms,		// pfnCenterSayParms
	mutil_CenterSayVarargs,		// pfnCenterSayVarargs
	mutil_CallGameEntity,		// pfnCallGameEntity
	mutil_GetUserMsgID,		// pfnGetUserMsgID
	mutil_GetUserMsgName,		// pfnGetUserMsgName
	mutil_GetPluginPath,		// pfnGetPluginPath
	mutil_GetGameInfo,		// pfnGetGameInfo
	mutil_LoadMetaPlugin,		// pfnLoadPlugin
	mutil_UnloadMetaPlugin,		// pfnUnloadPlugin
	mutil_UnloadMetaPluginByHandle,	// pfnUnloadPluginByHandle
	mutil_IsQueryingClientCvar,	// pfnIsQueryingClientCvar
	mutil_MakeRequestId,		// pfnMakeRequestId
	mutil_GetHookTables,		// pfnGetHookTables
};
