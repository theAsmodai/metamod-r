#include "precompiled.h"

// Compatibility with legacy glibc
#if !defined(_WIN32)

// i386 versions of the `stat' interface
#define _STAT_VER_LINUX	3
#define _STAT_VER _STAT_VER_LINUX

struct stat;
struct stat64;

extern "C" {
	__asm__(".symver __xstat64,__xstat64@GLIBC_2.2");

	int __xstat64(int ver, const char *path, struct stat64 *stat_buf);
	int __wrap_stat64(const char *file, struct stat64 *buf)
	{
		return __xstat64(_STAT_VER, file, buf);
	}
}

#endif // #if !defined(_WIN32)
