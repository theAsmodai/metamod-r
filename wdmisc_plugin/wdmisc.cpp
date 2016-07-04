// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// plugin.cpp - generic plugin operation

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

#include <meta_api.h>		// 
#include <vdate.h>			// COMPILE_TIME,
#include <support_meta.h>	// STRNCPY,

#include "wdmisc.h"			// me
#include "plugin.h"			// generic routines
#include "log_plugin.h"		// MAX_LOGMSG_LEN


// Must provide at least one of these..
META_FUNCTIONS gMetaFunctionTable = {
	NULL,			// pfnGetEntityAPI				HL SDK; called before game DLL
	NULL,			// pfnGetEntityAPI_Post			META; called after game DLL
	GetEntityAPI2,	// pfnGetEntityAPI2				HL SDK2; called before game DLL
	NULL,			// pfnGetEntityAPI2_Post		META; called after game DLL
	NULL,			// pfnGetNewDLLFunctions		HL SDK2; called before game DLL
	NULL,			// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	NULL,			// pfnGetEngineFunctions_Post	META; called after HL engine
};


cvar_t init_plugin_debug = {"wd_debug", "0", FCVAR_EXTDLL, 0, NULL};
cvar_t bounce = {"bounce", "0", FCVAR_EXTDLL, 0, NULL};


// Meta_Query, any desired checks for valid use.
int plugin_query(void) {
	return(TRUE);
}

// Meta_Attach.  Plugin startup.  Register commands and cvars.
int plugin_attach(void) {
	log_init("wd_debug");

	bounce_init();
	do_my_getengfuncs();
	REG_SVR_COMMAND("msglist", &wd_msglist);
	REG_SVR_COMMAND("msgid", &wd_msgid);
	REG_SVR_COMMAND("testit", &wd_testit);

	return(TRUE);
}

// Meta_Detach.  Cleaning up.
int plugin_detach(void) {
	return(TRUE);
}


void bounce_init(void) {
	CVAR_REGISTER(&bounce);
}

void bounce_check(void) {
	if((int)CVAR_GET_FLOAT("bounce")==1) {
		LOG_MESSAGE(PLID, "Exiting server at end of map (bounce==1)");
		SERVER_COMMAND("quit\n");
	}
}

void do_my_getengfuncs(void) {
	memset(&meta_engfuncs, 0, sizeof(meta_engfuncs));
	meta_engfuncs.pfnAlertMessage = wd_AlertMessage;
}

// The below was done basically as a proof-of-concept for catching certain
// game-events that aren't otherwise triggered by the HL API. It works only
// by examining every log message that the server generates, and checking
// to see if it includes the string indicating the particular event in
// question (in this case "Planted_The_Bomb"). 
// 
// Although this works, it is really a rather poor solution to the problem,
// as it assumes a lot of string-compares, which can seriously degrade the
// performance of the server. I did try to take as many short-cuts as
// reasonable in the code, to decrease the amount of string-comparing going
// on, which makes the code a bit less readable than doing just a "strstr"
// on the log line. 

void wd_AlertMessage(ALERT_TYPE atype, char *szFmt, ...) {
	va_list ap;
	static char buf[MAX_LOGMSG_LEN];
	char *cp, *ep;
	int found=0;
	size_t len=0;

	if(atype != at_logged)
		RETURN_META(MRES_IGNORED);

	va_start(ap, szFmt);
	vsnprintf(buf, sizeof(buf), szFmt, ap);
	va_end(ap);

	// Team "...." triggered ...
	if((cp=buf) && (cp[0]=='T') 
			&& (cp=strchr(cp+1, '"')) 
			&& (cp=strchr(cp+1, '"'))
			&& !strncmp(cp+2, "triggered", 9))
	{
		cp+=2;
		found=1;
	}
	// World triggered ...
	else if((cp=buf) && (cp[0]=='W') 
			&& !strncmp(cp+6, "triggered", 9))
	{
		cp+=6;
		found=1;
	}
	// "player<444><44444><...>" triggered ...
	else if((cp=buf) && (cp[0]=='"') 
			&& (cp=strchr(cp+1, '"')) 
			&& !strncmp(cp+2, "triggered", 9))
	{
		cp+=2;
		found=1;
	}

	if(found) {
		// past 'triggered "'
		cp+=11;
		if((ep=strchr(cp, '"')))
			len=ep-cp-1;
	}

	if(len && !strncmp(cp, "Planted_The_Bomb", len)) {
		char name[128];
		char *beg, *end;
		beg=buf + 1;
		end=strchr(buf, '<') + 1;
		if(!beg || !end)
			RETURN_META(MRES_IGNORED);
		len=end-beg;
		if(len > sizeof(name))
			len=sizeof(name);
		STRNCPY(name, beg, len);
		CENTER_SAY(PLID, "%s set up us the bomb!", name);
	}
	RETURN_META(MRES_HANDLED);
}

void wd_msglist(void) {
	int i, size;
	const char *cp;
	LOG_CONSOLE(PLID, "registered user msgs:");
	for(i=0; i < MAX_REG_MSGS; i++) {
		cp=GET_USER_MSG_NAME(PLID, i, &size);
		if(cp)
			LOG_CONSOLE(PLID, "%d %s %d", i, cp, size);
	}
}

void wd_msgid(void) {
	const char *msgname;
	int id;
	msgname=CMD_ARGV(1);
	id=GET_USER_MSG_ID(PLID, msgname, NULL);
	if(id)
		LOG_CONSOLE(PLID, "%s = %d", msgname, id);
	else
		LOG_CONSOLE(PLID, "msg not found: %s", msgname);
}

void wd_testit(void) {
	cvar_t *cv;
	cv=CVAR_GET_POINTER("csguard_version");
	if(cv)
		LOG_CONSOLE(PLID, "csguard_version found!: %d", cv);
	else
		LOG_CONSOLE(PLID, "csguard_version not found!");
}
