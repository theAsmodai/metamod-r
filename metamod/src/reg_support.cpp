#include "precompiled.h"

// "Register" support.
//
// This code is necessary to support the different "register" engine
// functions that can be called by plugins, so that the plugins can be
// safely unloaded without incurring segmentation faults.
//
// The problem was that, when a plugin called, say:
//    REG_SVR_COMMAND("stub_version", svr_stub_version);
//
// the engine stored the address to the "stub_version" string, as well as,
// of course, the svr_stub_version() function pointer.  If the plugin was
// unloaded, and dlclose()'d, the memory locations for both the string and
// function became invalid and unavailable.  If the engine tried to
// strcmp() against the string, or tried to call the function pointer, it
// generated a segfault and killed the process.
//
// The solution used here is to create strings and functions allocated
// locally, in metamod.dll, which won't become unavailable or invalid due
// to a dlclose().  These local copies are passed to the engine instead of
// those allocated in the plugin DLLs.  Thus, when a plugin is later
// unloaded and dlclose'd, the engine can still reference the memory
// locations of registered strings and functions, without segfaulting.
//
// Now, what happens with a registered string or function after a plugin is
// unloaded not quite as nice as I'd like.
//
// Console commands will print a message along the lines of:
//    [metamod: command '%s' unavailable; plugin unloaded]
//
// Cvars though, have their value unaffected.  This can be convenient when
// reloading a plugin, as the cvars keep their old values.  However it can
// also be confusing, as there is no indication when viewing/setting cvars
// from the console that they no longer have an effect.
//
// Also note, the console commands for listing registered cmds and cvars
// will try to show the name of the associated plugin.  However, this only
// works under linux; under windows it will say "(unknown)".  Linux
// provides a routine "dladdr()" which can indicate (during Reg request)
// which plugin it's coming from, and this is stored with the cvar entry.
// Windows didn't seem to have a similar routine, and I couldn't find
// another way to get the information..

// Generic command handler, passed to the engine for any AddServerCommand
// calls made by the plugin.  It finds the appropriate plugin function
// pointer to call based on CMD_ARGV(0).
void meta_command_handler()
{
	META_DEBUG(5, ("called: meta_command_handler; arg0=%s args='%s'", CMD_ARGV(0), CMD_ARGS()));
	const char *cmd = CMD_ARGV(0);
	if (!cmd)
	{
		META_ERROR("Null command name in meta_command_handler() ??");
		return;
	}

	MRegCmd *icmd = g_regCmds->find(cmd);
	if (!icmd)
	{
		META_ERROR("Couldn't find registered plugin command: %s", cmd);
		return;
	}

	if (icmd->call() != mTRUE)
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
void meta_AddServerCommand(char *cmd_name, void (*function)())
{
	MPlugin *iplug = NULL;
	MRegCmd *icmd = NULL;

	META_DEBUG(4, ("called: meta_AddServerCommand; cmd_name=%s, function=%d", cmd_name, function));

	// try to find which plugin is registering this command
	if (!(iplug = g_plugins->find_memloc((void *)function)))
	{
		// if this isn't supported on this OS, don't log an error
		if (meta_errno != ME_OSNOTSUP)
			META_ERROR("Failed to find memloc for regcmd '%s'", cmd_name);
	}

	// See if this command was previously registered, ie a "reloaded" plugin.
	icmd = g_regCmds->find(cmd_name);
	if (!icmd)
	{
		// If not found, add.
		icmd = g_regCmds->add(cmd_name);
		if (!icmd)
		{
			// error details logged in add()
			return;
		}

		// Only register if not previously registered..
		REG_SVR_COMMAND(icmd->name, meta_command_handler);
	}

	icmd->pfnCmd = function;
	icmd->status = RG_VALID;
	// Store which plugin this is for, if we know.  We can use '0' for
	// unknown plugin, since plugin index starts at 1.
	if (iplug)
		icmd->plugid = iplug->index;
	else
		icmd->plugid = 0;
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
void meta_CVarRegister(cvar_t *pCvar)
{
	MPlugin *iplug = nullptr;
	MRegCvar *icvar = nullptr;

	META_DEBUG(4, ("called: meta_CVarRegister; name=%s", pCvar->name));

	// try to find which plugin is registering this cvar
	if (!(iplug = g_plugins->find_memloc((void *)pCvar)))
	{
		// if this isn't supported on this OS, don't log an error
		if (meta_errno != ME_OSNOTSUP)
		// Note: if cvar_t was malloc'd by the plugin, we can't
		// determine the calling plugin.  Thus, this becomes a Debug
		// rather than Error message.
		META_DEBUG(1, ("Failed to find memloc for regcvar '%s'", pCvar->name));
	}

	// See if this cvar was previously registered, ie a "reloaded" plugin.
	icvar = g_regCvars->find(pCvar->name);
	if (!icvar)
	{
		// If not found, add.
		icvar = g_regCvars->add(pCvar->name);
		if (!icvar)
		{
			// error details logged in add()
			return;
		}

		// Reset to given value
		icvar->set(pCvar);
		CVAR_REGISTER(icvar->data);
	}

	// Note: if not a new cvar, then we don't set the values, and just keep
	// the pre-existing value.
	icvar->status = RG_VALID;

	// Store which plugin this is for, if we know.  Use '0' for unknown
	// plugin, as plugin index starts at 1.
	if (iplug)
		icvar->plugid = iplug->index;
	else
		icvar->plugid = 0;
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

int meta_RegUserMsg(const char *pszName, int iSize)
{
	char *cp = Q_strdup(pszName);
	return REG_USER_MSG(cp, iSize);
}

// Intercept and record queries
void meta_QueryClientCvarValue(const edict_t *player, const char *cvarName)
{
	g_Players.set_player_cvar_query(player, cvarName);
	(*g_engfuncs.pfnQueryClientCvarValue)(player, cvarName);
}
