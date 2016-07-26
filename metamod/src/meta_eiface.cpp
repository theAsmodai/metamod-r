#include "precompiled.h"

meta_new_dll_functions_t::meta_new_dll_functions_t(
	void (*_pfnOnFreeEntPrivateData)(edict_t*),
	void (*_pfnGameShutdown)(void),
	int (*_pfnShouldCollide)(edict_t*, edict_t*),
	void (*_pfnCvarValue)(const edict_t*, const char*),
	void (*_pfnCvarValue2)(const edict_t*, int, const char*, const char*)
)
{
	pfnOnFreeEntPrivateData = _pfnOnFreeEntPrivateData;
	pfnGameShutdown = _pfnGameShutdown;
	pfnShouldCollide = _pfnShouldCollide;
	pfnCvarValue = _pfnCvarValue;
	pfnCvarValue2 = _pfnCvarValue2;
}

void meta_new_dll_functions_t::copy_to(NEW_DLL_FUNCTIONS *_pFuncs)
{
#if 0
	// TODO: Implemented check regamedll
	// exit

#endif
	Q_memcpy(_pFuncs, this, sizeof(NEW_DLL_FUNCTIONS));
}

void HL_enginefuncs_t::initialise_interface(enginefuncs_t* _pFuncs)
{
	set_from(_pFuncs);

	// Now the pfnAlertMessage is available and we trust it to be a valid
	// pointer, so flush the message buffer.
	flush_ALERT_buffer();
}
