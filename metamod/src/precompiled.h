#pragma once

#include "version/appversion.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>			// for strncpy(), etc

#include <extdll.h>
#include <eiface.h>

#include "osconfig.h"
#include "h_export.h"

#include "osdep.h"			// win32 vsnprintf, etc
#include "sdk_util.h"

#include "eiface.h"			// engfuncs_t, globalvars_t
#include "meta_api.h"			// meta_globals_t, etc

#include "linkent.h"	// LINK_ENTITY_TO_PLUGIN

#include "ret_type.h"
#include "types_meta.h"
#include "api_info.h"
#include "api_hook.h"
#include "mplugin.h"
#include "metamod.h"

#include "commands_meta.h"	// me
#include "log_meta.h"		// META_CONS, etc

#include "conf_meta.h"		// me
#include "support_meta.h"	// strmatch


// #include <malloc.h>			// alloca, etc	??????????


	// Don't include winspool.h; clashes with SERVER_EXECUTE from engine
//	#define _WINSPOOL_H	
//	#include <windows.h>
//	#include <winnt.h>     // Header structures


#include "osdep_p.h"				// is_gamedll, ...
#include "game_support.h"			// lookup_game, etc
#include "reg_support.h"		// meta_AddServerCommand, etc
#include "mm_pextensions.h"









