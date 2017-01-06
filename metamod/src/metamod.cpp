#include "precompiled.h"

cvar_t meta_version = { "metamod_version", APP_VERSION_STRD, FCVAR_SERVER, 0, nullptr };

MConfig static_config;
MConfig *g_config = &static_config;
option_t global_options[] =
{
	{ "debuglevel", CF_INT, &g_config->debuglevel, "0" },
	{ "plugins_file", CF_PATH, &g_config->plugins_file, PLUGINS_INI },
	{ "exec_cfg", CF_STR, &g_config->exec_cfg, EXEC_CFG },

	// list terminator
	{ NULL, CF_NONE, NULL, NULL }
};

gamedll_t GameDLL;

meta_globals_t g_metaGlobals;

meta_enginefuncs_t g_plugin_engfuncs;

MPluginList *g_plugins;
MRegCmdList *g_regCmds;
MRegCvarList *g_regCvars;
MRegMsgList *g_regMsgs;

MPlayerList g_players;

unsigned int CALL_API_count = 0;

int requestid_counter = 0;

#ifdef UNFINISHED
MHookList *Hooks;
#endif // UNFINISHED

// Very first metamod function that's run.
// Do startup operations...
void metamod_startup()
{
	const char *mmfile = NULL;
	const char *cfile = NULL;
	const char *cp;

	META_CONS("   ");
	META_CONS("   Metamod version %s Copyright (c) 2001-2016 Will Day (modification by ReHLDS Team)", APP_VERSION_STRD);
	META_CONS("   Metamod comes with ABSOLUTELY NO WARRANTY; for details type `meta gpl'.");
	META_CONS("   This is free software, and you are welcome to redistribute it");
	META_CONS("   under certain conditions; type `meta gpl' for details.");
	META_CONS("   ");

	META_CONS("Metamod v%s, API (%s)", APP_VERSION_STRD, META_INTERFACE_VERSION);
	META_CONS("Metamod build: " __TIME__ " " __DATE__ " (" APP_VERSION_STRD ")");
	META_CONS("Metamod from: " APP_COMMITS_URL APP_COMMIT_ID " " APP_COMMIT_AUTHOR "");

	// Get gamedir, very early on, because it seems we need it all over the
	// place here at the start.
	if (!meta_init_gamedll())
	{
		META_ERROR("Failure to init game DLL; exiting...");
		do_exit(1);
	}

	// Register various console commands and cvars.
	// Can I do these here, rather than waiting for GameDLLInit() ?
	// Looks like it works okay..
	meta_register_cmdcvar();

	// Set a slight debug level for developer mode, if debug level not
	// already set.
	if ((int) CVAR_GET_FLOAT("developer") != 0 && meta_debug.value == 0)
		CVAR_SET_FLOAT("meta_debug", 3.0);

	// Init default values
	g_config->init(global_options);
	// Find config file
	cfile = CONFIG_INI;
	if ((cp = LOCALINFO("mm_configfile")) && *cp != '\0')
	{
		META_LOG("Configfile specified via localinfo: %s", cp);
		if (valid_gamedir_file(cp))
			cfile = cp;
		else
			META_ERROR("Empty/missing config.ini file: %s; falling back to %s", cp, cfile);
	}
	// Load config file
	if (valid_gamedir_file(cfile))
		g_config->load(cfile);
	else
		META_DEBUG(2, ("No config.ini file found: %s", CONFIG_INI));

	// Now, override config options with localinfo commandline options.
	if ((cp = LOCALINFO("mm_debug")) && *cp != '\0')
	{
		META_LOG("Debuglevel specified via localinfo: %s", cp);
		g_config->set("debuglevel", cp);
	}
	if ((cp = LOCALINFO("mm_gamedll")) && *cp != '\0')
	{
		META_LOG("Gamedll specified via localinfo: %s", cp);
		g_config->set("gamedll", cp);
	}
	if ((cp = LOCALINFO("mm_pluginsfile")) && *cp != '\0')
	{
		META_LOG("Pluginsfile specified via localinfo: %s", cp);
		g_config->set("plugins_file", cp);
	}
	if ((cp = LOCALINFO("mm_execcfg")) && *cp != '\0')
	{
		META_LOG("Execcfg specified via localinfo: %s", cp);
		g_config->set("exec_cfg", cp);
	}

	// Check for an initial debug level, since cfg files don't get exec'd
	// until later.
	if (g_config->debuglevel != 0)
		CVAR_SET_FLOAT("meta_debug", g_config->debuglevel);

	// Prepare for registered commands from plugins.
	g_regCmds = new MRegCmdList();
	g_regCvars = new MRegCvarList();

	// Prepare for registered user messages from gamedll.
	g_regMsgs = new MRegMsgList();

	// Copy, and store pointer in g_engine struct.  Yes, we could just store
	// the actual engine_t struct in g_engine, but then it wouldn't be a
	// pointer to match the other g_engfuncs.
	g_plugin_engfuncs.set_from(g_engine.funcs);
	g_engine.pl_funcs = &g_plugin_engfuncs;
	// substitute our special versions of various commands
	g_engine.pl_funcs->pfnAddServerCommand = meta_AddServerCommand;
	g_engine.pl_funcs->pfnCVarRegister = meta_CVarRegister;
	g_engine.pl_funcs->pfnCvar_RegisterVariable = meta_CVarRegister;
	g_engine.pl_funcs->pfnRegUserMsg = meta_RegUserMsg;

	if (g_engine.pl_funcs->pfnQueryClientCvarValue)
		g_engine.pl_funcs->pfnQueryClientCvarValue = meta_QueryClientCvarValue;

	// Before, we loaded plugins before loading the game DLL, so that if no
	// plugins caught engine functions, we could pass engine funcs straight
	// to game dll, rather than acting as intermediary.  (Should perform
	// better, right?)
	//
	// But since a plugin can be loaded at any time, we have to go ahead
	// and catch the engine funcs regardless.  Also, we want to give each
	// plugin a copy of the gameDLL's api tables, in case they need to call
	// API functions directly.
	//
	// Thus, load gameDLL first, then plugins.
	//
	// However, we have to init the g_plugins object first, because if the
	// gamedll calls engine functions during GiveFnptrsToDll (like hpb_bot
	// does) then it needs to be non-null so META_ENGINE_HANDLE won't crash.
	//
	// However, having replaced valid_file with valid_gamedir_file, we need
	// to at least initialize the gameDLL to include the gamedir, before
	// looking for plugins.ini.
	//
	// In fact, we need gamedir even earlier, so moved up above.

	// Fall back to old plugins filename, if configured one isn't found.
	mmfile = PLUGINS_INI;

	if (!valid_gamedir_file(PLUGINS_INI) && valid_gamedir_file(OLD_PLUGINS_INI))
		mmfile = OLD_PLUGINS_INI;
	if (valid_gamedir_file(g_config->plugins_file))
		mmfile = g_config->plugins_file;
	else
		META_ERROR("g_plugins file is empty/missing: %s; falling back to %s", g_config->plugins_file, mmfile);

	g_plugins = new MPluginList(mmfile);

	if (!meta_load_gamedll())
	{
		META_ERROR("Failure to load game DLL; exiting...");
		do_exit(1);
	}

	if (!g_plugins->load())
	{
		META_ERROR("Failure to load plugins...");
		// Exit on failure here?  Dunno...
	}

#ifdef UNFINISHED
	// Start up the log parsing thread.
	startup_logparse_thread();
#endif

	// Allow for commands to metamod plugins at startup.  Autoexec.cfg is
	// read too early, and server.cfg is read too late.
	//
	// Only attempt load if the file appears to exist and be non-empty, to
	// avoid confusing users with "couldn't exec exec.cfg" console
	// messages.
	if (valid_gamedir_file(g_config->exec_cfg))
		mmfile = g_config->exec_cfg;

	else if (valid_gamedir_file(OLD_EXEC_CFG))
		mmfile = OLD_EXEC_CFG;
	else
		mmfile = NULL;

	if (mmfile)
	{
		if (mmfile[0] == '/')
			META_ERROR("Cannot exec absolute pathnames: %s", mmfile);
		else
		{
			char cmd[NAME_MAX];
			META_LOG("Exec'ing metamod exec.cfg: %s...", mmfile);
			Q_snprintf(cmd, sizeof(cmd), "exec %s\n", mmfile);
			SERVER_COMMAND(cmd);
		}
	}
}

