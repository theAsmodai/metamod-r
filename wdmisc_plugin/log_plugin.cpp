// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// log_plugin.cpp - generic/common logging routines for plugin codebases

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

#include <stdio.h>		// vsnprintf(), etc
#include <stdarg.h>		// va_start(), etc

#include <extdll.h>
#include <enginecallback.h>		// ALERT()

#include <meta_api.h>
#include <support_meta.h>

#include "log_plugin.h"
#include "plugin.h"

cvar_t *plugin_debug;

// Setup for logging.  We pass the name of the debug cvar so that this
// code can be used by multiple different plugins.
void log_init(const char *cvarname) {
	CVAR_REGISTER(&init_plugin_debug);
	// Since metamod replaces our cvar_t structure with a structure local
	// to metamod when we call CVAR_REGISTER, we have to get the address of
	// the new struct if we expect to be able to read/write directly to
	// the cvar struct.
	plugin_debug=CVAR_GET_POINTER(cvarname);
	// Since we haven't read server.cfg yet, allow debug logging (level 3)
	// to be enabled with the "+developer" cmdline option.
	if((int) CVAR_GET_FLOAT("developer") != 0)
		CVAR_SET_FLOAT(cvarname, 3);
}
