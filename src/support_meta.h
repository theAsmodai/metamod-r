#ifndef SUPPORT_META_H
#define SUPPORT_META_H

#include <string.h>		// strcpy(), strncat()
#include <sys/types.h>	// stat
#include <sys/stat.h>	// stat

#include "osdep.h"		// strcasecmp, S_ISREG,
#include "enginecallbacks.h"		// LOAD_FILE_FOR_ME, etc

void DLLINTERNAL do_exit(int exitval);

//use pointer to avoid inlining of strcmp
inline int DLLINTERNAL mm_strcmp(const char *s1, const char *s2) {
#if 0
	int (*__strcmp)(const char*, const char*) = &Q_strcmp;
	return((*__strcmp)(s1, s2));
#else
	return(Q_strcmp(s1,s2));
#endif
}

//use pointer to avoid inlining of strncmp
inline int DLLINTERNAL mm_strncmp(const char *s1, const char *s2, size_t n) {
#if 0
	int (*__strncmp)(const char*, const char*, size_t) = &Q_strncmp;
	return((*__strncmp)(s1, s2, n));
#else
	return(Q_strncmp(s1,s2,n));
#endif
}

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
#if 0
#define STRNCPY(dst, src, size) \
	do { Q_strcpy(dst, "\0"); Q_strncat(dst, src, size-1); } while(0)
#endif

// Technique 2: use parens and commas:
#if 0
#define STRNCPY(dst, src, size) \
	(Q_strcpy(dst, "\0"), Q_strncat(dst, src, size-1))
#endif

// Technique 3: use inline
inline char * DLLINTERNAL STRNCPY(char *dst, const char *src, int size) {
	return(Q_strncat(&(*dst = 0), src, size-1));
}

// Renamed string functions to be clearer.
inline int DLLINTERNAL strmatch(const char *s1, const char *s2) {
	if(likely(s1) && likely(s2))
		return(!mm_strcmp(s1, s2));
	else
		return(0);
}
inline int DLLINTERNAL strnmatch(const char *s1, const char *s2, size_t n) {
	if(likely(s1) && likely(s2))
		return(!mm_strncmp(s1, s2, n));
	else
		return(0);
}
inline int DLLINTERNAL strcasematch(const char *s1, const char *s2) {
	if(likely(s1) && likely(s2))
		return(!strcasecmp(s1, s2));
	else
		return(0);
}
inline int DLLINTERNAL strncasematch(const char *s1, const char *s2, size_t n) {
	if(likely(s1) && likely(s2))
		return(!strncasecmp(s1, s2, n));
	else
		return(0);
}

inline int DLLINTERNAL old_valid_file(char *path) {
	char *cp;
	int len, ret;
	cp = (char *)LOAD_FILE_FOR_ME(path, &len);
	if(cp && len)
		ret=1;
	else
		ret=0;
	FREE_FILE(cp);
	return(ret);
}
int DLLINTERNAL valid_gamedir_file(const char *path);
char * DLLINTERNAL full_gamedir_path(const char *path, char *fullpath);

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
