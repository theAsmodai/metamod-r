#include "precompiled.h"

CExtDll g_meta_extdll;

CExtDll::CExtDll()
	: m_hGameDLL(CSysModule::INVALID_HANDLE)
{
}

void CExtDll::load()
{
	if (g_RehldsFuncs)
	{
		if (!g_RehldsFuncs->AddExtDll(m_hGameDLL)) {
			Sys_Error("Failure to add extension DLL; exiting...\n");
		}

		return;
	}
}

void CExtDll::unload()
{
	if (g_RehldsFuncs) {
		g_RehldsFuncs->RemoveExtDll(m_hGameDLL);
		return;
	}
}

bool CExtDll::init(CSysModule *module)
{
	m_hGameDLL = g_GameDLL.sys_module.gethandle();
	if (!m_hGameDLL)
		return false;

	return true;
}