// Set initial GameDLL fields (name, gamedir).
// meta_errno values:
//  - ME_NULLRESULT	_getcwd failed
bool meta_init_gamedll(void)
{
	char gamedir[PATH_MAX];
	char *cp;

	Q_memset(&GameDLL, 0, sizeof(GameDLL));

	GET_GAME_DIR(gamedir);
	normalize_pathname(gamedir);
	//
	// As of 1.1.1.1, the engine routine GET_GAME_DIR no longer returns a
	// full-pathname, but rather just the path specified as the argument to
	// "-game".
	//
	// However, since we have to work properly under both the new version
	// as well as previous versions, we have to support both possibilities.
	//
	// Note: the code has always assumed the server op wouldn't do:
	//    hlds -game other/firearms
	//
	if (is_absolute_path(gamedir))
	{
		// Old style; GET_GAME_DIR returned full pathname.  Copy this into
		// our gamedir, and truncate to get the game name.
		// (note check for both linux and win32 full pathname.)
		Q_strncpy(GameDLL.gamedir, gamedir, sizeof(GameDLL.gamedir) - 1);
		GameDLL.gamedir[sizeof(GameDLL.gamedir) - 1] = '\0';

		cp = Q_strrchr(gamedir, '/') + 1;

		Q_strncpy(GameDLL.name, cp, sizeof(GameDLL.name) - 1);
		GameDLL.name[sizeof(GameDLL.name) - 1] = '\0';
	}
	else
	{
		// New style; GET_GAME_DIR returned game name.  Copy this into our
		// game name, and prepend the current working directory.
		char buf[PATH_MAX];
		if (!_getcwd(buf, sizeof(buf)))
		{
			META_WARNING("dll: Couldn't get cwd; %s", strerror(errno));
			RETURN_ERRNO(false, ME_NULLRESULT);
		}

		Q_snprintf(GameDLL.gamedir, sizeof(GameDLL.gamedir), "%s/%s", buf, gamedir);
		Q_strncpy(GameDLL.name, gamedir, sizeof(GameDLL.name) - 1);
		GameDLL.name[sizeof(GameDLL.name) - 1] = '\0';
	}

	META_DEBUG(3, ("Game: %s", GameDLL.name));
	return true;
}

