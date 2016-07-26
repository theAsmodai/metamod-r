#include "precompiled.h"

#ifdef _WIN32
// Required DLL entry point
// The above SDK comment indicates this routine is required, but the MSDN
// documentation indicates it's actually optional.  We keep it, though, for
// completeness. 
// Note: 'extern "C"' needed for mingw compile.
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		metamod_handle = hinstDLL;
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		/* nothing */
	}

	return TRUE;
}
#else
// Linux routines to correspond to ATTACH and DETACH cases above.  These
// aren't required by linux, but are included here for completeness, and
// just in case we come across a need to do something at dll load or
// unload.
void _init() {
	// called before dlopen() returns
}

void _fini() {
	// called before dlclose() returns
}
#endif

// Holds engine functionality callbacks
HL_enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;
engine_t Engine;

// Receive engine function table from engine.
//
// This appears to be the _first_ DLL routine called by the engine, so this
// is where we hook to load all the other DLLs (game, plugins, etc), which
// is actually all done in meta_startup().
C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals)
{
#ifndef _WIN32
	metamod_handle = get_module_handle_of_memptr((void*)&g_engfuncs);
#endif

	gpGlobals = pGlobals;
	Engine.funcs = &g_engfuncs;
	Engine.globals = pGlobals;

	g_engfuncs.initialise_interface(pengfuncsFromEngine);

	// NOTE: Have to call logging function _after_ initialising g_engfuncs, so
	// that g_engfuncs.pfnAlertMessage() can be resolved properly, heh. :)
	META_DEV("called: GiveFnptrsToDll");
	// Load plugins, load game dll.
	if (!metamod_startup())
	{
		metamod_not_loaded = 1;
	}

	return;
}
