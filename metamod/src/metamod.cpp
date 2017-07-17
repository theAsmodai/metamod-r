#include "precompiled.h"

cvar_t g_meta_version = { "metamod_version", APP_VERSION_STRD, FCVAR_SERVER, 0, nullptr };

MConfig g_static_config;
MConfig *g_config = &g_static_config;
option_t g_global_options[] =
{
	{ "debuglevel",		CF_INT,		&g_config->m_debuglevel,	"0" },
	{ "gamedll",		CF_PATH,	&g_config->m_gamedll,		nullptr },
	{ "exec_cfg",		CF_STR,		&g_config->m_exec_cfg,		nullptr },
	{ "clientmeta",		CF_BOOL,	&g_config->m_clientmeta,	"no" },

	// list terminator
	{ nullptr, CF_NONE, nullptr, nullptr }
};

gamedll_t g_GameDLL;

ALIGN16
meta_globals_t g_metaGlobals;

MPluginList *g_plugins;
MRegCmdList *g_regCmds;
MRegCvarList *g_regCvars;
MRegMsgList *g_regMsgs;

MPlayerList g_players;

unsigned int g_CALL_API_count = 0;

int g_requestid_counter = 0;

// Very first metamod function that's run.
// Do startup operations...
void metamod_startup()
{
	char configFile[MAX_PATH];
	char pluginFile[MAX_PATH];
	char execFile[MAX_PATH];

	Q_snprintf(configFile, sizeof configFile, "%s/%s", g_config->directory(), CONFIG_INI);
	Q_snprintf(pluginFile, sizeof pluginFile, "%s/%s", g_config->directory(), PLUGINS_INI);
	Q_snprintf(execFile, sizeof execFile, "%s/%s", g_config->directory(), EXEC_CFG);

	META_CONS("   ");
	META_CONS("   Metamod-r version %s Copyright (c) 2016-2017 ReHlds Team (rebuild of original Metamod by Will Day)", APP_VERSION_STRD);
	META_CONS("   Metamod-r comes with ABSOLUTELY NO WARRANTY; for details type `meta gpl'.");
	META_CONS("   This is free software, and you are welcome to redistribute it");
	META_CONS("   under certain conditions; type `meta gpl' for details.");
	META_CONS("   ");

	META_CONS("Metamod-r v%s, API (%s)", APP_VERSION_STRD, META_INTERFACE_VERSION);
	META_CONS("Metamod-r build: " __TIME__ " " __DATE__ " (" APP_VERSION_STRD ")");
	META_CONS("Metamod-r from: " APP_COMMITS_URL APP_COMMIT_ID " " APP_COMMIT_AUTHOR "");

	// Get gamedir, very early on, because it seems we need it all over the
	// place here at the start.
	if (!meta_init_gamedll()) {
		META_ERROR("Failure to init game DLL; exiting...");
		do_exit(1);
	}

	// Register various console commands and cvars.
	// Can I do these here, rather than waiting for GameDLLInit() ?
	// Looks like it works okay..
	meta_register_cmdcvar();

	// Set a slight debug level for developer mode, if debug level not
	// already set.
	if (CVAR_GET_FLOAT("developer") != 0.0 && g_meta_debug.value == 0.0)
		CVAR_SET_FLOAT("meta_debug", 3.0);

	// Init default values
	g_config->init(g_global_options);

	// Find config file
	const char *cp = LOCALINFO("mm_configfile");
	if (cp && *cp != '\0') {
		META_LOG("Configfile specified via localinfo: %s", cp);

		if (is_file_exists_in_gamedir(cp)) {
			Q_strncpy(configFile, cp, sizeof configFile - 1);
			configFile[sizeof configFile - 1] = '\0';
		}
		else
			META_ERROR("Empty/missing config.ini file: %s; falling back to %s", cp, configFile);
	}

	if (!is_file_exists_in_gamedir(configFile)) {
		Q_strncpy(configFile, g_config->directory(), sizeof configFile - 1);
		configFile[sizeof configFile - 1] = '\0';

		// Get out of sub directory and check
		char *dir = Q_strrchr(configFile, '/');
		if (dir) {
			*dir = '\0';
		}

		Q_strcat(configFile, "/" CONFIG_INI);
		if (!is_file_exists_in_gamedir(configFile)) {
			META_DEBUG(2, "No config.ini file found: %s", CONFIG_INI);
		}
	}

	// Load config file
	if (is_file_exists_in_gamedir(configFile))
		g_config->load(configFile);

	// Now, override config options with localinfo commandline options.
	cp = LOCALINFO("mm_debug");
	if (cp && *cp != '\0') {
		META_LOG("Debuglevel specified via localinfo: %s", cp);
		g_config->set("debuglevel", cp);
	}

	cp = LOCALINFO("mm_gamedll");
	if (cp && *cp != '\0') {
		META_LOG("Gamedll specified via localinfo: %s", cp);
		g_config->set("gamedll", cp);
	}

	cp = LOCALINFO("mm_execcfg");
	if (cp && *cp != '\0') {
		META_LOG("Execcfg specified via localinfo: %s", cp);
		g_config->set("exec_cfg", cp);
	}

	cp = LOCALINFO("mm_clientmeta");
	if (cp && *cp != '\0') {
		META_LOG("Clientmeta specified via localinfo: %s", cp);
		g_config->set("clientmeta", cp);
	}

	// Check for an initial debug level, since cfg files don't get exec'd
	// until later.
	if (g_config->m_debuglevel)
		CVAR_SET_FLOAT("meta_debug", g_config->m_debuglevel);
	if (!g_config->m_clientmeta)
		disable_clientcommand_fwd();

	// Prepare for registered commands from plugins.
	g_regCmds = new MRegCmdList();
	g_regCvars = new MRegCvarList();

	// Prepare for registered user messages from gamedll.
	g_regMsgs = new MRegMsgList();

	// Copy, and store pointer in g_engine struct.  Yes, we could just store
	// the actual engine_t struct in g_engine, but then it wouldn't be a
	// pointer to match the other g_engfuncs.
	g_engine.pl_funcs = *g_engine.funcs;
	// substitute our special versions of various commands
	g_engine.pl_funcs.pfnAddServerCommand = meta_AddServerCommand;
	g_engine.pl_funcs.pfnCVarRegister = meta_CVarRegister;
	g_engine.pl_funcs.pfnCvar_RegisterVariable = meta_CVarRegister;
	g_engine.pl_funcs.pfnRegUserMsg = meta_RegUserMsg;

	if (g_engine.pl_funcs.pfnQueryClientCvarValue)
		g_engine.pl_funcs.pfnQueryClientCvarValue = meta_QueryClientCvarValue;

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
	// However, having replaced valid_file with FileExistsInGameDir, we need
	// to at least initialize the gameDLL to include the gamedir, before
	// looking for plugins.ini.
	//
	// In fact, we need gamedir even earlier, so moved up above.

	// Load plugins file
	if (!is_file_exists_in_gamedir(pluginFile)) {
		Q_strncpy(pluginFile, g_config->directory(), sizeof pluginFile - 1);
		pluginFile[sizeof pluginFile - 1] = '\0';

		// Get out of sub directory and check
		char *dir = Q_strrchr(pluginFile, '/');
		if (dir) {
			*dir = '\0';
		}

		Q_strcat(pluginFile, "/" PLUGINS_INI);
		if (!is_file_exists_in_gamedir(pluginFile)) {
			META_DEBUG(2, "No plugins.ini file found: %s", PLUGINS_INI);
		}
	}

	g_plugins = new MPluginList(pluginFile);

	if (!meta_load_gamedll()) {
		META_ERROR("Failure to load game DLL; exiting...");
		do_exit(1);
	}

	if (!g_plugins->load()) {
		META_ERROR("Failure to load plugins...");
		// Exit on failure here?  Dunno...
	}

	meta_init_rehlds_api();

	// Allow for commands to metamod plugins at startup.  Autoexec.cfg is
	// read too early, and server.cfg is read too late.
	//
	// Only attempt load if the file appears to exist and be non-empty, to
	// avoid confusing users with "couldn't exec exec.cfg" console
	// messages.
	if (g_config->m_exec_cfg) {
		Q_strncpy(execFile, g_config->m_exec_cfg, sizeof execFile - 1);
		execFile[sizeof execFile - 1] = '\0';
	}

	if (is_file_exists_in_gamedir(execFile)) {
		if (execFile[0] == '/')
			META_ERROR("Cannot exec absolute pathnames: %s", execFile);
		else {
			char cmd[NAME_MAX];
			META_LOG("Exec'ing metamod exec.cfg: %s...", execFile);
			Q_snprintf(cmd, sizeof cmd, "exec %s\n", execFile);
			SERVER_COMMAND(cmd);
		}
	}
}

