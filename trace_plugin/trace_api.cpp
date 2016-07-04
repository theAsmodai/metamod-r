// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// trace_api.cpp - (main) implementation of HL API tracing operations

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

#include <extdll.h>			// always

#include <api_info.h>		// api_info_t, etc
#include <sdk_util.h>		// REG_SVR_COMMAND, etc
#include <meta_api.h>		// Plugin_info, etc

#include "trace_api.h"		// me
#include "log_plugin.h"		// LOG_MSG, etc
#include "vers_meta.h"		// OPT_TYPE
#include "vdate.h"			// COMPILE_TIME

cvar_t init_dllapi_trace = {"trace_dllapi", "0", FCVAR_EXTDLL, 0, NULL};
cvar_t init_newapi_trace = {"trace_newapi", "0", FCVAR_EXTDLL, 0, NULL};
cvar_t init_engine_trace = {"trace_engine", "0", FCVAR_EXTDLL, 0, NULL};
cvar_t init_unlimit_trace =  {"trace_unlimit", "0", FCVAR_EXTDLL, 0, NULL};

cvar_t *dllapi_trace = NULL;
cvar_t *newapi_trace = NULL;
cvar_t *engine_trace = NULL;
cvar_t *unlimit_trace = NULL;

const char *msg_dest_types[32];

time_t last_trace_log = 0;

// Plugin startup.  Register commands and cvars.
void trace_init(void) {
	CVAR_REGISTER(&init_dllapi_trace);
	CVAR_REGISTER(&init_newapi_trace);
	CVAR_REGISTER(&init_engine_trace);
	CVAR_REGISTER(&init_unlimit_trace);

	dllapi_trace=CVAR_GET_POINTER("trace_dllapi");
	newapi_trace=CVAR_GET_POINTER("trace_newapi");
	engine_trace=CVAR_GET_POINTER("trace_engine");
	unlimit_trace=CVAR_GET_POINTER("trace_unlimit");

	REG_SVR_COMMAND("trace", svr_trace);

	memset(msg_dest_types, 0, sizeof(msg_dest_types));

	msg_dest_types[MSG_BROADCAST]="all_unreliable";
	msg_dest_types[MSG_ONE]="one_reliable";
	msg_dest_types[MSG_ALL]="all_reliable";
	msg_dest_types[MSG_INIT]="init";
	msg_dest_types[MSG_PVS]="pvs_unreliable";
	msg_dest_types[MSG_PAS]="pas_unreliable";
	msg_dest_types[MSG_PVS_R]="pvs_reliable";
	msg_dest_types[MSG_PAS_R]="pas_reliable";
	msg_dest_types[MSG_ONE_UNRELIABLE]="one_unreliable";
}

// Parse "trace" console command.
void svr_trace(void) {
	const char *cmd;
	cmd=CMD_ARGV(1);
	if(!strcasecmp(cmd, "version"))
		cmd_trace_version();
	else if(!strcasecmp(cmd, "show"))
		cmd_trace_show();
	else if(!strcasecmp(cmd, "set"))
		cmd_trace_set();
	else if(!strcasecmp(cmd, "unset"))
		cmd_trace_unset();
	else if(!strcasecmp(cmd, "list"))
		cmd_trace_list();
	else {
		LOG_CONSOLE(PLID, "Unrecognized trace command: %s", cmd);
		cmd_trace_usage();
		return;
	}
}

// Print usage for "trace" console command.
void cmd_trace_usage(void) {
	LOG_CONSOLE(PLID, "usage: trace <command> [<arguments>]");
	LOG_CONSOLE(PLID, "valid commands are:");
	LOG_CONSOLE(PLID, "   version          - display plugin version info");
	LOG_CONSOLE(PLID, "   show             - show currently traced api routines");
	LOG_CONSOLE(PLID, "   set <routine>    - set tracing for given routine");
	LOG_CONSOLE(PLID, "   unset <routine>  - unset tracing for given routine");
	LOG_CONSOLE(PLID, "   list dllapi      - list all dllapi routines available for tracing");
	LOG_CONSOLE(PLID, "   list newapi      - list all newapi routines available for tracing");
	LOG_CONSOLE(PLID, "   list engine      - list all engine routines available for tracing");
	LOG_CONSOLE(PLID, "   list all         - list dllapi, neapi, and engine");
}

// "trace version" console command.
void cmd_trace_version(void) {
	LOG_CONSOLE(PLID, "%s v%s, %s", Plugin_info.name, Plugin_info.version, Plugin_info.date);
	LOG_CONSOLE(PLID, "by %s", Plugin_info.author);
	LOG_CONSOLE(PLID, "   %s", Plugin_info.url);
	LOG_CONSOLE(PLID, "compiled: %s Eastern (%s)", COMPILE_TIME, OPT_TYPE);
}

// "trace set" console command.
void cmd_trace_set(void) {
	int i, argc;
	const char *arg;
	const char *api;
	TRACE_RESULT ret;
	argc=CMD_ARGC();
	if(argc < 3) {
		LOG_CONSOLE(PLID, "usage: trace set <routine> [<routine> ...]");
		return;
	}
	for(i=2; i < argc; i++) {
		arg=CMD_ARGV(i);
		ret=trace_setflag(&arg, mTRUE, &api);
		if(ret==TR_SUCCESS)
			LOG_MESSAGE(PLID, "Tracing %s routine '%s'", api, arg);
		else if(ret==TR_ALREADY)
			LOG_CONSOLE(PLID, "Already tracing %s routine '%s'", api, arg);
		else
			LOG_CONSOLE(PLID, "Unrecognized API routine '%s'", arg);
	}
}

