#ifndef SUPPORT_META_H
#define SUPPORT_META_H

#include <string.h>		// strcpy(), strncat()
#include <sys/types.h>	// stat
#include <sys/stat.h>	// stat

#include "osdep.h"		// strcasecmp, S_ISREG,
#include "enginecallbacks.h"		// LOAD_FILE_FOR_ME, etc

void do_exit(int exitval);

// Unlike snprintf(), strncpy() doesn't necessarily null-terminate the
// target.  It appears the former function reasonably considers the given
// size to be "max size of target string" (including the null-terminator),
// whereas strncpy() strangely considers the given size to be "total number
// of bytes to copy".  Note strncpy() _always_ writes n bytes, whereas
// snprintf() writes a _max_ of n bytes (incl the NULL).  If strncpy()
// needs to write extra bytes to reach n, it uses NULLs, so the target
// _can_ be null-terminated, but only if the source string would have fit
// anyway -  in which case why not just use strcpy() instead?
//
// Thus, it appears strncpy() is not only unsafe, it's also inefficient,
// and seemingly no better than plain strcpy() anyway.
//
// With this logic, strncpy() doesn't appear to be much of a "str" function
// at all, IMHO.
//
// Strncat works better, although it considers the given size to be "number
// of bytes to append", and doesn't include the null-terminator in that
// count.  Thus, we can use it for what we want to do, by setting the
// target to zero-length (NULL in first byte), and copying n-1 bytes
// (leaving room for the null-termiator).
//
// Why does it have to be soo haaard...

// Also note, some kind of wrapper is necessary to group the two
// statements into one, for use in situations like non-braced else
// statements.

// Technique 1: use "do..while":

int valid_gamedir_file(const char *path);
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


// Smallest of two
#define MIN(x, y) (((x)<(y))?(x):(y))


// Greatest of two
#define MAX(x, y) (((x)>(y))?(x):(y))


#endif /* SUPPORT_META_H */