// Set initial GameDLL fields (name, gamedir).
// meta_errno values:
//  - ME_NULLRESULT	_getcwd failed
bool meta_init_gamedll()
{
	Q_memset(&g_GameDLL, 0, sizeof g_GameDLL);

	char gamedir[PATH_MAX];
	GET_GAME_DIR(gamedir);
	normalize_path(gamedir);

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
	if (is_abs_path(gamedir)) {
		// Old style; GET_GAME_DIR returned full pathname.  Copy this into
		// our gamedir, and truncate to get the game name.
		// (note check for both linux and win32 full pathname.)
		Q_strncpy(g_GameDLL.gamedir, gamedir, sizeof g_GameDLL.gamedir - 1);
		g_GameDLL.gamedir[sizeof g_GameDLL.gamedir - 1] = '\0';

		char* cp = Q_strrchr(gamedir, '/') + 1;

		Q_strncpy(g_GameDLL.name, cp, sizeof g_GameDLL.name - 1);
		g_GameDLL.name[sizeof g_GameDLL.name - 1] = '\0';
	}
	else {
		// New style; GET_GAME_DIR returned game name.  Copy this into our
		// game name, and prepend the current working directory.
		char buf[PATH_MAX];
		if (!_getcwd(buf, sizeof buf)) {
			META_WARNING("dll: Couldn't get cwd; %s", strerror(errno));
			return false;
		}

		Q_snprintf(g_GameDLL.gamedir, sizeof g_GameDLL.gamedir, "%s/%s", buf, gamedir);
		Q_strncpy(g_GameDLL.name, gamedir, sizeof g_GameDLL.name - 1);
		g_GameDLL.name[sizeof g_GameDLL.name - 1] = '\0';
	}

	META_DEBUG(3, "Game: %s", g_GameDLL.name);
	return true;
}

