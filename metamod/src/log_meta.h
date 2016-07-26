#pragma once

#include "enginecallbacks.h"		// ALERT, etc
#include "sdk_util.h"			// UTIL_VarArgs, etc

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
// As suggested by Jussi Kivilinna: Use "if (meta_debug.value < level); else
// DO(something);" style because "meta_debug.value < level" is in most common
// case "false". Check disasm, contitional jumps are predicted not to be
// taken by CPU.
//
// Yes, it's all a bit of a hack.

#define META_DEBUG(level, args) \
	do { if (meta_debug.value < level) break; else ALERT(at_logged, "[META] (debug:%d) %s\n", level, UTIL_VarArgs args ); } while(0)

// max buffer size for printed messages
#define MAX_LOGMSG_LEN	1024

// max buffer size for client messages
#define MAX_CLIENTMSG_LEN 128

extern cvar_t meta_debug;

// META_DEV provides debug logging via the cvar "developer" (when set to 1)
// and uses a function call rather than a macro as it's really intended to
// be used only during startup, before meta_debug has been set from reading
// server.cfg.
// NOTE: META_DEV has now been mostly obsoleted in the code.

void META_CONS(const char *fmt, ...);
void META_DEV(const char *fmt, ...);
void META_INFO(const char *fmt, ...);
void META_WARNING(const char *fmt, ...);
void META_ERROR(const char *fmt, ...);
void META_LOG(const char *fmt, ...);
void META_CLIENT(edict_t *pEntity, const char *fmt, ...);

void flush_ALERT_buffer(void);
