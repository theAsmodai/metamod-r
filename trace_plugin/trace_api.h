// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// trace_api.h - (main) description of HL API tracing operations

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef TRACE_API_H
#define TRACE_API_H

#include <time.h>				// time()

#include <enginecallback.h>		// ALERT()
#include <sdk_util.h>			// UTIL_VarArgs()

#include "api_info.h"

#define API_TRACE(api_info_table, cvar_trace, api_str, pfnName, post, args) \
	do { if((cvar_trace->value >= api_info_table.pfnName.loglevel || api_info_table.pfnName.trace) && (unlimit_trace->value || (last_trace_log != time(NULL)))) { \
			ALERT(at_logged, "[%s] %s(%d): called: %s%s; %s\n", \
					Plugin_info.logtag, api_str, \
					api_info_table.pfnName.loglevel, \
					api_info_table.pfnName.name, \
					(post ? "_Post" : ""), \
					UTIL_VarArgs args ); \
			last_trace_log=time(NULL); \
		} \
	} while(0)

#define DLL_TRACE(pfnName, post, args) \
	API_TRACE(dllapi_info, dllapi_trace, "dllapi", pfnName, post, args)

#define NEWDLL_TRACE(pfnName, post, args) \
	API_TRACE(newapi_info, newapi_trace, "newapi", pfnName, post, args)

#define ENGINE_TRACE(pfnName, post, args) \
	API_TRACE(engine_info, engine_trace, "engine", pfnName, post, args)

typedef enum {
	TR_FAILURE = 0,
	TR_SUCCESS,
	TR_ALREADY,
} TRACE_RESULT;

#define MAX_REG_MESSAGES	256

extern time_t last_trace_log;

extern const char *msg_dest_types[32];

extern cvar_t init_dllapi_trace;
extern cvar_t init_newapi_trace;
extern cvar_t init_engine_trace;
extern cvar_t init_unlimit_trace;

extern cvar_t *dllapi_trace;
extern cvar_t *newapi_trace;
extern cvar_t *engine_trace;
extern cvar_t *unlimit_trace;

void trace_init(void);

void svr_trace(void);
void cmd_trace_usage(void);
void cmd_trace_version(void);
void cmd_trace_set(void);
void cmd_trace_unset(void);
void cmd_trace_show(void);
void cmd_trace_list(void);

TRACE_RESULT trace_setflag(const char **pfn_string, mBOOL flagval, const char **api);

#endif /* TRACE_API_H */
