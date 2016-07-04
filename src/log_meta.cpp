#include <stdio.h>		// vsnprintf, etc
#include <stdarg.h>		// va_start, etc
#include <extdll.h>				// always
#include "enginecallbacks.h"		// ALERT, etc
#include "sdk_util.h"			// SERVER_PRINT, etc
#include "log_meta.h"			// me
#include "osdep.h"				// win32 vsnprintf, etc
#include "support_meta.h"		// MAX

cvar_t meta_debug = {"meta_debug", "0", FCVAR_EXTDLL, 0, NULL};

int meta_debug_value = 0; //meta_debug_value is converted from float(meta_debug.value) to int on every frame

enum MLOG_SERVICE {
	mlsCONS = 1,
	mlsDEV,
	mlsIWEL,
	mlsCLIENT
};

static void buffered_ALERT(MLOG_SERVICE service, ALERT_TYPE atype, const char *prefix, const char *fmt, va_list ap);

// Print to console.
void DLLINTERNAL META_CONS(const char *fmt, ...) {
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	unsigned int len;
	va_start(ap, fmt);
	safevoid_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	len=Q_strlen(buf);
	if(len < sizeof(buf)-2) {	// -1 null, -1 for newline
		buf[len+0] = '\n';
		buf[len+1] = 0;
	}
	else
		buf[len-1] = '\n';
	SERVER_PRINT(buf);
}

// Log developer-level messages (obsoleted).
static const char *const prefixDEV = "[META] dev:";
void DLLINTERNAL META_DEV(const char *fmt, ...) {
	va_list ap;
	int dev;
	if(NULL != g_engfuncs.pfnCVarGetFloat) {
		dev=(int) CVAR_GET_FLOAT("developer");
		if(dev==0) return;
	}
	va_start(ap, fmt);
	buffered_ALERT(mlsDEV, at_logged, prefixDEV, fmt, ap);
	va_end(ap);
}

// Log infos.
static const char *const prefixINFO = "[META] INFO:";
void DLLINTERNAL META_INFO(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, prefixINFO, fmt, ap);
	va_end(ap);
}

// Log warnings.
static const char *const prefixWARNING = "[META] WARNING:";
void DLLINTERNAL META_WARNING(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, prefixWARNING, fmt, ap);
	va_end(ap);
}

// Log errors.
static const char *const prefixERROR = "[META] ERROR:";
void DLLINTERNAL META_ERROR(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, prefixERROR, fmt, ap);
	va_end(ap);
}

// Normal log messages.
static const char *const prefixLOG = "[META]";
void DLLINTERNAL META_LOG(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, prefixLOG, fmt, ap);
	va_end(ap);
}

// Print to client.
void DLLINTERNAL META_CLIENT(edict_t *pEntity, const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_CLIENTMSG_LEN];
	unsigned int len;
	va_start(ap, fmt);
	safevoid_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	len=Q_strlen(buf);
	if(len < sizeof(buf)-2)	{	// -1 null, -1 for newline
		buf[len+0] = '\n';
		buf[len+1] = 0;
	}
	else
		buf[len-1] = '\n';
	CLIENT_PRINTF(pEntity, print_console, buf);
}

#ifndef __BUILD_FAST_METAMOD__

static int debug_level;

void DLLINTERNAL META_DEBUG_SET_LEVEL(int level)
{
	debug_level = level;
}

void DLLINTERNAL META_DO_DEBUG(const char *fmt, ...)
{
	char meta_debug_str[1024];
	va_list ap;
	va_start(ap, fmt);
	safevoid_vsnprintf(meta_debug_str, sizeof(meta_debug_str), fmt, ap);
	va_end(ap);
	ALERT(at_logged, "[META] (debug:%d) %s\n", debug_level, meta_debug_str);
}

#endif /*!__BUILD_FAST_METAMOD__*/

class BufferedMessage : public class_metamod_new {
public:
	MLOG_SERVICE service;
	ALERT_TYPE atype;
	const char *prefix;
	char buf[MAX_LOGMSG_LEN];
	BufferedMessage *next;
};

static BufferedMessage *messageQueueStart = NULL;
static BufferedMessage *messageQueueEnd   = NULL;

static void buffered_ALERT(MLOG_SERVICE service, ALERT_TYPE atype, const char *prefix, const char *fmt, va_list ap) {
	char buf[MAX_LOGMSG_LEN];
	BufferedMessage *msg;
	if (NULL != g_engfuncs.pfnAlertMessage)
	{
		vsnprintf(buf, sizeof(buf), fmt, ap);
		ALERT(atype, "%s %s\n", prefix, buf);
		return;
	}
	// Engine AlertMessage function not available. Buffer message.
	msg = new BufferedMessage;
	if (NULL == msg)
	{
		// though luck, gonna lose this message
		return;
	}
	msg->service = service;
	msg->atype = atype;
	msg->prefix = prefix;
	vsnprintf(msg->buf, sizeof(buf), fmt, ap);
	msg->next = NULL;
	if (NULL == messageQueueEnd)
	{
		messageQueueStart = messageQueueEnd = msg;
	} else {
		messageQueueEnd->next = msg;
		messageQueueEnd = msg;
	}	
} 

// Flushes the message queue, printing messages to the respective
// service. This function doesn't check anymore if the g_engfuncs
// jumptable is set. Don't call it if it isn't set.
void DLLINTERNAL flush_ALERT_buffer(void)
{
	BufferedMessage *msg = messageQueueStart;
	int dev = (int) CVAR_GET_FLOAT("developer");
	while (NULL != msg)
	{
		if(msg->service == mlsDEV && dev==0)
		{
			;
		} else {
			ALERT(msg->atype, "b>%s %s\n", msg->prefix, msg->buf);
		}
		messageQueueStart = messageQueueStart->next;
		delete msg;
		msg = messageQueueStart;
	}
	messageQueueStart = messageQueueEnd = NULL;
}
