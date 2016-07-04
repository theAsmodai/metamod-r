// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mreg.h - description of registered items (classes MRegCmd, MRegCmdList)

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

#ifndef MREG_H
#define MREG_H

#include "types_meta.h"		// mBOOL
#include "comp_dep.h"		//
#include "new_baseclass.h"

// Number of entries to add to reglists when they need to grow.  Typically
// more cvars than commands, so we grow them at different increments.
#define REG_CMD_GROWSIZE	32
#define REG_CVAR_GROWSIZE	64

// Width required to printf a Reg*List index number, for show() functions.
// This used to correspond to the number of digits in MAX_REG, which was a
// fixed, compile-time limit.  However, now that the reg lists are grown
// dynamically, this has less strong correspondance to list sizes.  So for
// the moment, it reflects what one might normally expect to be the max
// width needed to print an index number; 4 allows 9999 (which is a damn
// lot, if you ask me).
#define WIDTH_MAX_REG	4

// Max number of registered user msgs we can manage.
#define MAX_REG_MSGS	256

// Max number of clients on server
#define MAX_CLIENTS_CONNECTED 32

// Flags to indicate if given cvar or func is part of a loaded plugin.
typedef enum {
	RG_INVALID,
	RG_VALID,
} REG_STATUS;

// Pointer to function registered by AddServerCommand.
typedef void (*REG_CMD_FN) (void);


// An individual registered function/command.
class MRegCmd : public class_metamod_new {
	friend class MRegCmdList;
	private:
	// data:
		int index;			// 1-based
	public:
		char *name;			// space is malloc'd
		REG_CMD_FN pfnCmd;		// pointer to the function
		int plugid;			// index id of corresponding plugin
		REG_STATUS status;		// whether corresponding plugin is loaded
	// functions:
		void DLLINTERNAL init(int idx);	// init values, as not using constructors
		mBOOL DLLINTERNAL call(void);	// try to call the function
};


// A list of registered commands.
class MRegCmdList : public class_metamod_new {
	private:
	// data:
		MRegCmd *mlist;			// malloc'd array of registered commands
		int size;			// current size of list
		int endlist;			// index of last used entry
		// Private; to satisfy -Weffc++ "has pointer data members but does
		// not override" copy/assignment constructor.
		void operator=(const MRegCmdList &src);
		MRegCmdList(const MRegCmdList &src);

	public:
	// constructor:
		MRegCmdList(void) DLLINTERNAL;

	// functions:
		MRegCmd * DLLINTERNAL find(const char *findname);	// find by MRegCmd->name
		MRegCmd * DLLINTERNAL add(const char *addname);
		void DLLINTERNAL disable(int plugin_id);		// change status to Invalid
		void DLLINTERNAL show(void);			// list all funcs to console
		void DLLINTERNAL show(int plugin_id);		// list given plugin's funcs to console
};



// An individual registered cvar.
class MRegCvar : public class_metamod_new {
	friend class MRegCvarList;
	private:
	// data:
		int index;				// 1-based
	public:
		cvar_t *data;				// actual cvar structure, malloc'd
		int plugid;				// index id of corresponding plugin
		REG_STATUS status;			// whether corresponding plugin is loaded
	// functions:
		void DLLINTERNAL init(int idx);		// init values, as not using constructors
		mBOOL DLLINTERNAL set(cvar_t *src);
};


// A list of registered cvars.
class MRegCvarList : public class_metamod_new {
	private:
	// data:
		MRegCvar *vlist;		// malloc'd array of registered cvars
		int size;			// size of list, ie MAX_REG_CVARS
		int endlist;			// index of last used entry
		// Private; to satisfy -Weffc++ "has pointer data members but does
		// not override" copy/assignment constructor.
		void operator=(const MRegCvarList &src);
		MRegCvarList(const MRegCvarList &src);

	public:
	// constructor:
		MRegCvarList(void) DLLINTERNAL;

	// functions:
		MRegCvar * DLLINTERNAL add(const char *addname);
		MRegCvar * DLLINTERNAL find(const char *findname);	// find by MRegCvar->data.name
		void DLLINTERNAL disable(int plugin_id);		// change status to Invalid
		void DLLINTERNAL show(void);			// list all cvars to console
		void DLLINTERNAL show(int plugin_id);		// list given plugin's cvars to console
};



// An individual registered user msg, from gamedll.
class MRegMsg : public class_metamod_new {
	friend class MRegMsgList;
	private:
	// data:
		int index;				// 1-based
	public:
		const char *name;		// name, assumed constant string in gamedll
		int msgid;				// msgid, assigned by engine
		int size;				// size, if given by gamedll
};


// A list of registered user msgs.
class MRegMsgList : public class_metamod_new {
	private:
	// data:
		MRegMsg mlist[MAX_REG_MSGS];	// array of registered msgs
		int size;						// size of list, ie MAX_REG_MSGS
		int endlist;					// index of last used entry

	public:
	// constructor:
		MRegMsgList(void) DLLINTERNAL;

	// functions:
		MRegMsg * DLLINTERNAL add(const char *addname, int addmsgid, int addsize);
		MRegMsg * DLLINTERNAL find(const char *findname);
		MRegMsg * DLLINTERNAL find(int findmsgid);
		void DLLINTERNAL show(void);						// list all msgs to console
};

#endif /* MREG_H */
