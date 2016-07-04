#include "info_name.h"		// for VNAME, VVERSION, etc
#include "vdate.h"


// Grab date/time of compile.  The Makefile is set up to recompile this
// module before each link, so that this will always indicate the time the
// library was compiled and linked.

// This is in a separate file from vers_*, so it can be generically used by
// multiple projects.

char const *COMPILE_TIME=__DATE__ ", " __TIME__;

#ifndef COMPILE_TZ
	#define COMPILE_TZ "EET"
#endif

char const *COMPILE_TZONE = COMPILE_TZ;

// Include a string for /usr/bin/ident.

char const *vstring="\n$Pg: " VNAME " -- " VVERSION " | " __DATE__ " - " __TIME__ " $\n";
