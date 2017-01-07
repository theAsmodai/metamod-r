#include "precompiled.h"

cvar_t meta_debug = { "meta_debug", "0", FCVAR_EXTDLL , 0, nullptr };

enum MLOG_SERVICE
{
	mlsCONS = 1,
	mlsDEV,
	mlsIWEL,
	mlsCLIENT
};

static void buffered_ALERT(MLOG_SERVICE service, ALERT_TYPE atype, const char *prefix, const char *fmt, va_list ap);

// Print to console.
void META_CONS(const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	unsigned int len;

	va_start(ap, fmt);
	len = Q_vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);

	buf[len] = '\n';
	buf[len + 1] = '\0';

	SERVER_PRINT(buf);
}

void META_DEV(const char *fmt, ...)
{
	if (CVAR_GET_FLOAT && CVAR_GET_FLOAT("developer"))
	{
		va_list ap;
		va_start(ap, fmt);
		buffered_ALERT(mlsDEV, at_logged, "[META] dev:", fmt, ap);
		va_end(ap);
	}
}

void META_INFO(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, "[META] INFO:", fmt, ap);
	va_end(ap);
}

void META_WARNING(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, "[META] WARNING:", fmt, ap);
	va_end(ap);
}

void META_ERROR(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, "[META] ERROR:", fmt, ap);
	va_end(ap);
}

void META_LOG(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	buffered_ALERT(mlsIWEL, at_logged, "[META]", fmt, ap);
	va_end(ap);
}

// Print to client.
void META_CLIENT(edict_t *pEntity, const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_CLIENTMSG_LEN];
	unsigned int len;

	va_start(ap, fmt);
	len = Q_vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);

	buf[len] = '\n';
	buf[len + 1] = '\0';

	CLIENT_PRINTF(pEntity, print_console, buf);
}

struct BufferedMessage
{
	MLOG_SERVICE service;
	ALERT_TYPE atype;
	const char *prefix;
	char buf[MAX_LOGMSG_LEN];
	BufferedMessage *next;
};

static BufferedMessage *messageQueueStart = nullptr;
static BufferedMessage *messageQueueEnd = nullptr;

void buffered_ALERT(MLOG_SERVICE service, ALERT_TYPE atype, const char *prefix, const char *fmt, va_list ap)
{
	char buf[MAX_LOGMSG_LEN];

	if (g_engfuncs.pfnAlertMessage)
	{
		Q_vsnprintf(buf, sizeof(buf), fmt, ap);
		ALERT(atype, "%s %s\n", prefix, buf);
		return;
	}

	// g_engine AlertMessage function not available. Buffer message.
	BufferedMessage *msg = new BufferedMessage;
	if (!msg)
	{
		// though luck, gonna lose this message
		return;
	}

	msg->service = service;
	msg->atype = atype;
	msg->prefix = prefix;
	Q_vsnprintf(msg->buf, sizeof(buf), fmt, ap);
	msg->next = nullptr;

	if (!messageQueueEnd)
	{
		messageQueueStart = messageQueueEnd = msg;
	}
	else
	{
		messageQueueEnd->next = msg;
		messageQueueEnd = msg;
	}
}


// Flushes the message queue, printing messages to the respective
// service. This function doesn't check anymore if the g_engfuncs
// jumptable is set. Don't call it if it isn't set.
void flush_ALERT_buffer(void)
{
	BufferedMessage *msg = messageQueueStart;
	int dev = (int)CVAR_GET_FLOAT("developer");

	while (msg)
	{
		if (msg->service == mlsDEV && dev == 0) {
			;
		}
		else
		{
			ALERT(msg->atype, "b>%s %s\n", msg->prefix, msg->buf);
		}

		messageQueueStart = messageQueueStart->next;
		delete msg;
		msg = messageQueueStart;
	}

	messageQueueStart = messageQueueEnd = nullptr;
}
