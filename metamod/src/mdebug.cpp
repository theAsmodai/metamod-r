#include "precompiled.h"

void mdebug_to_file(const char* fmt, ...)
{
	char buf[1024];

	va_list argptr;
	va_start(argptr, fmt);
	Q_vsnprintf(buf, sizeof buf, fmt, argptr);
	va_end(argptr);

	FILE* fp = fopen("mdebug.log", "a");
	if (fp) {
		time_t td;
		time(&td);

		char date[32];
		strftime(date, 31, "%m/%d/%Y - %H:%M:%S", localtime(&td));

		fprintf(fp, "%s: %s", date, buf);
		fclose(fp);
	}
}