template<typename ifvers_t, typename table_t>
bool get_function_table(const char* ifname, int ifvers_mm, table_t*& table, size_t table_size = sizeof(table_t))
{
	typedef int(*getfunc_t)(table_t *pFunctionTable, ifvers_t interfaceVersion);

	auto pfnGetFuncs = (getfunc_t)g_GameDLL.sys_module.getsym(ifname);

	if (pfnGetFuncs) {
		table = (table_t *)Q_calloc(1, table_size);

		int ifvers_gamedll = ifvers_mm;

		if (pfnGetFuncs(table, ifvers_gamedll)) {
			META_DEBUG(3, "dll: Game '%s': Found %s", g_GameDLL.name, ifname);
			return true;
		}

		META_ERROR("dll: Failure calling %s in game '%s'", ifname, g_GameDLL.name);
		Q_free(table);
		table = nullptr;

		if (ifvers_gamedll != ifvers_mm) {
			META_ERROR("dll: Interface version didn't match; we wanted %d, they had %d", ifvers_mm, ifvers_gamedll);
			META_CONS("==================");
			META_CONS("Game DLL version mismatch");
			META_CONS("DLL version is %d, engine version is %d", ifvers_gamedll, ifvers_mm);
			if (ifvers_gamedll > ifvers_mm)
				META_CONS("g_engine appears to be outdated, check for updates");
			else
				META_CONS("The game DLL for %s appears to be outdated, check for updates", g_GameDLL.name);
			META_CONS("==================");
			ALERT(at_error, "Exiting...\n");
		}
	}
	else {
		META_DEBUG(5, "dll: Game '%s': No %s", g_GameDLL.name, ifname);
		table = nullptr;
	}

	return false;
}

