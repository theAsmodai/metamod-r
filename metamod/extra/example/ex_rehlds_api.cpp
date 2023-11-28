#include <extdll.h>
#include <meta_api.h>
#include "ex_rehlds_api.h"

IRehldsApi* g_RehldsApi;
const RehldsFuncs_t* g_RehldsFuncs;
IRehldsHookchains* g_RehldsHookchains;
IRehldsServerStatic* g_RehldsSvs;

bool rehlds_api_init(CSysModule* engineModule)
{
	if (!engineModule) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "Failed to locate engine module\n");
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
	if (!ifaceFactory) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "Failed to locate interface factory in engine module\n");
		return false;
	}

	int retCode = 0;
	g_RehldsApi = (IRehldsApi*)ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode);
	if (!g_RehldsApi) {
		return false;
	}

	int majorVersion = g_RehldsApi->GetMajorVersion();
	int minorVersion = g_RehldsApi->GetMinorVersion();

	if (majorVersion != REHLDS_API_VERSION_MAJOR) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "REHLDS Api major version mismatch; expected %d, real %d\n", REHLDS_API_VERSION_MAJOR, majorVersion);
		return false;
	}

	if (minorVersion < REHLDS_API_VERSION_MINOR) {
		gpMetaUtilFuncs->pfnLogConsole(PLID, "REHLDS Api minor version mismatch; expected at least %d, real %d\n", REHLDS_API_VERSION_MINOR, minorVersion);
		return false;
	}

	g_RehldsFuncs = g_RehldsApi->GetFuncs();
	g_RehldsHookchains = g_RehldsApi->GetHookchains();
	g_RehldsSvs = g_RehldsApi->GetServerStatic();

	return true;
}

bool meta_init_rehlds_api()
{
#ifdef _WIN32
	// Find the most appropriate module handle from a list of DLL candidates
	// Notes:
	// - "swds.dll" is the library Dedicated Server
	//
	//    Let's also attempt to locate the ReHLDS API in the client's library
	// - "sw.dll" is the client library for Software render, with a built-in listenserver
	// - "hw.dll" is the client library for Hardware render, with a built-in listenserver
	const char *dllNames[] = { "swds.dll", "hw.dll", "sw.dll" }; // List of DLL candidates to lookup for the ReHLDS API
	CSysModule *engineModule = NULL; // The module handle of the selected DLL
	for (const char *dllName : dllNames)
	{
		if (engineModule = Sys_GetModuleHandle(dllName))
			break; // gotcha
	}

#else
	CSysModule *engineModule = Sys_GetModuleHandle("engine_i486.so");
#endif

	if (!rehlds_api_init(engineModule)) {
		return false;
	}

	return true;
}
