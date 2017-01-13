#include "precompiled.h"

void meta_new_dll_functions_t::set_from(NEW_DLL_FUNCTIONS* _pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(NEW_DLL_FUNCTIONS));
}

void meta_enginefuncs_t::set_from(enginefuncs_t* _pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(enginefuncs_t));
}

void HL_enginefuncs_t::initialise_interface(enginefuncs_t* _pFuncs)
{
	set_from(_pFuncs);

	// Now the pfnAlertMessage is available and we trust it to be a valid
	// pointer, so flush the message buffer.
	flush_ALERT_buffer();
}
