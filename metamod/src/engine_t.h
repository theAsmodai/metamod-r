#pragma once

// Our structure for storing engine references.
struct engine_t
{
	engine_t() : funcs(nullptr), globals(nullptr), pl_funcs(nullptr)
	{
	}

	enginefuncs_t* funcs; // engine funcs
	globalvars_t* globals; // engine globals
	enginefuncs_t* pl_funcs; // "modified" eng funcs we give to plugins
};

extern engine_t g_engine;
