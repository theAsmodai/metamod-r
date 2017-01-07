#include "precompiled.h"

MRegCmd::MRegCmd(char* cmd_name, REG_CMD_FN cmd_handler, MPlugin* cmd_plugin) : pfnCmd(cmd_handler), status(RG_VALID), plugid(cmd_plugin->index)
{
	name = _strdup(name);
}

bool MRegCmd::call() const
{
	pfnCmd();
	return status != RG_INVALID;
}

void MRegCmd::disable()
{
	pfnCmd = [](){};
	plugid = 0;
	status = RG_INVALID;
}

char* MRegCmd::getname() const
{
	return name;
}

MRegCmdList::MRegCmdList() : m_list()
{
}

MRegCmd *MRegCmdList::find(const char *name) const
{
	for (auto icmd : m_list)
	{
		if (!Q_stricmp(icmd->name, name))
			return icmd;
	}

	return nullptr;
}

MRegCmd *MRegCmdList::add(char *addname, REG_CMD_FN cmd_handler, MPlugin* cmd_plugin)
{
	auto icmd = new MRegCmd(addname, cmd_handler, cmd_plugin);
	m_list.push_back(icmd);
	return icmd;
}

void MRegCmdList::remove(char* cmd_name)
{
	for (auto it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		auto icmd = *it;

		if (!Q_stricmp(icmd->name, cmd_name)) {
			if (g_RehldsFuncs) {
				g_RehldsFuncs->Cmd_RemoveCmd(cmd_name);
				m_list.erase(it);
			}
			else {
				icmd->disable();
			}
		}
	}
}

// Disable any functions belonging to the given plugin (by index id).
void MRegCmdList::remove(int owner_plugin_id)
{
	for (auto it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		auto icmd = *it;

		if (icmd->plugid == owner_plugin_id) {
			if (g_RehldsFuncs) {
				g_RehldsFuncs->Cmd_RemoveCmd(icmd->name);
				m_list.erase(it);
			}
			else {
				icmd->disable();
			}
		}
	}
}

// List all the registered commands.
void MRegCmdList::show() const
{
	int n = 0, a = 0;
	char bplug[18 + 1]; // +1 for term null

	META_CONS("Registered plugin commands:");
	META_CONS("  %*s  %-*s  %-s", WIDTH_MAX_REG, "", sizeof bplug - 1, "plugin", "command");

	for (auto icmd : m_list)
	{
		if (icmd->status == RG_VALID)
		{
			auto iplug = g_plugins->find(icmd->plugid);

			Q_strncpy(bplug, iplug ? iplug->desc : "(unknown)", sizeof bplug - 1);
			bplug[sizeof bplug - 1] = '\0';
		}
		else
		{
			Q_strncpy(bplug, "(unloaded)", sizeof bplug - 1);
			bplug[sizeof bplug - 1] = '\0';
		}

		META_CONS(" [%*d] %-*s  %-s", WIDTH_MAX_REG, ++n, sizeof bplug - 1, bplug, icmd->name);

		if (icmd->status == RG_VALID)
			a++;
	}

	META_CONS("%d commands, %d available", n, a);
}

// List all the registered commands for the given plugin id.
void MRegCmdList::show(int plugin_id) const
{
	int n = 0;

	META_CONS("Registered commands:");
	for (auto icmd : m_list)
	{
		if (icmd->plugid != plugin_id)
			continue;

		META_CONS("   %s", icmd->name);
		n++;
	}

	META_CONS("%d commands", n);
}

// Init values.  It would probably be more "proper" to use containers and
// constructors, rather than arrays and init-functions.
void MRegCvar::init(int idx)
{
	index = idx;
	data = NULL;
	plugid = 0;
	status = RG_INVALID;
}

// Set the cvar, copying values from given cvar.
// meta_errno values:
//  - ME_ARGUMENT	given cvar doesn't match this cvar
bool MRegCvar::set(cvar_t *src) const
{
	if (Q_stricmp(src->name, data->name))
	{
		META_ERROR("Tried to set cvar with mismatched name; src=%s dst=%s", src->name, data->name);
		return false;
	}

	// Would like to free() existing string, but can't tell where it was
	// allocated...
	data->string = Q_strdup(src->string);
	data->flags = src->flags;
	data->value = src->value;
	data->next = src->next;
	return true;
}

