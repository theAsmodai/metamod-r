#include "precompiled.h"

CExtDll g_meta_extdll;

CExtDll::CExtDll()
	: m_dlls(nullptr), m_count(nullptr), m_hGameDLL(CSysModule::INVALID_HANDLE)
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

	auto extdll = &m_dlls[(*m_count)++];
	Q_memset(extdll, 0, sizeof(*extdll));
	extdll->lDLLHandle = m_hGameDLL;
}

void CExtDll::unload()
{
	if (g_RehldsFuncs) {
		g_RehldsFuncs->RemoveExtDll(m_hGameDLL);
		return;
	}

	for (auto i = 0u; i < *m_count; i++)
	{
		if (m_dlls[i].lDLLHandle == m_hGameDLL)
		{
			--(*m_count);
			if (*m_count != i)
			{
				Q_memmove(&m_dlls[i], &m_dlls[i + 1], (*m_count - i) * sizeof(m_dlls[0]));
				i = *m_count;
			}

			Q_memset(&m_dlls[i], 0, sizeof(m_dlls[0]));
			return;
		}
	}
}

bool CExtDll::init(CSysModule *module)
{
	m_hGameDLL = g_GameDLL.sys_module.gethandle();

	if (g_RehldsFuncs) {
		return true;
	}

	m_dlls = (extensiondll_t *)module->getsym("g_rgextdll");
	m_count = (size_t *)module->getsym("g_iextdllMac");

	if (m_dlls && m_count) {
		return true;
	}

	// HACK HACK MORE HACK
	// HLDS SHIT
	auto pos = module->find_string_push("SV_SaveGameComment");
	if (!pos) {
		return false;
	}

	const char patterns[2][2][14] = {
		{ "\x56\x33\xF6\x39\x2A\x2A\x2A\x2A\x2A\x7E\x2A\x53\x8B", "\x5B\xC3\x90\x90\xA1\x2A\x2A\x2A\x2A\x53\x56\x57\x33" },
		{ "\x53\x8B\x2A\x2A\x2A\x2A\x2A\x57\xBF\x2A\x2A\x2A\x2A", "\x7E\x2A\x8B\x2A\x2A\x2A\x2A\x2A\xBE\x2A\x2A\x2A\x2A" },
	};

	for (auto i = 0u; i < ARRAYSIZE(patterns); i++)
	{
		auto addr = mem_find_pattern(pos - 33, 20, patterns[0][i], sizeof(patterns[0][i]) - 1);
		if (!addr) {
			continue;
		}

		m_count = *(size_t **)(addr + 5);

		addr = mem_find_pattern(addr, 30, patterns[1][i], sizeof(patterns[1][i]) - 1);
		if (!addr) {
			return false;
		}

		m_dlls = *(extensiondll_t **)(addr + 9);
		return true;
	}

	return false;
}
