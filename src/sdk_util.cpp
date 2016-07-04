#include <extdll.h>
#include "sdk_util.h"
#include <string.h>			// for strncpy(), etc
#include "osdep.h"			// win32 vsnprintf, etc

const char * DLLINTERNAL META_UTIL_VarArgs(const char *format, ...)
{
	va_list argptr;
	static char string[4096];
	va_start(argptr, format);
	safevoid_vsnprintf(string, sizeof(string), format, argptr);
	va_end(argptr);
	return(string);
}

short DLLINTERNAL FixedSigned16(float value, float scale)
{
	int output;
	output = (int)(value * scale);
	if(output > 32767)
		output = 32767;
	if(output < -32768)
		output = -32768;
	return((short)output);
}

unsigned short DLLINTERNAL FixedUnsigned16(float value, float scale)
{
	int output;
	output = (int)(value * scale);
	if(output < 0)
		output = 0;
	if(output > 0xFFFF)
		output = 0xFFFF;
	return((unsigned short)output);
}


void DLLINTERNAL META_UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, const char *pMessage)
{
	if (fast_FNullEnt(pEntity) || pEntity->free)
	{
		return;
	}
	MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, pEntity );
	WRITE_BYTE( TE_TEXTMESSAGE );
	WRITE_BYTE( textparms.channel & 0xFF );
	WRITE_SHORT( FixedSigned16( textparms.x, 1<<13 ) );
	WRITE_SHORT( FixedSigned16( textparms.y, 1<<13 ) );
	WRITE_BYTE( textparms.effect );
	WRITE_BYTE( textparms.r1 );
	WRITE_BYTE( textparms.g1 );
	WRITE_BYTE( textparms.b1 );
	WRITE_BYTE( textparms.a1 );
	WRITE_BYTE( textparms.r2 );
	WRITE_BYTE( textparms.g2 );
	WRITE_BYTE( textparms.b2 );
	WRITE_BYTE( textparms.a2 );
	WRITE_SHORT( FixedUnsigned16( textparms.fadeinTime, 1<<8 ) );
	WRITE_SHORT( FixedUnsigned16( textparms.fadeoutTime, 1<<8 ) );
	WRITE_SHORT( FixedUnsigned16( textparms.holdTime, 1<<8 ) );
	if (textparms.effect == 2)
	{
		WRITE_SHORT(FixedUnsigned16(textparms.fxTime, 1 << 8));
	}
	if(Q_strlen( pMessage ) < 512)
	{
		WRITE_STRING( pMessage );
	} else {
		char tmp[512];
		Q_strncpy( tmp, pMessage, 511 );
		tmp[511] = 0;
		WRITE_STRING( tmp );
	}
	MESSAGE_END();
}