// Constructor
MRegCvarList::MRegCvarList()
	: vlist(0), size(REG_CVAR_GROWSIZE), endlist(0)
{
	vlist = (MRegCvar *)Q_malloc(size * sizeof(MRegCvar));

	// initialize array
	for (int i = 0; i < size; i++)
		vlist[i].init(i + 1); // 1 - based

	endlist = 0;
}

// Add the given cvar name to the list and return the instance.  This only
// writes the "name" to the new cvar; other fields are written with
// cvar::set().
// meta_errno values:
//  - ME_NOMEM			couldn't alloc or realloc for various parts
MRegCvar *MRegCvarList::add(const char *addname)
{
	MRegCvar *icvar;
	if (endlist == size)
	{
		// grow array
		int newsize = size + REG_CVAR_GROWSIZE;

		META_DEBUG(6, ("Growing reg cvar list from %d to %d", size, newsize));
		MRegCvar *temp = (MRegCvar *) realloc(vlist, newsize * sizeof(MRegCvar));
		if (!temp)
		{
			META_ERROR("Couldn't grow registered cvar list to %d for '%s'; %s", newsize, addname, strerror(errno));
			return NULL;
		}
		vlist = temp;
		size = newsize;
		// initialize new (unused) entries
		for (int i = endlist; i < size; i++)
			vlist[i].init(i + 1); // 1-based
	}

	icvar = &vlist[endlist];

	// Malloc space for the cvar and cvar name, for two reasons:
	//  - Can't point to memory loc in plugin (another segv waiting to
	//    happen).
	//  - Can't point to memory in vlist which might get moved later by
	//    realloc (again, segv).
	icvar->data = (cvar_t *)Q_malloc(sizeof(cvar_t));
	if (!icvar->data)
	{
		META_ERROR("Couldn't malloc cvar for adding reg cvar name '%s': %s", addname, strerror(errno));
		return NULL;
	}

	icvar->data->name = Q_strdup(addname);
	if (!icvar->data->name)
	{
		META_ERROR("Couldn't Q_strdup for adding reg cvar name '%s': %s", addname, strerror(errno));
		return NULL;
	}
	endlist++;
	return icvar;
}

// Try to find a registered cvar with the given name.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching cvar
MRegCvar *MRegCvarList::find(const char *findname)
{
	for (int i = 0; i < endlist; i++)
	{
		if (!_stricmp(vlist[i].data->name, findname))
			return &vlist[i];
	}

	return NULL;
}

// Disable any cvars belonging to the given plugin (by index id).
void MRegCvarList::disable(int plugin_id) const
{
	int i;
	MRegCvar *icvar;
	for (i = 0; i < size; i++)
	{
		icvar = &vlist[i];
		if (icvar->plugid == plugin_id)
		{
			icvar->status = RG_INVALID;
			icvar->plugid = 0;
			// Decided not to do this, in order to keep pre-existing values
			// after a plugin reload.
			// CVAR_SET_STRING(icvar->data->name, "[metamod: cvar invalid; plugin unloaded]");
		}
	}
}

// List all the registered cvars.
void MRegCvarList::show() const
{
	int i, n = 0, a = 0;
	MRegCvar *icvar;
	MPlugin *iplug;
	char bplug[13 + 1], bname[20 + 1], bval[15 + 1]; // +1 for term null

	META_CONS("Registered plugin cvars:");
	META_CONS("  %*s  %-*s  %-*s  %*s  %s", WIDTH_MAX_REG, "", sizeof(bplug) - 1, "plugin", sizeof(bname) - 1, "cvar", sizeof(bval) - 1, "float value", "string value");
	for (i = 0; i < endlist; i++)
	{
		icvar = &vlist[i];
		if (icvar->status == RG_VALID)
		{
			iplug = g_plugins->find(icvar->plugid);
			if (iplug)
			{
				Q_strncpy(bplug, iplug->desc, sizeof bplug - 1);
				bplug[sizeof bplug - 1] = '\0';
			}
			else
			{
				Q_strncpy(bplug, "(unknown)", sizeof bplug - 1);
				bplug[sizeof bplug - 1] = '\0';
			}
		}
		else
		{
			Q_strncpy(bplug, "(unloaded)", sizeof bplug - 1);
			bplug[sizeof bplug - 1] = '\0';
		}

		Q_strncpy(bname, icvar->data->name, sizeof bname - 1);
		bname[sizeof bname - 1] = '\0';
		Q_snprintf(bval, sizeof(bval), "%f", icvar->data->value);
		META_CONS(" [%*d] %-*s  %-*s  %*s  %s", WIDTH_MAX_REG, icvar->index, sizeof(bplug) - 1, bplug, sizeof(bname) - 1, bname, sizeof(bval) - 1, bval, icvar->data->string);

		if (icvar->status == RG_VALID)
			a++;

		n++;
	}
	META_CONS("%d cvars, %d available (%d allocated)", n, a, size);
}

