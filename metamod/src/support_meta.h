#pragma once

#include <sys/types.h>		// stat
#include <sys/stat.h>		// stat

#include "osdep.h"		// _stricmp, S_ISREG,

void __declspec(noreturn) do_exit(int exitval);

bool FileExistsInGameDir(const char *path);
char *full_gamedir_path(const char *path, char *fullpath);

// Turn a variable/function name into the corresponding string, optionally
// stripping off the leading "len" characters.  Useful for things like
// turning 'pfnClientCommand' into "ClientCommand" so we don't have to
// specify strings used for all the debugging/log messages.
#define STRINGIZE(name, len)		#name+len

// Max description length for plugins.ini and other places.
#define MAX_DESC_LEN 256

// For various character string buffers.
#define MAX_STRBUF_LEN 1024
