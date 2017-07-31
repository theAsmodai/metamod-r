#pragma once

#include "pr_dlls.h"

class CExtDll
{
public:
	CExtDll();
	bool init(CSysModule *module);
	void load();
	void unload();

private:
	module_handle_t m_hGameDLL;
	extensiondll_t *m_dlls;
	size_t *m_count;
};

extern CExtDll g_meta_extdll;