// List the registered cvars for the given plugin id.
void MRegCvarList::show(int plugin_id) const
{
	int n = 0;
	MRegCvar *icvar;
	char bname[30 + 1], bval[15 + 1]; // +1 for term null

	META_CONS("%-*s     %*s  %s", sizeof(bname) - 1, "Registered cvars:", sizeof(bval) - 1, "float value", "string value");
	for (int i = 0; i < endlist; i++)
	{
		icvar = &vlist[i];
		if (icvar->plugid != plugin_id)
			continue;

		Q_strncpy(bname, icvar->data->name, sizeof bname - 1);
		bname[sizeof bname - 1] = '\0';
		Q_snprintf(bval, sizeof(bval), "%f", icvar->data->value);
		META_CONS("   %-*s  %*s  %s", sizeof(bname) - 1, bname, sizeof(bval) - 1, bval, icvar->data->string);
		n++;
	}

	META_CONS("%d cvars", n);
}

// Constructor
MRegMsgList::MRegMsgList()
	: size(MAX_REG_MSGS), endlist(0)
{
	// initialize array
	Q_memset(mlist, 0, sizeof(mlist));
	for (int i = 0; i < size; i++)
	{
		mlist[i].index = i + 1; // 1-based
	}

	endlist = 0;
}

// Add the given user msg the list and return the instance.
// meta_errno values:
//  - ME_MAXREACHED		reached max number of msgs allowed
MRegMsg *MRegMsgList::add(const char *addname, int addmsgid, int addsize)
{
	if (endlist == size)
	{
		// all slots used
		META_ERROR("Couldn't add registered msg '%s' to list; reached max msgs (%d)", addname, size);
		return NULL;
	}

	MRegMsg *imsg = &mlist[endlist];
	endlist++;

	// Copy msg data into empty slot.
	// Note: 'addname' assumed to be a constant string allocated in the
	// gamedll.
	imsg->name = addname;
	imsg->msgid = addmsgid;
	imsg->size = addsize;

	return imsg;
}

// Try to find a registered msg with the given name.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching cvar
MRegMsg *MRegMsgList::find(const char *findname)
{
	for (int i = 0; i < endlist; i++)
	{
		if (!Q_strcmp(mlist[i].name, findname))
			return &mlist[i];
	}

	return NULL;
}

// Try to find a registered msg with the given msgid.
// meta_errno values:
//  - ME_NOTFOUND	couldn't find a matching cvar
MRegMsg *MRegMsgList::find(int findmsgid)
{
	for (int i = 0; i < endlist; i++)
	{
		if (mlist[i].msgid == findmsgid)
			return &mlist[i];
	}

	return NULL;
}

// List the registered usermsgs for the gamedll.
void MRegMsgList::show()
{
	int i, n = 0;
	MRegMsg *imsg;
	char bname[25 + 1]; // +1 for term null

	META_CONS("%-*s    %5s  %5s", sizeof(bname) - 1, "Game registered user msgs:", "msgid", "size");
	for (i = 0; i < endlist; i++)
	{
		imsg = &mlist[i];

		Q_strncpy(bname, imsg->name, sizeof bname - 1);
		bname[sizeof bname - 1] = '\0';
		META_CONS("   %-*s   %3d    %3d", sizeof(bname) - 1, bname, imsg->msgid, imsg->size);
		n++;
	}

	META_CONS("%d game user msgs", n);
}
