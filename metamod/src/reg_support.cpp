#include "precompiled.h"

void EXT_FUNC meta_command_handler()
{
	const char *cmd = CMD_ARGV(0);
	MRegCmd *icmd = g_regCmds->find(cmd);

	if (!icmd)
	{
		META_ERROR("Couldn't find registered plugin command: %s", cmd);
		return;
	}

	if (!icmd->call())
	{
		META_CONS("[metamod: command '%s' unavailable; plugin unloaded]", cmd);
	}
}

// Replacement for engine routine AddServerCommand; called by plugins.
// Rather then handing the engine the plugin's command string and function
// pointer (both of which are allocated in the plugin DLL), this hands the
// engine a command string and function pointer allocated locally (in the
// metamod DLL).
//
// The string handed to the engine is just a Q_strdup() of the plugin's
// string.  The function pointer handed to the engine is actually a pointer
// to a generic command-handler function (see above).
void EXT_FUNC meta_AddServerCommand(char *cmd_name, void (*function)())
{
	MPlugin *plug = g_plugins->find_memloc(function);

	META_DEBUG(4, "called: meta_AddServerCommand; cmd_name=%s, function=%d, plugin=%s", cmd_name, function, plug ? plug->file : "unknown");

	if (!plug) {
		META_ERROR("Failed to find memloc for regcmd '%s'", cmd_name);
	}

	// See if this command was previously registered, ie a "reloaded" plugin.
	auto cmd = g_regCmds->find(cmd_name);
	if (!cmd)
	{
		// If not found, add.
		cmd = g_regCmds->add(cmd_name, function, plug);
		REG_SVR_COMMAND(cmd->getname(), g_RehldsFuncs ? cmd->gethandler() : meta_command_handler);
	}
}


// Replacement for engine routine CVarRegister; called by plugins.  Rather
// then handing the engine the plugin's cvar structure (which is allocated
// in the plugin DLL), this hands the engine a cvar structure allocated
// locally (in the metamod DLL).
//
// The cvar handed to the engine is globally allocated in the metamod.dll;
// the "name" and "string" fields are Q_strdup()'s of the plugin's strings.
// Note that, once this is done, the cvar_t allocated in the plugin is no
// longer used for _anything_.  As long as everything sets/gets the cvar
// values via the engine functions, this will work fine.  If the plugin
// code tries to _directly_ read/set the fields of its own cvar structures,
// it will fail to work properly.
void EXT_FUNC meta_CVarRegister(cvar_t *pCvar)
{
	MPlugin *plug = g_plugins->find_memloc(pCvar);

	META_DEBUG(4, "called: meta_CVarRegister; name=%s", pCvar->name);

	// try to find which plugin is registering this cvar
	if (!plug)
	{
		META_DEBUG(1, "Failed to find memloc for regcvar '%s'", pCvar->name);
	}

	// See if this cvar was previously registered, ie a "reloaded" plugin.
	auto reg = g_regCvars->find(pCvar->name);

	if (!reg)
	{
		reg = g_regCvars->add(pCvar, plug);
		CVAR_REGISTER(reg->getcvar());
	}
}

// Replacement for engine routine RegUserMsg; called by plugins.  Rather
// than handing the engine the plugin's string (which is allocated in the
// plugin DLL), this hands the engine a string allocated from the stack.
//
// Note that while the above two functions maintain a list of registered
// strings/funcs/cvars from the plugins, this is not done here as I
// couldn't identify a need for it.  Since the engine function merely maps
// a string to an int (msgid) for subsequent use in MessageBegin/etc,
// there's no function that needs to be provided and caught (like the
// commands) nor any useful actions to perform upon plugin unload (like the
// commands and cvars).  This merely provides differently located storage
// for the string.

int EXT_FUNC meta_RegUserMsg(const char *pszName, int iSize)
{
	char *cp = Q_strdup(pszName);
	return REG_USER_MSG(cp, iSize);
}

// Intercept and record queries
void EXT_FUNC meta_QueryClientCvarValue(const edict_t *player, const char *cvarName)
{
	g_players.set_player_cvar_query(player, cvarName);
	g_engfuncs.pfnQueryClientCvarValue(player, cvarName);
}
