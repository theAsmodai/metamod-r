// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// conf_meta.h - configfile reading

// Modeled after mutt/init.[ch].

/*
 * Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
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
 *    link the code of this program with the Half-Life Game g_engine ("HL
 *    g_engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL g_engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef CONF_META_H
#define CONF_META_H

#include "types_meta.h"		// mBOOL

// Max length of line in config file.
#define MAX_CONF_LEN	1024

// Supported config value-types.
typedef enum {
	CF_NONE=0,
	CF_INT,
	CF_BOOL,
	CF_STR,
	CF_PATH,
#if 0
	CF_CVAR,
	CF_CMD,
#endif
} cf_type_t;

typedef mBOOL (*SETOPT_FN) (char *key, char *value);

typedef struct option_s {
	const char *name;		// option name
	cf_type_t type;			// option type
	void *dest;				// addr of destination variable, or handler function
	const char *init;		// initial value, as a string, just as config file would
} option_t;

class MConfig {
	private:
		// data
		option_t *list;
		char *filename;
		// functions
		option_t *find(const char *lookup) const;
	static mBOOL set(option_t *setp, const char *value);
		// Private; to satisfy -Weffc++ "has pointer data members but does
		// not override" copy/assignment constructor.
		void operator=(const MConfig &src);
		MConfig(const MConfig &src);
	public:
		// contructor
		MConfig(void);
		// data
		int debuglevel;		// to use for meta_debug
		char *gamedll;		// string if specified in config.ini
		char *plugins_file;	// ie metamod.ini, plugins.ini
		char *exec_cfg;		// ie metaexec.cfg, exec.cfg
		// functions
		void init(option_t *global_options);
		mBOOL load(const char *filename);
		mBOOL set(const char *key, const char *value);
		void show(void) const;
};

#endif /* CONF_META_H */
