#include "precompiled.h"

MRegCmd::MRegCmd(const char* cmd_name, REG_CMD_FN cmd_handler, MPlugin* cmd_plugin) : m_pfunction(cmd_handler), m_plugid(cmd_plugin ? cmd_plugin->index() : 0), m_status(RG_VALID)
{
	m_name = Q_strdup(cmd_name);
}

MRegCmd::~MRegCmd()
{
	free(m_name);
}

bool MRegCmd::call() const
{
	m_pfunction();
	return m_status != RG_INVALID;
}

void MRegCmd::disable()
{
	m_pfunction = []() {
		};
	m_plugid = 0;
	m_status = RG_INVALID;
}

char* MRegCmd::getname() const
{
	return m_name;
}

REG_CMD_FN MRegCmd::gethandler() const
{
	return m_pfunction;
}

MRegCmdList::MRegCmdList() : m_list()
{
}

MRegCmdList::~MRegCmdList()
{
	for (auto reg : m_list) {
		delete reg;
	}
}

MRegCmd* MRegCmdList::find(const char* name) const
{
	for (auto reg : m_list) {
		if (!Q_stricmp(reg->m_name, name))
			return reg;
	}

	return nullptr;
}

MRegCmd* MRegCmdList::add(const char* addname, REG_CMD_FN cmd_handler, MPlugin* cmd_plugin)
{
	auto reg = new MRegCmd(addname, cmd_handler, cmd_plugin);
	m_list.push_back(reg);
	return reg;
}

void MRegCmdList::remove(char* cmd_name)
{
	for (auto it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		auto reg = *it;

		if (!Q_stricmp(reg->m_name, cmd_name)) {
			if (g_RehldsFuncs) {
				g_RehldsFuncs->Cmd_RemoveCmd(cmd_name);
				m_list.erase(it);
			}
			else {
				reg->disable();
			}
		}
	}
}

// Disable any functions belonging to the given plugin (by index id).
void MRegCmdList::remove(int owner_plugin_id)
{
	for (auto it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		auto reg = *it;

		if (reg->m_plugid == owner_plugin_id) {
			if (g_RehldsFuncs) {
				g_RehldsFuncs->Cmd_RemoveCmd(reg->m_name);
				m_list.erase(it);
			}
			else {
				reg->disable();
			}
		}
	}
}

// List all the registered commands.
void MRegCmdList::show() const
{
	int total_count = 0, valid_count = 0;
	char bplug[18 + 1]; // +1 for term null

	META_CONS("Registered plugin commands:");
	META_CONS("  %*s  %-*s  %-s", WIDTH_MAX_REG, "", sizeof bplug - 1, "plugin", "command");

	for (auto reg : m_list) {
		if (reg->m_status == RG_VALID) {
			auto iplug = g_plugins->find(reg->m_plugid);

			Q_strlcpy(bplug, iplug ? iplug->description() : "(unknown)");
		}
		else {
			Q_strlcpy(bplug, "(unloaded)");
		}

		META_CONS(" [%*d] %-*s  %-s", WIDTH_MAX_REG, ++total_count, sizeof bplug - 1, bplug, reg->m_name);

		if (reg->m_status == RG_VALID)
			valid_count++;
	}

	META_CONS("%d commands, %d available", total_count, valid_count);
}

// List all the registered commands for the given plugin id.
void MRegCmdList::show(int plugin_id) const
{
	int total_count = 0;

	META_CONS("Registered commands:");

	for (auto reg : m_list) {
		if (reg->m_plugid != plugin_id)
			continue;

		META_CONS("   %s", reg->m_name);
		total_count++;
	}

	META_CONS("%d commands", total_count);
}

MRegCvar::MRegCvar(cvar_t* cv_ptr, MPlugin* cv_plugin) : m_cvar(cv_ptr), m_plugid(cv_plugin ? cv_plugin->index() : 0), m_status(RG_VALID)
{
	m_cvar = new cvar_t;
	m_cvar->name = Q_strdup(cv_ptr->name);
	m_cvar->string = Q_strdup(cv_ptr->string);
	m_cvar->flags = cv_ptr->flags;
	m_cvar->value = cv_ptr->value;
	m_cvar->next = cv_ptr->next;
}

MRegCvar::~MRegCvar()
{
	free((void *)m_cvar->name);
	free(m_cvar->string);
	delete m_cvar;
}

cvar_t* MRegCvar::getcvar() const
{
	return m_cvar;
}

// Constructor
MRegCvarList::MRegCvarList() : m_list()
{
}

MRegCvarList::~MRegCvarList()
{
	for (auto reg : m_list) {
		delete reg;
	}
}

