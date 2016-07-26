#pragma once

#include "types_meta.h"		// mBOOL
#include "comp_dep.h"		//
#include "new_baseclass.h"

// Number of entries to add to reglists when they need to grow. Typically
// more cvars than commands, so we grow them at different increments.
#define REG_CMD_GROWSIZE	32
#define REG_CVAR_GROWSIZE	64

// Width required to printf a Reg*List index number, for show() functions.
// This used to correspond to the number of digits in MAX_REG, which was a
// fixed, compile-time limit. However, now that the reg lists are grown
// dynamically, this has less strong correspondance to list sizes. So for
// the moment, it reflects what one might normally expect to be the max
// width needed to print an index number; 4 allows 9999 (which is a damn
// lot, if you ask me).
#define WIDTH_MAX_REG	4

// Max number of registered user msgs we can manage.
#define MAX_REG_MSGS	256

// Max number of clients on server
#define MAX_CLIENTS_CONNECTED 32

// Flags to indicate if given cvar or func is part of a loaded plugin.
enum REG_STATUS { RG_INVALID, RG_VALID };

// Pointer to function registered by AddServerCommand.
typedef void (*REG_CMD_FN)();

// An individual registered function/command.
class MRegCmd: public class_metamod_new
{
public:
	friend class MRegCmdList;

	char *name;			// space is malloc'd
	REG_CMD_FN pfnCmd;		// pointer to the function
	int plugid;			// index id of corresponding plugin
	REG_STATUS status;		// whether corresponding plugin is loaded

	void init(int idx);		// init values, as not using constructors
	mBOOL call();			// try to call the function

private:
	int index;			// 1-based
};

// A list of registered commands.
class MRegCmdList : public class_metamod_new {
public:
	MRegCmdList();

	MRegCmd *find(const char *findname);	// find by MRegCmd->name
	MRegCmd *add(const char *addname);
	void disable(int plugin_id);		// change status to Invalid
	void show();			// list all funcs to console
	void show(int plugin_id);		// list given plugin's funcs to console

private:
	MRegCmd *mlist;			// malloc'd array of registered commands
	int size;			// current size of list
	int endlist;			// index of last used entry

	// Private; to satisfy -Weffc++ "has pointer data members but does
	// not override" copy/assignment constructor.
	void operator=(const MRegCmdList &src);
	MRegCmdList(const MRegCmdList &src);
};

// An individual registered cvar.
class MRegCvar : public class_metamod_new {
public:
	friend class MRegCvarList;

	cvar_t *data;				// actual cvar structure, malloc'd
	int plugid;				// index id of corresponding plugin
	REG_STATUS status;			// whether corresponding plugin is loaded

	void init(int idx);			// init values, as not using constructors
	mBOOL set(cvar_t *src);
private:
	int index;				// 1-based
};

// A list of registered cvars.
class MRegCvarList: public class_metamod_new {
public:
	MRegCvarList();

	MRegCvar *add(const char *addname);
	MRegCvar *find(const char *findname);	// find by MRegCvar->data.name
	void disable(int plugin_id);		// change status to Invalid
	void show();				// list all cvars to console
	void show(int plugin_id);		// list given plugin's cvars to console

private:
	MRegCvar *vlist;			// malloc'd array of registered cvars
	int size;				// size of list, ie MAX_REG_CVARS
	int endlist;				// index of last used entry

	// Private; to satisfy -Weffc++ "has pointer data members but does
	// not override" copy/assignment constructor.
	void operator=(const MRegCvarList &src);
	MRegCvarList(const MRegCvarList &src);
};

// An individual registered user msg, from gamedll.
class MRegMsg: public class_metamod_new {
public:
	friend class MRegMsgList;

	const char *name;			// name, assumed constant string in gamedll
	int msgid;				// msgid, assigned by engine
	int size;				// size, if given by gamedll

private:
	int index;				// 1-based
};

// A list of registered user msgs.
class MRegMsgList: public class_metamod_new {
public:
	MRegMsgList();

	MRegMsg *add(const char *addname, int addmsgid, int addsize);
	MRegMsg *find(const char *findname);
	MRegMsg *find(int findmsgid);
	void show();						// list all msgs to console

private:
	MRegMsg mlist[MAX_REG_MSGS];			// array of registered msgs
	int size;					// size of list, ie MAX_REG_MSGS
	int endlist;					// index of last used entry
};
