#pragma once

#include "eiface.h"             // engfuncs_t, globalvars_t
#include "comp_dep.h"

// Our structure for storing engine references.
struct engine_t {
	engine_t();
	engine_t(const engine_t&);
	engine_t& operator=(const engine_t&);

	enginefuncs_t *funcs;		// engine funcs
	globalvars_t *globals;		// engine globals
	enginefuncs_t *pl_funcs;	// "modified" eng funcs we give to plugins
};

inline engine_t::engine_t() 
	: funcs(NULL), globals(NULL), pl_funcs(NULL)
{
}


inline engine_t::engine_t(const engine_t& _rhs) 
	: funcs(_rhs.funcs), globals(_rhs.globals), pl_funcs(_rhs.pl_funcs)
{
}


inline engine_t& engine_t::operator=(const engine_t& _rhs) 
{
	funcs = _rhs.funcs;
	globals = _rhs.globals;
	pl_funcs = _rhs.pl_funcs;
	return *this;
}

extern engine_t Engine;