template<typename table_t>
bool get_function_table_old(const char* ifname, int ifvers_mm, table_t*& table, size_t table_size = sizeof(table_t))
{
	typedef int(*getfunc_t)(table_t *pFunctionTable, int interfaceVersion);

	auto pfnGetFuncs = (getfunc_t)g_GameDLL.sys_module.getsym(ifname);

	if (pfnGetFuncs) {
		table = (table_t *)Q_calloc(1, table_size);

		if (pfnGetFuncs(table, ifvers_mm)) {
			META_DEBUG(3, "dll: Game '%s': Found %s", g_GameDLL.name, ifname);
			return true;
		}

		META_ERROR("dll: Failure calling %s in game '%s'", ifname, g_GameDLL.name);
		Q_free(table);
		table = nullptr;
	}
	else {
		META_DEBUG(5, "dll: Game '%s': No %s", g_GameDLL.name, ifname);
		table = nullptr;
	}

	return false;
}

// Load game DLL.
// meta_errno values:
//  - ME_DLOPEN		couldn't dlopen game dll file
//  - ME_DLMISSING	couldn't find required routine in game dll
//                	(GiveFnptrsToDll, GetEntityAPI, GetEntityAPI2)
bool meta_load_gamedll()
{
	if (!setup_gamedll(&g_GameDLL)) {
		META_ERROR("dll: Unrecognized game: %s", g_GameDLL.name);
		// meta_errno should be already set in lookup_game()
		return false;
	}

	// open the game DLL
	if (!g_GameDLL.sys_module.load(g_GameDLL.pathname)) {
		META_ERROR("dll: Couldn't load game DLL %s: %s", g_GameDLL.pathname, CSysModule::getloaderror());
		return false;
	}

	// prepare meta_engfuncs
	compile_engine_callbacks();

	// Used to only pass our table of engine funcs if a loaded plugin
	// wanted to catch one of the functions, but now that plugins are
	// dynamically loadable at any time, we have to always pass our table,
	// so that any plugin loaded later can catch what they need to.
	auto pfn_give_engfuncs = (GIVE_ENGINE_FUNCTIONS_FN)g_GameDLL.sys_module.getsym("GiveFnptrsToDll");
	if (pfn_give_engfuncs) {
		pfn_give_engfuncs(&g_meta_engfuncs, gpGlobals);
		META_DEBUG(3, "dll: Game '%s': Called GiveFnptrsToDll", g_GameDLL.name);
	}
	else {
		META_ERROR("dll: Couldn't find GiveFnptrsToDll() in game DLL '%s'", g_GameDLL.name);
		return false;
	}

	// Look for API-NEW interface in Game dll.  We do this before API2/API, because
	// that's what the engine appears to do..
	get_function_table<int&>("GetNewDLLFunctions", NEW_DLL_FUNCTIONS_VERSION, g_GameDLL.funcs.newapi_table);

	// Look for API2 interface in plugin; preferred over API-1.
	bool found = get_function_table<int&>("GetEntityAPI2", INTERFACE_VERSION, g_GameDLL.funcs.dllapi_table);

	// Look for API-1 in plugin, if API2 interface wasn't found.
	if (!found) {
		found = get_function_table_old("GetEntityAPI", INTERFACE_VERSION, g_GameDLL.funcs.dllapi_table);
	}

	// If didn't find either, return failure.
	if (!found) {
		META_ERROR("dll: Couldn't find either GetEntityAPI nor GetEntityAPI2 in game DLL '%s'", g_GameDLL.name);
		return false;
	}

	// prepare gamedll callbacks
	compile_gamedll_callbacks();

	META_LOG("Game DLL for '%s' loaded successfully", g_GameDLL.desc);
	return true;
}