// Load game DLL.
// meta_errno values:
//  - ME_DLOPEN		couldn't dlopen game dll file
//  - ME_DLMISSING	couldn't find required routine in game dll
//                	(GiveFnptrsToDll, GetEntityAPI, GetEntityAPI2)
bool meta_load_gamedll(void)
{
	int iface_vers;
	int found = 0;

	GIVE_ENGINE_FUNCTIONS_FN pfn_give_engfuncs;
	GETNEWDLLFUNCTIONS_FN pfn_getapinew;
	GETENTITYAPI2_FN pfn_getapi2;
	GETENTITYAPI_FN pfn_getapi;

	if (!setup_gamedll(&GameDLL))
	{
		META_ERROR("dll: Unrecognized game: %s", GameDLL.name);
		// meta_errno should be already set in lookup_game()
		return false;
	}

	// open the game DLL
	if (!(GameDLL.handle = DLOPEN(GameDLL.pathname)))
	{
		META_ERROR("dll: Couldn't load game DLL %s: %s", GameDLL.pathname, DLERROR());
		RETURN_ERRNO(false, ME_DLOPEN);
	}

	// prepare meta_engfuncs
	compile_engine_callbacks();

	// Used to only pass our table of engine funcs if a loaded plugin
	// wanted to catch one of the functions, but now that plugins are
	// dynamically loadable at any time, we have to always pass our table,
	// so that any plugin loaded later can catch what they need to.
	if ((pfn_give_engfuncs = (GIVE_ENGINE_FUNCTIONS_FN) DLSYM(GameDLL.handle, "GiveFnptrsToDll")))
	{
		pfn_give_engfuncs(&meta_engfuncs, gpGlobals);
		META_DEBUG(3, ("dll: Game '%s': Called GiveFnptrsToDll", GameDLL.name));
	}
	else
	{
		META_ERROR("dll: Couldn't find GiveFnptrsToDll() in game DLL '%s': %s", GameDLL.name, DLERROR());
		RETURN_ERRNO(false, ME_DLMISSING);
	}

	// Yes...another macro.
#define GET_FUNC_TABLE_FROM_GAME(gamedll, pfnGetFuncs, STR_GetFuncs, struct_field, API_TYPE, TABLE_TYPE, vers_pass, vers_int, vers_want, gotit) \
		if ((pfnGetFuncs = (API_TYPE) DLSYM(gamedll.handle, STR_GetFuncs))) { \
			gamedll.funcs.struct_field = (TABLE_TYPE *)Q_calloc(1, sizeof(TABLE_TYPE)); \
			if (!gamedll.funcs.struct_field) {\
				META_ERROR("malloc failed for gamedll struct_field: %s", STR_GetFuncs); \
			} \
			else if (pfnGetFuncs(gamedll.funcs.struct_field, vers_pass)) { \
				META_DEBUG(3, ("dll: Game '%s': Found %s", gamedll.name, STR_GetFuncs)); \
				gotit = 1; \
			} \
			else { \
				META_ERROR("dll: Failure calling %s in game '%s'", STR_GetFuncs, gamedll.name); \
				Q_free(gamedll.funcs.struct_field); \
				gamedll.funcs.struct_field = NULL; \
				if (vers_int != vers_want) { \
					META_ERROR("dll: Interface version didn't match; we wanted %d, they had %d", vers_want, vers_int); \
					/* reproduce error from engine */ \
					META_CONS("=================="); \
					META_CONS("Game DLL version mismatch"); \
					META_CONS("DLL version is %d, engine version is %d", vers_int, vers_want); \
					if (vers_int > vers_want) \
						META_CONS("g_engine appears to be outdated, check for updates"); \
					else \
						META_CONS("The game DLL for %s appears to be outdated, check for updates", GameDLL.name); \
					META_CONS("=================="); \
					ALERT(at_error, "Exiting...\n"); \
				} \
			} \
		} \
		else { \
			META_DEBUG(5, ("dll: Game '%s': No %s", gamedll.name, STR_GetFuncs)); \
			gamedll.funcs.struct_field = NULL; \
		}

	// Look for API-NEW interface in Game dll.  We do this before API2/API, because
	// that's what the engine appears to do..
	iface_vers = NEW_DLL_FUNCTIONS_VERSION;
	GET_FUNC_TABLE_FROM_GAME(GameDLL, pfn_getapinew, "GetNewDLLFunctions", newapi_table, GETNEWDLLFUNCTIONS_FN, meta_new_dll_functions_t, &iface_vers, iface_vers, NEW_DLL_FUNCTIONS_VERSION, found);

	// Look for API2 interface in plugin; preferred over API-1.
	found = 0;
	iface_vers = INTERFACE_VERSION;
	GET_FUNC_TABLE_FROM_GAME(GameDLL, pfn_getapi2, "GetEntityAPI2", dllapi_table, GETENTITYAPI2_FN, DLL_FUNCTIONS, &iface_vers, iface_vers, INTERFACE_VERSION, found);

	// Look for API-1 in plugin, if API2 interface wasn't found.
	if (!found)
	{
		found = 0;
		GET_FUNC_TABLE_FROM_GAME(GameDLL, pfn_getapi, "GetEntityAPI", dllapi_table, GETENTITYAPI_FN, DLL_FUNCTIONS, INTERFACE_VERSION, INTERFACE_VERSION, INTERFACE_VERSION, found);
	}

	// If didn't find either, return failure.
	if (!found)
	{
		META_ERROR("dll: Couldn't find either GetEntityAPI nor GetEntityAPI2 in game DLL '%s'", GameDLL.name);
		RETURN_ERRNO(false, ME_DLMISSING);
	}

	// prepare gamedll callbacks
	compile_gamedll_callbacks();

	META_LOG("Game DLL for '%s' loaded successfully", GameDLL.desc);
	return true;
}

void meta_rebuild_callbacks()
{
	g_jit.clear_callbacks();

	compile_engine_callbacks();
	compile_gamedll_callbacks();
}
