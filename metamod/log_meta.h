// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// log_meta.h - functions & macros for logging

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

#ifndef LOG_META_H
#define LOG_META_H

#include "comp_dep.h"
#include "osdep.h"	//unlikely, OPEN_ARGS

// Debug logging.  
//
// This is done as a macro, rather than a function.  This way, you can add
// DEBUG statements all over, without worrying about performance
// implications.  If the debugging level is set low, all those statements
// will only generate a simple float/int compare each; if we were to use a
// function instead of a macro, it would end up wasting a lot of cpu cycles
// calling/returning from the function every time.  With a fair number of
// DEBUG statements, or if they're placed in frequently excuted code, the
// overhead of the wasted function calls could significantly impact server
// performance.
//
// For this reason, we also compare directly to the float value of the
// cvar, rather than calling CVAR_GET_FLOAT() and thus generating a string
// compare for each DEBUG statement.
//
// Called as: 
//    META_DEBUG(3, ("return code: %d", ret)); 
//
// Note the double parens, and the missing parens around "args" in the
// macro itself.  Note also the "do..while(0)" loop wrapping the
// statements, so they become a single statement when expanded, necessary
// for times when it might be called as a single-statement result of an
// else (or other flow control).
//
// Yes, it's all a bit of a hack.
//
// Using meta_debug_value instead of meta_debug.value.
// meta_debug_value is preconverted int-value of meta_debug.value.
// Reason for this optimization: Integer compare is much faster than float compare.
// i686 has fast float compare, but since we want to have i386 binary, we use this.

#ifdef __BUILD_FAST_METAMOD__
	#define META_DEBUG(level, args) do { break; } while(0)
#else
	#define META_DEBUG(level, args) \
		do { \
			if(unlikely(meta_debug_value >= level)) { \
				META_DEBUG_SET_LEVEL(level); \
				META_DO_DEBUG args; \
			} \
		} while(0)
#endif

// max buffer size for printed messages
#define MAX_LOGMSG_LEN	1024

// max buffer size for client messages
#define MAX_CLIENTMSG_LEN 128

extern cvar_t meta_debug DLLHIDDEN;
extern int meta_debug_value DLLHIDDEN;

// META_DEV provides debug logging via the cvar "developer" (when set to 1)
// and uses a function call rather than a macro as it's really intended to
// be used only during startup, before meta_debug has been set from reading
// server.cfg.
// NOTE: META_DEV has now been mostly obsoleted in the code.

void DLLINTERNAL META_CONS(const char *fmt, ...);
void DLLINTERNAL META_DEV(const char *fmt, ...);
void DLLINTERNAL META_INFO(const char *fmt, ...);
void DLLINTERNAL META_WARNING(const char *fmt, ...);
void DLLINTERNAL META_ERROR(const char *fmt, ...);
void DLLINTERNAL META_LOG(const char *fmt, ...);
void DLLINTERNAL META_CLIENT(edict_t *pEntity, const char *fmt, ...);
#ifndef __BUILD_FAST_METAMOD__
	void DLLINTERNAL META_DEBUG_SET_LEVEL(int level);
	void DLLINTERNAL META_DO_DEBUG(const char *fmt, ...);
#endif

void DLLINTERNAL flush_ALERT_buffer(void);

#endif /* LOG_META_H */
