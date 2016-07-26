#pragma once

// This file is a wrapper around the SDK's enginecallback.h file. We need
// this because we use a different type for the global object g_engfuncs,
// which is still compatible with the enginefuncs_t that the SDK
// uses.
// This is only done for files that belong to Metamod, not other projects
// like plugins that use this file, or others that include it, too.
// Since we don't have a clean seperation of include files right now we
// "hack" our way around that by using a flag METAMOD_CORE which is set
// when compiling Metamod proper.

#ifdef METAMOD_CORE
	#include "meta_eiface.h" // HL_enginefuncs_t

	// Use a #define to bend the enginefuncs_t type to our HL_enginefuncs_t
	// type instead as we now use that for the global object g_engfuncs.
	#define enginefuncs_t HL_enginefuncs_t
#endif

#include <enginecallback.h>		// ALERT, etc

#ifdef METAMOD_CORE
#undef enginefuncs_t
#endif

// Also, create some additional macros for engine callback functions, which
// weren't in SDK dlls/enginecallbacks.h but probably should have been.
#define GET_INFOKEYBUFFER		(*g_engfuncs.pfnGetInfoKeyBuffer)
#define INFOKEY_VALUE			(*g_engfuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEYVALUE		(*g_engfuncs.pfnSetClientKeyValue)
#define REG_SVR_COMMAND			(*g_engfuncs.pfnAddServerCommand)
#define SERVER_PRINT			(*g_engfuncs.pfnServerPrint)
#define SET_SERVER_KEYVALUE		(*g_engfuncs.pfnSetKeyValue)
#define QUERY_CLIENT_CVAR_VALUE		(*g_engfuncs.pfnQueryClientCvarValue)
#define QUERY_CLIENT_CVAR_VALUE2	(*g_engfuncs.pfnQueryClientCvarValue2)
