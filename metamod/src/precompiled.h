#pragma once

#include "version/appversion.h"

#if defined(linux) || defined(__APPLE__)
// enable extra routines in system header files, like dladdr
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
#include <signal.h>			// sigaction, etc
#else
#define PSAPI_VERSION 1
#endif

#include <vector>
#include <array>

#include "osconfig.h"
#include "jitasm.h"

#define CreateInterface mm_CreateInterface

#include <extdll.h>
#include <studio.h>
#include <studioapi.h>
#include <rehlds_api.h>

#include "osdep.h"
#include "api_info.h"
#include "commands_meta.h"
#include "metamod.h"
#include "log_meta.h"
#include "conf_meta.h"
#include "support_meta.h"
#include "dllapi.h"
#include "engine_api.h"
#include "game_support.h"
#include "meta_eiface.h"
#include "mreg.h"
#include "meta_api.h"
#include "mutil.h"
#include "reg_support.h"
#include "mlist.h"
#include "mplugin.h"
#include "plinfo.h"
#include "mplayer.h"
#include "sdk_util.h"
#include "enginecallbacks.h"
#include "utils.h"
#include "callback_jit.h"
#include "metamod_rehlds_api.h"

#undef CreateInterface
#include "linkent.h"