struct fixdata_t
{
	uint32* pret;
	uint32 callback;
};

static void meta_collect_fix_data(uint32* const esp, std::vector<fixdata_t>& data)
{
	uint32* pret = esp;

	// scan for retaddr
	do pret--;
	while (!g_jit.is_callback_retaddr(*pret));

	char* raddr = (char *)*pret;
	size_t args_count = 0;

	if ((raddr[0] == 0x83 && raddr[1] == 0xC4)) // add esp, XX
		args_count = raddr[2] / 4;

	// 8B 0D 4E 61 BC 00        mov     ecx, ds:0BC614Eh
	// 80 3D 4E 61 BC 00 05     cmp     byte ptr ds:0BC614Eh, 5
	char pattern[] = "\x8B\x0D\x2A\x2A\x2A\x2A\x80\x3D";

	// scan for callback addr
	do raddr--;
	while (!mem_compare(raddr, pattern, sizeof pattern - 1));

	uint32 callback = *(uint32 *)(raddr + 2); // mov ecx, [callback]
	data.push_back({pret, callback});

	// 0F 29 44 24 1C               movaps  xmmword ptr [esp + 1Ch], xmm0
	// 66 0F D6 4C 24 14            movq    mmword ptr [esp + 14h], xmm1

	// 0F 29 84 24 36 05 00 00      movaps  xmmword ptr [esp + 536h], xmm0
	// 66 0F D6 8C 24 36 05 00 00   movq    qword ptr [esp + 536h], xmm1
	char pattern2[] = "\x66\x0F\xD6\x2A\x24";

	// scan for mg_backup at stack
	do raddr--;
	while (!mem_compare(raddr, pattern2, sizeof pattern2 - 1));

	uint32 mg_backup;
	if (raddr[3] == 0x4C)
		mg_backup = *(uint8 *)&raddr[sizeof pattern2 - 1];
	else
		mg_backup = *(uint32 *)&raddr[sizeof pattern2 - 1];

	uint32* esp_save = *(uint32 **)(uint32(esp) + mg_backup + sizeof(int));
	if (esp_save)
		meta_collect_fix_data(esp_save, data);
}

static void meta_apply_fix_data(std::vector<fixdata_t>& data)
{
	for (auto& d : data) {
		// 8B 0D 4E 61 BC 00    mov     ecx, ds:0BC614Eh
		char pattern[6] = "\x8B\x0D";
		*(uint32 *)(pattern + 2) = d.callback;

		char* ptr = g_jit.find_callback_pattern(pattern, sizeof pattern);
		if (!ptr) {
			META_ERROR("Failed to fix callback retaddr.\n Bye bye...\n");
			do_exit(666);
		}

		// FF D1        call    ecx
		// 83 C4 20     add     esp, 20h ; optional
		// 8B 13        mov     edx, [ebx]
		do ptr++;
		while (!mem_compare(ptr, "\xFF\xD1\x83\xC4", 4) && !mem_compare(ptr, "\xFF\xD1\x8B\x13", 4));

		*d.pret = uint32(ptr + 2);
	}
}

void meta_rebuild_callbacks()
{
	std::vector<fixdata_t> fixdata;
	if (g_metaGlobals.esp_save) {
		META_LOG("dll: Begin scan to collect callback fix data...");
		meta_collect_fix_data(g_metaGlobals.esp_save, fixdata);
	}

	META_LOG("dll: Rebuilding callbacks...");

#ifdef JIT_DEBUG_FILE
	mdebug_to_file("dll: Rebuilding callbacks...\n");
#endif

	g_jit.clear_callbacks();

	compile_engine_callbacks();
	compile_gamedll_callbacks();

	if (fixdata.size()) {
		META_LOG("dll: Begin callbacks fixing...");
		meta_apply_fix_data(fixdata);
	}

	META_LOG("dll: Callbacks rebuilded.");
}
