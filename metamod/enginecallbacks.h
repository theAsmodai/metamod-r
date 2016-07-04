// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// enginecallbacks.h - wrapper for <enginecallback.h>

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

#ifndef MM_ENGINECALLBACKS_H
#define MM_ENGINECALLBACKS_H

// This file is a wrapper around the SDK's enginecallback.h file. We need
// this because we use a different type for the global object g_engfuncs,
// which is still compatible with the enginefuncs_t that the SDK
// uses.
// This is only done for files that belong to Metamod, not other projects
// like plugins that use this file, or others that include it, too.
// Since we don't have a clean seperation of include files right now we
// "hack" our way around that by using a flag METAMOD_CORE which is set 
// when compiling Metamod proper.

#ifdef __METAMOD_BUILD__
#  include "meta_eiface.h"        // HL_enginefuncs_t

// Use a #define to bend the enginefuncs_t type to our HL_enginefuncs_t
// type instead as we now use that for the global object g_engfuncs.
#  define enginefuncs_t HL_enginefuncs_t
#endif /* METAMOD_CORE */

#include <enginecallback.h>		// ALERT, etc

#ifdef __METAMOD_BUILD__
#  undef enginefuncs_t
#endif /* METAMOD_CORE */

// Also, create some additional macros for engine callback functions, which
// weren't in SDK dlls/enginecallbacks.h but probably should have been.

#define GET_INFOKEYBUFFER	    	(*g_engfuncs.pfnGetInfoKeyBuffer)
#define INFOKEY_VALUE			    (*g_engfuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEYVALUE	    	(*g_engfuncs.pfnSetClientKeyValue)
#define REG_SVR_COMMAND	    		(*g_engfuncs.pfnAddServerCommand)
#define SERVER_PRINT			    (*g_engfuncs.pfnServerPrint)
#define SET_SERVER_KEYVALUE	    	(*g_engfuncs.pfnSetKeyValue)
#define QUERY_CLIENT_CVAR_VALUE	    (*g_engfuncs.pfnQueryClientCvarValue)
#define QUERY_CLIENT_CVAR_VALUE2	(*g_engfuncs.pfnQueryClientCvarValue2)


#endif /* MM_ENGINECALLBACKS_H */

