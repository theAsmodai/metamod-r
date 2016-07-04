/*
 * Copyright (c) 2004-2006 Jussi Kivilinna
 *
 *    This file is part of "Metamod All-Mod-Support"-patch for Metamod.
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

#include <stddef.h>			// offsetof
#include <extdll.h>

#include "ret_type.h"
#include "types_meta.h"
#include "api_info.h"
#include "api_hook.h"
#include "mplugin.h"
#include "metamod.h"
#include "osdep.h"			//unlikely

// getting pointer with table index is faster than with if-else
static const void ** api_tables[3] = {
	(const void**)&Engine.funcs,
	(const void**)&GameDLL.funcs.dllapi_table,
	(const void**)&GameDLL.funcs.newapi_table
};

static const void ** api_info_tables[3] = {
	(const void**)&engine_info,
	(const void**)&dllapi_info,
	(const void**)&newapi_info
};

// Safety check for metamod-bot-plugin bugfix.
//  engine_api->pfnRunPlayerMove calls dllapi-functions before it returns.
//  This causes problems with bots running as metamod plugins, because
//  metamod assumed that PublicMetaGlobals is free to be used.
//  With call_count we can fix this by backuping up PublicMetaGlobals if 
//  it's already being used.
static unsigned int call_count = 0;

// get function pointer from api table by function pointer offset
inline void * DLLINTERNAL get_api_function(const void * api_table, unsigned int func_offset) {
	return(*(void**)((unsigned long)api_table + func_offset));
}

// get data pointer from api_info table by function offset
inline const api_info_t * DLLINTERNAL get_api_info(enum_api_t api, unsigned int api_info_offset) {
	return((const api_info_t *)((unsigned long)api_info_tables[api] + api_info_offset));
}

// simplified 'void' version of main hook function
void DLLINTERNAL main_hook_function_void(unsigned int api_info_offset, enum_api_t api, unsigned int func_offset, const void * packed_args) {
	const api_info_t *api_info;
	int i;
	META_RES mres, status, prev_mres;
	MPlugin *iplug;
	void *pfn_routine;
	int loglevel;
	const void *api_table;
	meta_globals_t backup_meta_globals[1];
	
	//passing offset from api wrapper function makes code faster/smaller
	api_info = get_api_info(api, api_info_offset);
	
	//Fix bug with metamod-bot-plugins.
	if(unlikely(call_count++>0)) {
		//Backup PublicMetaGlobals.
		backup_meta_globals[0] = PublicMetaGlobals;
	}
	
	//Setup
	loglevel=api_info->loglevel;
	mres=MRES_UNSET;
	status=MRES_UNSET;
	prev_mres=MRES_UNSET;
	pfn_routine=NULL;
	
	//Pre plugin functions
	prev_mres=MRES_UNSET;
	for(i=0; likely(i < Plugins->endlist); i++) {
		iplug=&Plugins->plist[i];
		
		if(unlikely(iplug->status != PL_RUNNING))
			continue;
		
		api_table = iplug->get_api_table(api);
		if(likely(!api_table)) {
			//plugin doesn't provide this api table
			continue;
		}
		
		pfn_routine=get_api_function(api_table, func_offset);
		if(likely(!pfn_routine)) {
			//plugin doesn't provide this function
			continue;
		}
		
		// initialize PublicMetaGlobals
		PublicMetaGlobals.mres = MRES_UNSET;
		PublicMetaGlobals.prev_mres = prev_mres;
		PublicMetaGlobals.status = status;
		
		// call plugin
		META_DEBUG(loglevel, ("Calling %s:%s()", iplug->file, api_info->name));
		api_info->api_caller(pfn_routine, packed_args);
		API_UNPAUSE_TSC_TRACKING();
		
		// plugin's result code
		mres=PublicMetaGlobals.mres;
		if(unlikely(mres > status))
			status = mres;
		
		// save this for successive plugins to see
		prev_mres = mres;
		
		if(unlikely(mres==MRES_UNSET))
			META_WARNING("Plugin didn't set meta_result: %s:%s()", iplug->file, api_info->name);
	}
	
	call_count--;
	
	//Api call
	if(likely(status!=MRES_SUPERCEDE)) {
		//get api table
		api_table = *api_tables[api];
		
		if(likely(api_table)) {
			pfn_routine = get_api_function(api_table, func_offset);
			if(likely(pfn_routine)) {
				META_DEBUG(loglevel, ("Calling %s:%s()", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
				api_info->api_caller(pfn_routine, packed_args);
				API_UNPAUSE_TSC_TRACKING();
			} else {
				// don't complain for NULL routines in NEW_DLL_FUNCTIONS
				if(unlikely(api != e_api_newapi))
					META_WARNING("Couldn't find api call: %s:%s", (api==e_api_engine)?"engine":GameDLL.file, api_info->name);
				status=MRES_UNSET;
			}
		} else {
			// don't complain for NULL NEW_DLL_FUNCTIONS-table
			if(unlikely(api != e_api_newapi))
				META_DEBUG(loglevel, ("No api table defined for api call: %s:%s", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
			status=MRES_UNSET;
		}
	} else
		META_DEBUG(loglevel, ("Skipped (supercede) %s:%s()", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
	
	call_count++;
	
	//Post plugin functions
	prev_mres=MRES_UNSET;
	for(i=0; likely(i < Plugins->endlist); i++) {
		iplug=&Plugins->plist[i];
		
		if(unlikely(iplug->status != PL_RUNNING))
			continue;
		
		api_table = iplug->get_api_post_table(api);
		if(likely(!api_table)) {
			//plugin doesn't provide this api table
			continue;
		}
		
		pfn_routine=get_api_function(api_table, func_offset);
		if(likely(!pfn_routine)) {
			//plugin doesn't provide this function
			continue;
		}
		
		// initialize PublicMetaGlobals
		PublicMetaGlobals.mres = MRES_UNSET;
		PublicMetaGlobals.prev_mres = prev_mres;
		PublicMetaGlobals.status = status;
		
		// call plugin
		META_DEBUG(loglevel, ("Calling %s:%s_Post()", iplug->file, api_info->name));
		api_info->api_caller(pfn_routine, packed_args);
		API_UNPAUSE_TSC_TRACKING();
		
		// plugin's result code
		mres=PublicMetaGlobals.mres;
		if(unlikely(mres > status))
			status = mres;
		
		// save this for successive plugins to see
		prev_mres = mres;
		
		if(unlikely(mres==MRES_UNSET))
			META_WARNING("Plugin didn't set meta_result: %s:%s_Post()", iplug->file, api_info->name);
		else if(unlikely(mres==MRES_SUPERCEDE))
			META_WARNING("MRES_SUPERCEDE not valid in Post functions: %s:%s_Post()", iplug->file, api_info->name);
	}

	if(unlikely(--call_count>0)) {
		//Restore backup
		PublicMetaGlobals = backup_meta_globals[0];
	}
}

// full return typed version of main hook function
void * DLLINTERNAL main_hook_function(const class_ret_t ret_init, unsigned int api_info_offset, enum_api_t api, unsigned int func_offset, const void * packed_args) {
	const api_info_t *api_info;
	int i;
	META_RES mres, status, prev_mres;
	MPlugin *iplug;
	void *pfn_routine;
	int loglevel;
	const void *api_table;
	meta_globals_t backup_meta_globals[1];
	
	//passing offset from api wrapper function makes code faster/smaller
	api_info = get_api_info(api, api_info_offset);
	
	//Fix bug with metamod-bot-plugins.
	if(unlikely(call_count++>0)) {
		//Backup PublicMetaGlobals.
		backup_meta_globals[0] = PublicMetaGlobals;
	}
	
	//Return class setup
	class_ret_t dllret=ret_init;
	class_ret_t override_ret=ret_init;
	class_ret_t pub_override_ret=ret_init;
	class_ret_t orig_ret=ret_init;
	class_ret_t pub_orig_ret=ret_init;
	
	//Setup
	loglevel=api_info->loglevel;
	mres=MRES_UNSET;
	status=MRES_UNSET;
	prev_mres=MRES_UNSET;
	pfn_routine=NULL;
	
	//Pre plugin functions
	prev_mres=MRES_UNSET;
	for(i=0; likely(i < Plugins->endlist); i++) {
		iplug=&Plugins->plist[i];
		
		if(unlikely(iplug->status != PL_RUNNING))
			continue;
		
		api_table = iplug->get_api_table(api);
		if(likely(!api_table)) {
			//plugin doesn't provide this api table
			continue;
		}
		
		pfn_routine=get_api_function(api_table, func_offset);
		if(likely(!pfn_routine)) {
			//plugin doesn't provide this function
			continue;
		}
		
		// initialize PublicMetaGlobals
		PublicMetaGlobals.mres = MRES_UNSET;
		PublicMetaGlobals.prev_mres = prev_mres;
		PublicMetaGlobals.status = status;
		pub_orig_ret = orig_ret;
		PublicMetaGlobals.orig_ret = pub_orig_ret.getptr();
		if(unlikely(status==MRES_SUPERCEDE)) {
			pub_override_ret = override_ret;
			PublicMetaGlobals.override_ret = pub_override_ret.getptr();
		}
		
		// call plugin
		META_DEBUG(loglevel, ("Calling %s:%s()", iplug->file, api_info->name));
		dllret = class_ret_t(api_info->api_caller(pfn_routine, packed_args));
		API_UNPAUSE_TSC_TRACKING();
		
		// plugin's result code
		mres=PublicMetaGlobals.mres;
		if(unlikely(mres > status))
			status = mres;
		
		// save this for successive plugins to see
		prev_mres = mres;
		
		if(unlikely(mres==MRES_SUPERCEDE)) {
			pub_override_ret = dllret;
			override_ret = dllret;
		} 
		else if(unlikely(mres==MRES_UNSET)) {
			META_WARNING("Plugin didn't set meta_result: %s:%s()", iplug->file, api_info->name);
		}
	}
	
	call_count--;
	
	//Api call
	if(likely(status!=MRES_SUPERCEDE)) {
		//get api table
		api_table = *api_tables[api];
		
		if(likely(api_table)) {
			pfn_routine = get_api_function(api_table, func_offset);
			if(likely(pfn_routine)) {
				META_DEBUG(loglevel, ("Calling %s:%s()", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
				dllret = class_ret_t(api_info->api_caller(pfn_routine, packed_args));
				API_UNPAUSE_TSC_TRACKING();
				orig_ret = dllret;
			} else {
				// don't complain for NULL routines in NEW_DLL_FUNCTIONS
				if(unlikely(api != e_api_newapi))
					META_WARNING("Couldn't find api call: %s:%s", (api==e_api_engine)?"engine":GameDLL.file, api_info->name);
				status=MRES_UNSET;
			}
		} else {
			// don't complain for NULL NEW_DLL_FUNCTIONS-table
			if(unlikely(api != e_api_newapi))
				META_DEBUG(loglevel, ("No api table defined for api call: %s:%s", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
			status=MRES_UNSET;
		}
	} else {
		META_DEBUG(loglevel, ("Skipped (supercede) %s:%s()", (api==e_api_engine)?"engine":GameDLL.file, api_info->name));
		orig_ret = override_ret;
		pub_orig_ret = override_ret;
		PublicMetaGlobals.orig_ret = pub_orig_ret.getptr();
	}
	
	call_count++;
	
	//Pre plugin functions
	prev_mres=MRES_UNSET;
	for(i=0; likely(i < Plugins->endlist); i++) {
		iplug=&Plugins->plist[i];
		
		if(unlikely(iplug->status != PL_RUNNING))
			continue;
		
		api_table = iplug->get_api_post_table(api);
		if(likely(!api_table)) {
			//plugin doesn't provide this api table
			continue;
		}
		
		pfn_routine=get_api_function(api_table, func_offset);
		if(likely(!pfn_routine)) {
			//plugin doesn't provide this function
			continue;
		}
		
		// initialize PublicMetaGlobals
		PublicMetaGlobals.mres = MRES_UNSET;
		PublicMetaGlobals.prev_mres = prev_mres;
		PublicMetaGlobals.status = status;
		pub_orig_ret = orig_ret;
		PublicMetaGlobals.orig_ret = pub_orig_ret.getptr();
		if(unlikely(status==MRES_OVERRIDE)) {
			pub_override_ret = override_ret;
			PublicMetaGlobals.override_ret = pub_override_ret.getptr();
		}
		
		// call plugin
		META_DEBUG(loglevel, ("Calling %s:%s_Post()", iplug->file, api_info->name));
		dllret = class_ret_t(api_info->api_caller(pfn_routine, packed_args));
		API_UNPAUSE_TSC_TRACKING();
		
		// plugin's result code
		mres=PublicMetaGlobals.mres;
		if(unlikely(mres > status))
			status = mres;
		
		// save this for successive plugins to see
		prev_mres = mres;
		
		if(unlikely(mres==MRES_OVERRIDE)) {
			pub_override_ret = dllret;
			override_ret = dllret;
		}
		else if(unlikely(mres==MRES_UNSET)) {
			META_WARNING("Plugin didn't set meta_result: %s:%s_Post()", iplug->file, api_info->name);
		}
		else if(unlikely(mres==MRES_SUPERCEDE)) {
			META_WARNING("MRES_SUPERCEDE not valid in Post functions: %s:%s_Post()", iplug->file, api_info->name);
		}
	}
	
	if(unlikely(--call_count>0)) {
		//Restore backup
		PublicMetaGlobals = backup_meta_globals[0];
	}
	
	//return value is passed through ret_init!
	if(likely(status!=MRES_OVERRIDE)) {
		return(*(void**)orig_ret.getptr());
	} else {
		META_DEBUG(loglevel, ("Returning (override) %s()", api_info->name));
		return(*(void**)override_ret.getptr());
	}
}

//
// Macros for creating api caller functions
//
#define BEGIN_API_CALLER_FUNC(ret_type, args_type_code) \
	void * DLLINTERNAL _COMBINE4(api_caller_, ret_type, _args_, args_type_code)(const void * func, const void * packed_args) { \
		_COMBINE2(pack_args_type_, args_type_code) * p ATTRIBUTE(unused)= (_COMBINE2(pack_args_type_, args_type_code) *)packed_args;
#define END_API_CALLER_FUNC(ret_t, args_t, args) \
		API_PAUSE_TSC_TRACKING(); \
		return(*(void **)class_ret_t((*(( ret_t (*) args_t )func)) args).getptr()); \
	}
#define END_API_CALLER_FUNC_void(args_t, args) \
		API_PAUSE_TSC_TRACKING(); \
		return((*(( void* (*) args_t )func)) args); \
	}

//
// API function callers.
//

//-
BEGIN_API_CALLER_FUNC(void, ipV)
END_API_CALLER_FUNC_void( (int, const void*, ...), (p->i1, p->p1, p->str) )

//-
BEGIN_API_CALLER_FUNC(void, 2pV)
END_API_CALLER_FUNC_void( (const void*, const void*, ...), (p->p1, p->p2, p->str) )

//-
BEGIN_API_CALLER_FUNC(void, void)
END_API_CALLER_FUNC_void( (void), () )

BEGIN_API_CALLER_FUNC(ptr, void)
END_API_CALLER_FUNC(void*, (void), () )

BEGIN_API_CALLER_FUNC(int, void)
END_API_CALLER_FUNC(int, (void), () )

BEGIN_API_CALLER_FUNC(float, void)
END_API_CALLER_FUNC(float, (void), () )

//-
BEGIN_API_CALLER_FUNC(float, 2f)
END_API_CALLER_FUNC( float, (float, float), (p->f1, p->f2) )

//-
BEGIN_API_CALLER_FUNC(void, 2i)
END_API_CALLER_FUNC_void( (int, int), (p->i1, p->i2) );

BEGIN_API_CALLER_FUNC(int, 2i)
END_API_CALLER_FUNC(int, (int, int), (p->i1, p->i2) );

//-
BEGIN_API_CALLER_FUNC(void, 2i2p)
END_API_CALLER_FUNC_void( (int, int, const void*, const void*), (p->i1, p->i2, p->p1, p->p2) );

//-
BEGIN_API_CALLER_FUNC(void, 2i2pi2p)
END_API_CALLER_FUNC_void( (int, int, const void*, const void*, int, const void*, const void*), (p->i1, p->i2, p->p1, p->p2, p->i3, p->p3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(void, 2p)
END_API_CALLER_FUNC_void( (const void*, const void*), (p->p1, p->p2) );

BEGIN_API_CALLER_FUNC(ptr, 2p)
END_API_CALLER_FUNC(void*, (const void*, const void*), (p->p1, p->p2) );

BEGIN_API_CALLER_FUNC(int, 2p)
END_API_CALLER_FUNC(int, (const void*, const void*), (p->p1, p->p2) );

//-
BEGIN_API_CALLER_FUNC(void, 2p2f)
END_API_CALLER_FUNC_void( (const void*, const void*, float, float), (p->p1, p->p2, p->f1, p->f2) );

//-
BEGIN_API_CALLER_FUNC(void, 2p2i2p)
END_API_CALLER_FUNC_void( (const void*, const void*, int, int, const void*, const void*), (p->p1, p->p2, p->i1, p->i2, p->p3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(void, 2p3fus2uc)
END_API_CALLER_FUNC_void( (const void*, const void*, float, float, float, unsigned short, unsigned char, unsigned char), (p->p1, p->p2, p->f1, p->f2, p->f3, p->us1, p->uc1, p->uc2) );

//-
BEGIN_API_CALLER_FUNC(ptr, 2pf)
END_API_CALLER_FUNC(void*, (const void*, const void*, float), (p->p1, p->p2, p->f1) );

//-
BEGIN_API_CALLER_FUNC(void, 2pfi)
END_API_CALLER_FUNC_void( (const void*, const void*, float, int), (p->p1, p->p2, p->f1, p->i1) );

//-
BEGIN_API_CALLER_FUNC(void, 2pi)
END_API_CALLER_FUNC_void( (const void*, const void*, int), (p->p1, p->p2, p->i1) );

BEGIN_API_CALLER_FUNC(int, 2pi)
END_API_CALLER_FUNC(int, (const void*, const void*, int), (p->p1, p->p2, p->i1) );

//-
BEGIN_API_CALLER_FUNC(void, 2pui)
END_API_CALLER_FUNC_void( (const void*, const void*, unsigned int), (p->p1, p->p2, p->ui1) );

//-
BEGIN_API_CALLER_FUNC(void, 2pi2p)
END_API_CALLER_FUNC_void( (const void*, const void*, int, const void*, const void*), (p->p1, p->p2, p->i1, p->p3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(void, 2pif2p)
END_API_CALLER_FUNC_void( (const void*, const void*, int, float, const void*, const void*), (p->p1, p->p2, p->i1, p->f1, p->p3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(int, 3i)
END_API_CALLER_FUNC(int, (int, int, int), (p->i1, p->i2, p->i3) );

//-
BEGIN_API_CALLER_FUNC(void, 3p)
END_API_CALLER_FUNC_void( (const void*, const void*, const void*), (p->p1, p->p2, p->p3) );

BEGIN_API_CALLER_FUNC(ptr, 3p)
END_API_CALLER_FUNC(void*, (const void*, const void*, const void*), (p->p1, p->p2, p->p3) );

BEGIN_API_CALLER_FUNC(int, 3p)
END_API_CALLER_FUNC(int, (const void*, const void*, const void*), (p->p1, p->p2, p->p3) );

//-
BEGIN_API_CALLER_FUNC(void, 3p2f2i)
END_API_CALLER_FUNC_void( (const void*, const void*, const void*, float, float, int, int), (p->p1, p->p2, p->p3, p->f1, p->f2, p->i1, p->i2) );

//-
BEGIN_API_CALLER_FUNC(int, 3pi2p)
END_API_CALLER_FUNC(int, (const void*, const void*, const void*, int, const void*, const void*), (p->p1, p->p2, p->p3, p->i1, p->p4, p->p5) );

//-
BEGIN_API_CALLER_FUNC(void, 4p)
END_API_CALLER_FUNC_void( (const void*, const void*, const void*, const void*), (p->p1, p->p2, p->p3, p->p4) );

BEGIN_API_CALLER_FUNC(int, 4p)
END_API_CALLER_FUNC(int, (const void*, const void*, const void*, const void*), (p->p1, p->p2, p->p3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(void, 4pi)
END_API_CALLER_FUNC_void( (const void*, const void*, const void*, const void*, int), (p->p1, p->p2, p->p3, p->p4, p->i1) );

BEGIN_API_CALLER_FUNC(int, 4pi)
END_API_CALLER_FUNC(int, (const void*, const void*, const void*, const void*, int), (p->p1, p->p2, p->p3, p->p4, p->i1) );

//-
BEGIN_API_CALLER_FUNC(void, f)
END_API_CALLER_FUNC_void( (float), (p->f1) );

//-
BEGIN_API_CALLER_FUNC(void, i)
END_API_CALLER_FUNC_void( (int), (p->i1) );

BEGIN_API_CALLER_FUNC(int, i)
END_API_CALLER_FUNC(int, (int), (p->i1) );

BEGIN_API_CALLER_FUNC(ptr, i)
END_API_CALLER_FUNC(void*, (int), (p->i1) );

BEGIN_API_CALLER_FUNC(uint, ui)
END_API_CALLER_FUNC(unsigned int, (unsigned int), (p->ui1) );

BEGIN_API_CALLER_FUNC(ptr, ui)
END_API_CALLER_FUNC(void*, (unsigned int), (p->ui1) );

//-
BEGIN_API_CALLER_FUNC(ulong, ul)
END_API_CALLER_FUNC(unsigned long, (unsigned long), (p->ul1) );

//-
BEGIN_API_CALLER_FUNC(void, i2p)
END_API_CALLER_FUNC_void( (int, const void*, const void*), (p->i1, p->p1, p->p2) );

BEGIN_API_CALLER_FUNC(int, i2p)
END_API_CALLER_FUNC(int, (int, const void*, const void*), (p->i1, p->p1, p->p2) );

//-
BEGIN_API_CALLER_FUNC(void, i3p)
END_API_CALLER_FUNC_void( (int, const void*, const void*, const void*), (p->i1, p->p1, p->p2, p->p3) );

//-
BEGIN_API_CALLER_FUNC(void, ip)
END_API_CALLER_FUNC_void( (int, const void*), (p->i1, p->p1) );

BEGIN_API_CALLER_FUNC(ushort, ip)
END_API_CALLER_FUNC( unsigned short, (int, const void*), (p->i1, p->p1) );

BEGIN_API_CALLER_FUNC(int, ip)
END_API_CALLER_FUNC( int, (int, const void*), (p->i1, p->p1) );

//-
BEGIN_API_CALLER_FUNC(void, ipusf2p2f4i)
END_API_CALLER_FUNC_void( (int, const void*, unsigned short, float, const void*, const void*, float, float, int, int, int, int), (p->i1, p->p1, p->us1, p->f1, p->p2, p->p3, p->f2, p->f3, p->i2, p->i3, p->i4, p->i5) );

//-
BEGIN_API_CALLER_FUNC(void, p)
END_API_CALLER_FUNC_void( (const void*), (p->p1) );

BEGIN_API_CALLER_FUNC(ptr, p)
END_API_CALLER_FUNC(void*, (const void*), (p->p1) );

BEGIN_API_CALLER_FUNC(char, p)
END_API_CALLER_FUNC(char, (const void*), (p->p1) );

BEGIN_API_CALLER_FUNC(int, p)
END_API_CALLER_FUNC(int, (const void*), (p->p1) );

BEGIN_API_CALLER_FUNC(uint, p)
END_API_CALLER_FUNC(unsigned int, (const void*), (p->p1) );

BEGIN_API_CALLER_FUNC(float, p)
END_API_CALLER_FUNC(float, (const void*), (p->p1) );

//-
BEGIN_API_CALLER_FUNC(void, p2f)
END_API_CALLER_FUNC_void( (const void*, float, float), (p->p1, p->f1, p->f2) );

//-
BEGIN_API_CALLER_FUNC(int, p2fi)
END_API_CALLER_FUNC(int, (const void*, float, float, int), (p->p1, p->f1, p->f2, p->i1) );

//-
BEGIN_API_CALLER_FUNC(void, p2i)
END_API_CALLER_FUNC_void( (const void*, int, int), (p->p1, p->i1, p->i2) );

//-
BEGIN_API_CALLER_FUNC(void, p3i)
END_API_CALLER_FUNC_void( (const void*, int, int, int), (p->p1, p->i1, p->i2, p->i3) );

//-
BEGIN_API_CALLER_FUNC(void, p4i)
END_API_CALLER_FUNC_void( (const void*, int, int, int, int), (p->p1, p->i1, p->i2, p->i3, p->i4) );

//-
BEGIN_API_CALLER_FUNC(void, puc)
END_API_CALLER_FUNC_void( (const void*, unsigned char), (p->p1, p->uc1) );

//-
BEGIN_API_CALLER_FUNC(void, pf)
END_API_CALLER_FUNC_void( (const void*, float), (p->p1, p->f1) );

//-
BEGIN_API_CALLER_FUNC(void, pfp)
END_API_CALLER_FUNC_void( (const void*, float, const void*), (p->p1, p->f1, p->p2) );

//-
BEGIN_API_CALLER_FUNC(void, pi)
END_API_CALLER_FUNC_void( (const void*, int), (p->p1, p->i1) );

BEGIN_API_CALLER_FUNC(ptr, pi)
END_API_CALLER_FUNC(void*, (const void*, int), (p->p1, p->i1) );

BEGIN_API_CALLER_FUNC(int, pi)
END_API_CALLER_FUNC(int, (const void*, int), (p->p1, p->i1) );

//-
BEGIN_API_CALLER_FUNC(void, pi2p)
END_API_CALLER_FUNC_void( (const void*, int, const void*, const void*), (p->p1, p->i1, p->p2, p->p3) );

//-
BEGIN_API_CALLER_FUNC(int, pi2p2ip)
END_API_CALLER_FUNC(int, (const void*, int, const void*, const void*, int, int, const void*), (p->p1, p->i1, p->p2, p->p3, p->i2, p->i3, p->p4) );

//-
BEGIN_API_CALLER_FUNC(void, pip)
END_API_CALLER_FUNC_void( (const void*, int, const void*), (p->p1, p->i1, p->p2) );

BEGIN_API_CALLER_FUNC(ptr, pip)
END_API_CALLER_FUNC(void*, (const void*, int, const void*), (p->p1, p->i1, p->p2) );

//-
BEGIN_API_CALLER_FUNC(void, pip2f2i)
END_API_CALLER_FUNC_void( (const void*, int, const void*, float, float, int, int), (p->p1, p->i1, p->p2, p->f1, p->f2, p->i2, p->i3) );

//-
BEGIN_API_CALLER_FUNC(void, pip2f4i2p)
END_API_CALLER_FUNC_void( (const void*, int, const void*, float, float, int, int, int, int, const void*, const void*), (p->p1, p->i1, p->p2, p->f1, p->f2, p->i2, p->i3, p->i4, p->i5, p->p3, p->p4) );
