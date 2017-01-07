#pragma once

#include "enginecallbacks.h"		// ALERT, etc

// max buffer size for printed messages
#define MAX_LOGMSG_LEN	1024

// max buffer size for client messages
#define MAX_CLIENTMSG_LEN 128

extern cvar_t meta_debug;

template<typename ...t_args>
void META_DEBUG(int level, t_args... args)
{
	if (meta_debug.value < level)
		return;
	ALERT(at_logged, "[META] (debug:%i) %s\n", level, args...);
}

void META_CONS(const char *fmt, ...);
void META_DEV(const char *fmt, ...);
void META_INFO(const char *fmt, ...);
void META_WARNING(const char *fmt, ...);
void META_ERROR(const char *fmt, ...);
void META_LOG(const char *fmt, ...);
void META_CLIENT(edict_t *pEntity, const char *fmt, ...);

void flush_ALERT_buffer(void);