// "trace unset" console command.
void cmd_trace_unset(void) {
	int i, argc;
	const char *arg;
	const char *api;
	TRACE_RESULT ret;
	argc=CMD_ARGC();
	if(argc < 3) {
		LOG_CONSOLE(PLID, "usage: trace unset <routine>");
		return;
	}
	for(i=1; i < argc; i++) {
		arg=CMD_ARGV(i);
		ret=trace_setflag(&arg, mFALSE, &api);
		if(ret==TR_SUCCESS)
			LOG_MESSAGE(PLID, "Un-Tracing %s routine '%s'", api, arg);
		else if(ret==TR_ALREADY)
			LOG_CONSOLE(PLID, "Already not tracing %s routine '%s'", api, arg);
		else
			LOG_CONSOLE(PLID, "Unrecognized API routine '%s'", arg);
	}
}

// "trace show" console command.
void cmd_trace_show(void) {
	api_info_t *routine;
	int n=0;
	LOG_CONSOLE(PLID, "Tracing routines:");
	for(routine=(api_info_t *) &dllapi_info; routine->name; routine++) {
		if(routine->trace==mTRUE) {
			LOG_CONSOLE(PLID, "   %s (dllapi)", routine->name);
			n++;
		}
	}
	for(routine=(api_info_t *) &newapi_info; routine->name; routine++) {
		if(routine->trace==mTRUE) {
			LOG_CONSOLE(PLID, "   %s (newapi)", routine->name);
			n++;
		}
	}
	for(routine=(api_info_t *) &engine_info; routine->name; routine++) {
		if(routine->trace==mTRUE) {
			LOG_CONSOLE(PLID, "   %s (engine)", routine->name);
			n++;
		}
	}
	LOG_CONSOLE(PLID, "%d routines", n);
}

// "trace list" console command.
void cmd_trace_list(void) {
	api_info_t *routine;
	mBOOL valid=mFALSE;
	int n, t;
	const char *arg;
	arg=CMD_ARGV(2);
	if(!strcasecmp(arg, "all") || !strcasecmp(arg, "dllapi")) {
		valid=mTRUE;
		n=0; t=0;
		LOG_CONSOLE(PLID, "DLLAPI routines:");
		for(routine=(api_info_t *) &dllapi_info; routine->name; routine++) {
			LOG_CONSOLE(PLID, "  %c %s", 
					routine->trace ? '+' : ' ',
					routine->name);
			n++;
			if(routine->trace) t++;
		}
		LOG_CONSOLE(PLID, "%d DLLAPI routines, %d traced", n, t);
	}
	if(!strcasecmp(arg, "all") || !strcasecmp(arg, "newapi")) {
		valid=mTRUE;
		n=0; t=0;
		LOG_CONSOLE(PLID, "NEWAPI routines:");
		for(routine=(api_info_t *) &newapi_info; routine->name; routine++) {
			LOG_CONSOLE(PLID, "  %c %s", 
					routine->trace ? '+' : ' ',
					routine->name);
			n++;
			if(routine->trace) t++;
		}
		LOG_CONSOLE(PLID, "%d NEWAPI routines, %d traced", n, t);
	}
	if(!strcasecmp(arg, "all") || !strcasecmp(arg, "engine")) {
		valid=mTRUE;
		n=0; t=0;
		LOG_CONSOLE(PLID, "Engine routines:");
		for(routine=(api_info_t *) &engine_info; routine->name; routine++) {
			LOG_CONSOLE(PLID, "  %c %s", 
					routine->trace ? '+' : ' ',
					routine->name);
			n++;
			if(routine->trace) t++;
		}
		LOG_CONSOLE(PLID, "%d Engine routines, %d traced", n, t);
	}
	if(!valid) {
		LOG_CONSOLE(PLID, "usage: trace list <type>");
		LOG_CONSOLE(PLID, "where <type> is one of:");
		LOG_CONSOLE(PLID, "   dllapi    - list all dllapi routines available for tracing");
		LOG_CONSOLE(PLID, "   newapi    - list all newapi routines available for tracing");
		LOG_CONSOLE(PLID, "   engine    - list all engine routines available for tracing");
		LOG_CONSOLE(PLID, "   all       - list dllapi, newapi, engine");
	}
}

// Set or unset tracing of a given api routine string.  Searches all three
// API lists, in the order:
//    dllapi
//    newapi
//    engine
// Returns API list in which the routine was found, as well as the
// "canonicalized" routine name/string.
TRACE_RESULT trace_setflag(const char **pfn_string, mBOOL flagval, const char **api) {
	api_info_t *routine;
	for(routine=(api_info_t *) &dllapi_info; routine->name; routine++) {
		if(!strcasecmp(routine->name, *pfn_string)) {
			*pfn_string=routine->name;
			*api="DLLAPI";
			if(routine->trace==flagval)
				return(TR_ALREADY);
			routine->trace=flagval;
			return(TR_SUCCESS);
		}
	}
	for(routine=(api_info_t *) &newapi_info; routine->name; routine++) {
		if(!strcasecmp(routine->name, *pfn_string)) {
			*pfn_string=routine->name;
			*api="NEWAPI";
			if(routine->trace==flagval)
				return(TR_ALREADY);
			routine->trace=flagval;
			return(TR_SUCCESS);
		}
	}
	for(routine=(api_info_t *) &engine_info; routine->name; routine++) {
		if(!strcasecmp(routine->name, *pfn_string)) {
			*pfn_string=routine->name;
			*api="Engine";
			if(routine->trace==flagval)
				return(TR_ALREADY);
			routine->trace=flagval;
			return(TR_SUCCESS);
		}
	}
	return(TR_FAILURE);
}
