// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// vdate.cpp - compile-time version date

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

#include "info_name.h"		// for VNAME, VVERSION, etc
#include "vdate.h"


// Grab date/time of compile.  The Makefile is set up to recompile this
// module before each link, so that this will always indicate the time the
// library was compiled and linked.

// This is in a separate file from vers_*, so it can be generically used by
// multiple projects.

char const *COMPILE_TIME=__DATE__ ", " __TIME__;

#ifndef COMPILE_TZ
	#define COMPILE_TZ "EET"
#endif

char const *COMPILE_TZONE = COMPILE_TZ;

// Include a string for /usr/bin/ident.

char const *vstring="\n$Pg: " VNAME " -- " VVERSION " | " __DATE__ " - " __TIME__ " $\n";
