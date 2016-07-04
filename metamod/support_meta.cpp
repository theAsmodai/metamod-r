// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// support_meta.cpp - generic support routines

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <extdll.h>			// always

#include <metamod.h>		// GameDLL
#include "support_meta.h"	// me
#include "osdep.h"			// sleep, etc

META_ERRNO meta_errno;

void DLLINTERNAL do_exit(int exitval) {
	sleep(3);
	exit(exitval);
}

// Checks for a non-empty file, relative to the gamedir if necessary.
// Formerly used LOAD_FILE_FOR_ME, which provided a simple way to check for
// a file under the gamedir, but which would _also_ look in the sibling
// "valve" directory, thus sometimes finding files that weren't desired.
// Also, formerly named just "valid_file".
//
// Special-case-recognize "/dev/null" as a valid file.
int DLLINTERNAL valid_gamedir_file(const char *path) {
	char buf[PATH_MAX];
	struct stat st;
	int ret, reg, size;

	if(!path)
		return(FALSE);

	if(strmatch(path, "/dev/null"))
		return(TRUE);

	if(is_absolute_path(path))
		STRNCPY(buf, path, sizeof(buf));
	else
		safevoid_snprintf(buf, sizeof(buf), "%s/%s", GameDLL.gamedir, path);

	ret=stat(buf, &st);
	if(ret != 0) {
		META_DEBUG(5, ("Unable to stat '%s': %s", buf, strerror(errno)));
		return(FALSE);
	}

	reg=S_ISREG(st.st_mode);
	if(!reg) {
		META_DEBUG(5, ("Not a regular file: %s", buf));
		return(FALSE);
	}

	size=st.st_size;
	if(!size) {
		META_DEBUG(5, ("Empty file: %s", buf));
		return(FALSE);
	}

	if(ret==0 && reg && size)
		return(TRUE);
	else
		return(FALSE);
}

// Turns path into a full path:
//  - if not absolute, prepends gamedir
//  - calls realpath() to collapse ".." and such
//  - calls normalize_pathname() to fix backslashes, etc
//
// Much like realpath, buffer pointed to by fullpath is assumed to be 
// able to store a string of PATH_MAX length.
char * DLLINTERNAL full_gamedir_path(const char *path, char *fullpath) {
	char buf[PATH_MAX];

	// Build pathname from filename, plus gamedir if relative path.
	if(is_absolute_path(path))
		STRNCPY(buf, path, sizeof(buf));
	else
		safevoid_snprintf(buf, sizeof(buf), "%s/%s", GameDLL.gamedir, path);
	// Remove relative path components, if possible.
	if(!realpath(buf, fullpath)) {
		META_DEBUG(4, ("Unable to get realpath for '%s': %s", buf,
				str_os_error()));
		STRNCPY(fullpath, path, PATH_MAX);
	}
	// Replace backslashes, etc.
	normalize_pathname(fullpath);
	return(fullpath);
}
