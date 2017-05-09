#include "precompiled.h"

//! Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t* gpGlobals;
engine_t g_engine;

// Receive engine function table from engine.
//
// This appears to be the _first_ DLL routine called by the engine, so this
// is where we hook to load all the other DLLs (game, plugins, etc), which
// is actually all done in meta_startup().
void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals)
{
	gpGlobals = pGlobals;
	g_engine.funcs = &g_engfuncs;
	g_engine.globals = pGlobals;

	g_engfuncs = *pengfuncsFromEngine;
	flush_ALERT_buffer();
	// NOTE!  Have to call logging function _after_ initialising g_engfuncs, so
	// that g_engfuncs.pfnAlertMessage() can be resolved properly, heh. :)
	META_DEV("called: GiveFnptrsToDll");

	// Load plugins, load game dll.
	metamod_startup();
}