MRegCvar* MRegCvarList::add(cvar_t* src, MPlugin* plugin)
{
	MRegCvar* reg_cvar = new MRegCvar(src, plugin);
	m_list.push_back(reg_cvar);
	return reg_cvar;
}

MRegCvar* MRegCvarList::find(const char* findname)
{
	for (auto reg : m_list) {
		if (!Q_stricmp(reg->m_cvar->name, findname))
			return reg;
	}

	return nullptr;
}

// Disable any cvars belonging to the given plugin (by index id).
void MRegCvarList::disable(int plugin_id) const
{
	for (auto reg : m_list) {
		if (reg->m_plugid == plugin_id) {
			reg->m_status = RG_INVALID;
			reg->m_plugid = 0;
		}
	}
}

// List all the registered cvars.
void MRegCvarList::show() const
{
	int total_count = 0, valid_count = 0;
	char bplug[13 + 1], bname[20 + 1], bval[15 + 1]; // +1 for term null

	META_CONS("Registered plugin cvars:");
	META_CONS("  %*s  %-*s  %-*s  %*s  %s", WIDTH_MAX_REG, "", sizeof bplug - 1, "plugin", sizeof bname - 1, "cvar", sizeof bval - 1, "float value", "string value");

	for (auto reg : m_list) {
		if (reg->m_status == RG_VALID) {
			auto plug = g_plugins->find(reg->m_plugid);
			Q_strlcpy(bplug, plug ? plug->description() : "(unknown)");
		}
		else {
			Q_strlcpy(bplug, "(unloaded)");
		}

		Q_strlcpy(bname, reg->m_cvar->name);
		Q_snprintf(bval, sizeof bval, "%f", reg->m_cvar->value);
		META_CONS(" [%*d] %-*s  %-*s  %*s  %s", WIDTH_MAX_REG, ++total_count, sizeof bplug - 1, bplug, sizeof bname - 1, bname, sizeof bval - 1, bval, reg->m_cvar->string);

		if (reg->m_status == RG_VALID)
			valid_count++;
	}

	META_CONS("%d cvars, %d available", total_count, valid_count);
}

// List the registered cvars for the given plugin id.
void MRegCvarList::show(int plugin_id) const
{
	int total_count = 0;
	char bname[30 + 1], bval[15 + 1]; // +1 for term null

	META_CONS("%-*s     %*s  %s", sizeof bname - 1, "Registered cvars:", sizeof bval - 1, "float value", "string value");

	for (auto reg : m_list) {
		if (reg->m_plugid != plugin_id)
			continue;

		Q_strlcpy(bname, reg->m_cvar->name);
		Q_snprintf(bval, sizeof bval, "%f", reg->m_cvar->value);
		META_CONS("   %-*s  %*s  %s", sizeof bname - 1, bname, sizeof bval - 1, bval, reg->m_cvar->string);
		total_count++;
	}

	META_CONS("%d cvars", total_count);
}

MRegMsg::MRegMsg(const char* name, int msgid, int size) : m_name(name), m_msgid(msgid), m_size(size)
{
}

const char* MRegMsg::getname() const
{
	return m_name;
}

int MRegMsg::getid() const
{
	return m_msgid;
}

int MRegMsg::getsize() const
{
	return m_size;
}

MRegMsgList::MRegMsgList() : m_list()
{
}

MRegMsgList::~MRegMsgList()
{
	for (auto reg : m_list) {
		delete reg;
	}
}

MRegMsg* MRegMsgList::add(const char* addname, int addmsgid, int addsize)
{
	// Copy msg data into empty slot.
	// Note: 'addname' assumed to be a constant string allocated in the
	// gamedll.
	auto msg = new MRegMsg(addname, addmsgid, addsize);
	m_list.push_back(msg);
	return msg;
}

MRegMsg* MRegMsgList::find(const char* findname)
{
	for (auto msg : m_list) {
		if (!Q_strcmp(msg->m_name, findname))
			return msg;
	}

	return nullptr;
}

MRegMsg* MRegMsgList::find(int findmsgid)
{
	for (auto msg : m_list) {
		if (msg->m_msgid == findmsgid)
			return msg;
	}

	return nullptr;
}

// List the registered usermsgs for the gamedll.
void MRegMsgList::show()
{
	int total_count = 0;
	char bname[25 + 1]; // +1 for term null

	META_CONS("%-*s    %5s  %5s", sizeof bname - 1, "Game registered user msgs:", "msgid", "size");

	for (auto msg : m_list) {
		Q_strlcpy(bname, msg->m_name);
		META_CONS("   %-*s   %3d    %3d", sizeof bname - 1, bname, msg->m_msgid, msg->m_size);
		total_count++;
	}

	META_CONS("%d game user msgs", total_count);
}
