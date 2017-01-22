#include "precompiled.h"

const char* g_platform_postfixes[] = {
	"_i386.so",
	"_i486.so",
	"_i586.so",
	"_i686.so",
};

// Parse a line from plugins.ini into a plugin.
bool MPlugin::ini_parseline(char *_line)
{
	char line[1024];
	strncpy(line, _line, sizeof line - 1);
	line[sizeof line - 1] = '\0';

	trimbuf(line);

	// skip empty lines
	if (line[0] == '\0') {
		META_DEBUG(7, "ini: Ignoring empty line: %s", line);
		return false;
	}

	if (line[0] == '#' || line[0] == ';' || !Q_strncmp(line, "//", 2)) {
		META_DEBUG(7, "ini: Ignoring commented line: %s", line);
		return false;
	}

	// grab platform ("win32" or "linux")
	char* ptr_token;
	char* token = strtok_r(line, " \t", &ptr_token);
	if (!token)
		return false;

	if (!Q_stricmp(token, PLATFORM)) {
		m_pfspecific = 0; // TODO: remove it?
	}
	else if (!Q_stricmp(token, PLATFORM_SPC)) {
		m_pfspecific = 1;
	}
	else {
		// plugin is not for this OS
		META_DEBUG(7, "ini: Ignoring entry for %s", token);
		return false;
	}

	// grab filename
	token = strtok_r(nullptr, " \t\r\n", &ptr_token);
	if (!token) {
		return false;
	}

	Q_strncpy(m_filename, token, sizeof m_filename - 1);
	m_filename[sizeof m_filename - 1] = '\0';
	NormalizePath(m_filename);

	// Store name of just the actual _file_, without dir components.
	char* cp = Q_strrchr(m_filename, '/');
	if (cp)
		m_file = cp + 1;
	else
		m_file = m_filename;

	// Grab description.
	// Just get the the rest of the line, minus line-termination.
	token = strtok_r(nullptr, "\n\r", &ptr_token);
	if (token)
	{
		token = token + strspn(token, " \t"); // skip whitespace
		Q_strncpy(m_desc, token, sizeof m_desc - 1);
		m_desc[sizeof m_desc - 1] = '\0';
	}
	else
	{
		// If no description is specified, temporarily use plugin file,
		// until plugin can be queried, and desc replaced with info->name.
		Q_snprintf(m_desc, sizeof m_desc, "<%s>", m_file);
	}

	// Make full pathname (from gamedir if relative, remove "..",
	// backslashes, etc).
	full_gamedir_path(m_filename, m_pathname);

	m_source = PS_INI;
	m_status = PL_VALID;
	return true;
}

// Unload a plugin from plugin request
bool MPlugin::plugin_unload(plid_t plid, PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	MPlugin *pl_unloader = g_plugins->find(plid);

	if (!pl_unloader) {
		META_WARNING("dll: Not unloading plugin '%s'; plugin that requested unload is not found.", m_desc);
		return false;
	}
	if (pl_unloader->m_index == m_index) {
		META_WARNING("dll: Not unloading plugin '%s'; Plugin tried to unload itself.", m_desc);
		return false;
	}
	if (m_is_unloader) {
		META_WARNING("dll: Not unloading plugin '%s'; Plugin is unloading plugin that tried to unload it.", m_desc);
		return false;
	}
	
	m_unloader_index = pl_unloader->m_index;

	//block unloader from being unloaded by other plugin
	pl_unloader->m_is_unloader = true;

	//try unload
	PLUG_ACTION old_action = m_action;
	m_action = PA_UNLOAD;

	if (unload(now, reason, PNL_PLG_FORCED)) {
		META_DEBUG(1, "Unloaded plugin '%s'", m_desc);
		pl_unloader->m_is_unloader = false;
		return true;
	}

	pl_unloader->m_is_unloader = false;
	m_action = old_action;

	return false;
}

// Parse a filename string from PEXT_LOAD_PLUGIN_BY_ *function into a plugin.
bool MPlugin::plugin_parseline(const char *fname, int loader_index)
{
	char *cp;

	m_source_plugin_index = loader_index;

	Q_strncpy(m_filename, fname, sizeof m_filename - 1);
	m_filename[sizeof m_filename - 1] = '\0';
	NormalizePath(m_filename);

	//store just name of the actual _file, without path
	cp = Q_strrchr(m_filename, '/');
	if (cp)
		m_file = cp + 1;
	else
		m_file = m_filename;

	//grab description
	//temporarily use plugin file until plugin can be queried
	Q_snprintf(m_desc, sizeof m_desc, "<%s>", m_file);

	//make full pathname
	full_gamedir_path(m_filename, m_pathname);

	m_source = PS_PLUGIN;
	m_status = PL_VALID;

	return true;
}

// Parse a line from console "load" command into a plugin.
bool MPlugin::cmd_parseline(const char *line)
{
	char buf[NAME_MAX + PATH_MAX + MAX_DESC_LEN];
	char *token;
	char *ptr_token;
	char *cp;

	Q_strncpy(buf, line, sizeof buf - 1);
	buf[sizeof buf - 1] = '\0';

	// remove "load"
	token = strtok_r(buf, " \t", &ptr_token);
	if (!token)
		return false;

	// grab filename
	token = strtok_r(nullptr, " \t", &ptr_token);
	if (!token)
		return false;

	Q_strncpy(m_filename, token, sizeof m_filename - 1);
	m_filename[sizeof m_filename - 1] = '\0';
	NormalizePath(m_filename);

	// store name of just the actual _file_, without dir components
	cp = Q_strrchr(m_filename, '/');
	if (cp)
		m_file = cp + 1;
	else
		m_file = m_filename;

	// Grab description.
	// Specify no delimiter chars, as we just want the rest of the line.
	token = strtok_r(nullptr, "", &ptr_token);
	if (token)
	{
		token = token + strspn(token, " \t"); // skip whitespace
		Q_strncpy(m_desc, token, sizeof m_desc - 1);
		m_desc[sizeof m_desc - 1] = '\0';
	}
	else
	{
		// if no description is specified, temporarily use plugin file,
		// until plugin can be queried, and desc replaced with info->name.
		Q_snprintf(m_desc, sizeof m_desc, "<%s>", m_file);
	}

	// Make full pathname (from gamedir if relative, remove "..",
	// backslashes, etc).
	full_gamedir_path(m_filename, m_pathname);

	m_source = PS_CMD;
	m_status = PL_VALID;
	return true;
}

// Make sure this plugin has the necessary minimal information.
bool MPlugin::check_input(void)
{
	// doublecheck our input/state
	if (m_status < PL_VALID) {
		META_ERROR("dll: Tried to operate on plugin[%d] with a non-valid status (%d)", m_index, str_status());
		return false;
	}
	if (!m_file || !m_file[0]) {
		META_ERROR("dll: Tried to operate on plugin[%d] with an empty file", m_index);
		return false;
	}
	if (!m_filename[0]) {
		META_ERROR("dll: Tried to operate on plugin[%d] with an empty filename", m_index);
		return false;
	}
	if (!m_pathname[0]) {
		META_ERROR("dll: Tried to operate on plugin[%d] with an empty pathname", m_index);
		return false;
	}

	if (!m_desc[0]) {
		// if no description is specified, temporarily use plugin file,
		// until plugin can be queried, and desc replaced with info->name.
		Q_snprintf(m_desc, sizeof m_desc, "<%s>", m_file);
	}

	return true;
}

// Try to resolve a plugin's filename as a (possibly partial) path to an
// actual filename on disk, to facilitate easier console load-command
// arguments.  Uses resolve_dirs, resolve_prefix, and resolve_suffix below.
// Example paths that it tries:
//    filename
//    Gamedir/filename.dll, Gamedir/filename.so
//    Gamedir/filename_i386.so
//    Gamedir/dlls/mm_filename_i386.so
//    Gamedir/dlls/filename_mm_i386.so
//    Gamedir/dlls/filename_MM_i386.so
bool MPlugin::resolve(void)
{
	char *found;

	if (!check_input()) {
		return false;
	}

	if (IsAbsolutePath(m_filename))
		found = resolve_prefix(m_filename);
	else
		found = resolve_dirs(m_filename);

	if (!found)
	{
		META_DEBUG(2, "Couldn't resolve '%s' to file", m_filename);
		return false;
	}

	META_DEBUG(2, "Resolved '%s' to file '%s'", m_filename, found);

	// store pathname: the resolved path (should be absolute)
	Q_strncpy(m_pathname, found, sizeof m_pathname - 1);
	m_pathname[sizeof m_pathname - 1] = '\0';

	// store file: the name of the file (without dir)
	char* cp = Q_strrchr(m_pathname, '/');
	if (cp)
		m_file = cp + 1;
	else
		m_file = m_pathname;

	// store pathname: the gamedir relative path, or an absolute path
	size_t len = Q_strlen(g_GameDLL.gamedir);

	if (!Q_strnicmp(m_pathname, g_GameDLL.gamedir, len))
	{
		Q_strncpy(m_filename, m_pathname + len + 1, sizeof m_filename - 1);
		m_filename[sizeof m_filename - 1] = '\0';
	}
	else
	{
		Q_strncpy(m_filename, m_pathname, sizeof m_filename - 1);
		m_filename[sizeof m_filename - 1] = '\0';
	}

	return true;
}

// For the given path, tries to find file in several possible
// directories.
// Try:
//     GAMEDIR/filename
//     GAMEDIR/dlls/filename
char *MPlugin::resolve_dirs(char *path) const
{
	static char buf[PATH_MAX];

	Q_snprintf(buf, sizeof buf, "%s/%s", g_GameDLL.gamedir, path);

	// try this path
	struct stat st;
	if (!stat(buf, &st) && S_ISREG(st.st_mode))
		return buf;

	// try other file prefixes in this path
	char* found = resolve_prefix(buf);
	if (found)
		return found;

	Q_snprintf(buf, sizeof buf, "%s/dlls/%s", g_GameDLL.gamedir, path);

	// try this path
	if (!stat(buf, &st) && S_ISREG(st.st_mode))
		return buf;

	// try other file prefixes for this path
	return resolve_prefix(buf);
}

// For the given path, tries several possible filename prefixes.
// Try:
//     dir/mm_file
//     dir/file
char *MPlugin::resolve_prefix(char *path) const
{
	struct stat st;
	char *cp, *fname;
	char dname[PATH_MAX];
	static char buf[PATH_MAX];
	char *found;

	// try "mm_" prefix FIRST.
	// split into dirname and filename
	Q_strncpy(dname, path, sizeof dname - 1);
	dname[sizeof dname - 1] = '\0';

	cp = Q_strrchr(dname, '/');
	if (cp)
	{
		*cp = '\0';
		fname = cp + 1;
		Q_snprintf(buf, sizeof buf, "%s/mm_%s", dname, fname);
	}
	else
	{
		// no directory in given path
		Q_snprintf(buf, sizeof buf, "mm_%s", path);
	}

	// try this path
	if (!stat(buf, &st) && S_ISREG(st.st_mode))
		return buf;

	// try other suffixes for this path
	if ((found = resolve_suffix(buf)))
		return found;

	// try other suffixes for the original path
	if ((found = resolve_suffix(path)))
		return found;

	return nullptr;
}

// For the given path, tries several different filename suffixes.
// Try:
//     path
//     path_mm
//     path_MM
//     path.so (linux), path.dll (win32), path.dylib (osx)
//     path_i386.so, path_i486.so, etc (if linux)
char *MPlugin::resolve_suffix(char *path) const
{
	char *found = nullptr;

	auto check = [](const char* path) -> char*
	{
		struct stat st;

		if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
			static char buf[PATH_MAX];
			strncpy(buf, path, sizeof buf - 1);
			buf[sizeof buf - 1];
			return buf;
		}

		return nullptr;
	};

	if (Q_strstr(path, PLATFORM_DLEXT)) {
		return check(path);
	}

	char tmpbuf[PATH_MAX];
	Q_snprintf(tmpbuf, sizeof tmpbuf, "%s%s", path, PLATFORM_DLEXT);
	if ((found = check(tmpbuf))) return found;

	// Hmm, recursion.
	if (!Q_strstr(path, "_mm"))
	{
		Q_snprintf(tmpbuf, sizeof tmpbuf, "%s_mm", path);
		if ((found = resolve_suffix(tmpbuf))) return found;
	}

#ifndef _WIN32
	for (size_t i = 0; i < arraysize(g_platform_postfixes); i++) {
		Q_snprintf(tmpbuf, sizeof tmpbuf, "%s%s", path, g_platform_postfixes[i]);
		if ((found = check(tmpbuf))) return found;
	}
#endif

	return nullptr;
}

// Check if a passed string starts with a known platform postfix.
// It does not check beyond the period in order to work for both
// Linux and Win32.
static bool is_platform_postfix(char *pf)
{
	if (pf) {
		for (size_t i = 0; i < arraysize(g_platform_postfixes); i++) {
			if (!Q_strcmp(pf, g_platform_postfixes[i]))
				return true;
		}
	}
	return false;
}


// Check if a given plugin is the same but possibly for a
// different platform. A match is considered to be found if
// 1.  the filename without the path is the same, or
// 2a. for an attached plugin the logtag is the same, or
// 2b. the description is the same, or
// 3.  a significant part of the filename is the same.
// A significant part of a plugin name is currently defined to:
//  the part up to a known platform postfix as determined by
//  the is_platform_postfix() function (see above), or
//  the part up to the last dot, if one exists.
bool MPlugin::platform_match(MPlugin *other) const
{
	char *end, *other_end;
	int prefixlen;

	if (m_status == PL_EMPTY || other->m_status == PL_EMPTY)
		return false;

	if (!Q_strcmp(m_file, other->m_file))
		return true;

	if (m_status >= PL_OPENED && other->m_status >= PL_OPENED && !Q_strcmp(m_info->logtag, other->m_info->logtag))
		return true;

	if (*m_desc != '\0' && !Q_stricmp(m_desc, other->m_desc))
		return true;

	end = Q_strrchr(m_file, '_');
	if (!end || !is_platform_postfix(end)) end = Q_strrchr(m_file, '.');
		other_end = Q_strrchr(other->m_file, '_');

	if (!other_end || !is_platform_postfix(other_end))
		other_end = Q_strrchr(other->m_file, '.');

	if (!end || !other_end)
		return false;

	prefixlen = end - m_file;
	if (other_end - other->m_file != prefixlen)
		return false;

	if (!Q_strncmp(m_file, other->m_file, prefixlen))
		return true;

	return false;
}

// Load a plugin; query, check allowed time, attach.
bool MPlugin::load(PLUG_LOADTIME now)
{
	if (!check_input())
	{
		return false;
	}
	if (m_status >= PL_RUNNING)
	{
		META_ERROR("dll: Not loading plugin '%s'; already loaded (status=%s)", m_desc, str_status());
		return false;
	}
	if (m_action != PA_LOAD && m_action != PA_ATTACH)
	{
		META_ERROR("dll: Not loading plugin '%s'; not marked for load (action=%s)", m_desc, str_action());
		return false;
	}

	if (m_status < PL_OPENED)
	{
		// query plugin; open file and get info about it
		if (!query())
		{
			META_ERROR("dll: Skipping plugin '%s'; couldn't query", m_desc);
			if (!m_sys_module.unload())
			{
				META_ERROR("dll: Couldn't close plugin file '%s': %s", m_file, "invalid handle");
			}
			m_status = PL_BADFILE;
			m_info = nullptr; //prevent crash
			return false;
		}
		m_status = PL_OPENED;
	}

	// are we allowed to attach this plugin at this time?
	if (m_info->loadable < now)
	{
		if (m_info->loadable > PT_STARTUP)
		{
			// will try to attach again at next opportunity
			META_DEBUG(2, "dll: Delaying load plugin '%s'; can't attach now: allowed=%s; now=%s", m_desc, str_loadable(), str_loadtime(now, SL_SIMPLE));
			return false;
		}
		else
		{
			META_DEBUG(2, "dll: Failed load plugin '%s'; can't attach now: allowed=%s; now=%s", m_desc, str_loadable(), str_loadtime(now, SL_SIMPLE));
			// don't try to attach again later
			m_action = PA_NONE;
			return false;
		}
	}

	// attach plugin; get function tables
	if (attach(now) != true)
	{
		META_ERROR("dll: Failed to attach plugin '%s'", m_desc);
		// Note we don't dlclose() here, since we're returning PL_FAILED,
		// which implies that it's been dlopened and queried successfully.
		// Doing so causes crashes, because things like "meta list" try to
		// look at *info, which is in the DLL memory space and unaccessible
		// (segfault) after dlclosed.
		m_status = PL_FAILED;
		return false;
	}

	m_status = PL_RUNNING;
	m_action = PA_NONE;

	// If not loading at server startup, then need to call plugin's
	// GameInit, since we've passed that.
	if (now != PT_STARTUP && m_dllapi_table && m_dllapi_table->pfnGameInit) {
		m_dllapi_table->pfnGameInit();
	}
	// If loading during map, then I'd like to call plugin's
	// ServerActivate, since we've passed that.  However, I'm not sure what
	// arguments to give it...  So, we'll just have to say for the
	// moment that plugins that are loadable during a map can't need to
	// hook ServerActivate.

	META_LOG("dll: Loaded plugin '%s': %s v%s %s, %s", m_desc, m_info->name, m_info->version, m_info->date, m_info->author);
	return true;
}

// Query a plugin:
//  - dlopen() the file, store the handle
//  - dlsym() and call:
//	    Meta_Init (if present) - tell dll it'll be used as a metamod plugin
//	    GiveFnptrsToDll - give engine function ptrs
//	    Meta_Query - say "hi" and get info about plugin
bool MPlugin::query(void)
{
	// open the plugin DLL
	if (!m_sys_module.load(m_pathname)) {
		META_ERROR("dll: Failed query plugin '%s'; Couldn't open file '%s': %s", m_desc, m_pathname, m_sys_module.getloaderror());
		return false;
	}

	// First, we check to see if they have a Meta_Query.  We would normally
	// dlsym this just prior to calling it, after having called
	// GiveFnptrsToDll, but things like standalone-DLL bots do all their
	// startup in GiveFnptrsToDll, and trying to load them as metamod
	// plugins creates all sorts of crashes.  So, we do a trivial check
	// first to see if the DLL looks like a metamod plugin before
	// proceeding with the normal order.  Note that we still have to call
	// GiveFnptrsToDll before Meta_Query, because the latter typically uses
	// engine functions like AlertMessage, which have to be passed along via
	// GiveFnptrsToDll.
	auto pfn_query = (META_QUERY_FN)m_sys_module.getsym("Meta_Query");
	if (!pfn_query)
	{
		META_ERROR("dll: Failed query plugin '%s'; Couldn't find Meta_Query(): %s", m_desc, "function not found");
		// caller will dlclose()
		return false;
	}

	// Call Meta_Init, if present.  This is an optional plugin routine to
	// allow plugin to do any special initializing or other processing
	// prior to the standard query/attach procedure.
	//
	// In particular, this should allow for DLL's that can operate as both
	// a standalone dll AND a metamod plugin.  This routine has to be
	// called _before_ GiveFnptrsToDll, since the dll will usually do all
	// its startup processing in GiveFn, and has to know at that point
	// whether it needs to do just mm plugin startup, or all the extra
	// startup needed for a standalone DLL.
	//
	// This passes nothing and returns nothing, and the routine in the
	// plugin can NOT use any g_engine functions, as they haven't been
	// provided yet (done next, in GiveFnptrsToDll).
	auto pfn_init = (META_INIT_FN)m_sys_module.getsym("Meta_Init");
	if (pfn_init)
	{
		pfn_init();
		META_DEBUG(6, "dll: Plugin '%s': Called Meta_Init()", m_desc);
	}
	else
	{
		META_DEBUG(5, "dll: no Meta_Init present in plugin '%s'", m_desc);
		// don't return; not an error
	}

	// pass on engine function table and globals to plugin
	auto pfn_give_engfuncs = (GIVE_ENGINE_FUNCTIONS_FN)m_sys_module.getsym("GiveFnptrsToDll");
	if (!pfn_give_engfuncs)
	{
		// META_ERROR("dll: Couldn't find GiveFnptrsToDll() in plugin '%s': %s", desc, DLERROR());
		META_ERROR("dll: Failed query plugin '%s'; Couldn't find GiveFnptrsToDll(): %s", m_desc, "function not found");
		// caller will dlclose()
		return false;
	}
	pfn_give_engfuncs(g_engine.pl_funcs, g_engine.globals);
	META_DEBUG(6, "dll: Plugin '%s': Called GiveFnptrsToDll()", m_desc);

	// Call plugin's Meta_Query(), to pass our meta interface version, and get
	// plugin's info structure.
	m_info = nullptr;

	// Make a copy of the meta_util function table for each plugin, for the
	// same reason.
	Q_memcpy(&m_mutil_funcs, &g_MetaUtilFunctions, sizeof m_mutil_funcs);

	if (pfn_query(META_INTERFACE_VERSION, &m_info, &m_mutil_funcs) != TRUE)
	{
		META_ERROR("dll: Failed query plugin '%s'; Meta_Query returned error", m_desc);
	}
	else
	{
		META_DEBUG(6, "dll: Plugin '%s': Called Meta_Query() successfully", m_desc);
	}

	// Check for interface differences...  Generally, a difference in major
	// version will be incompatible, and a plugin that expects a later
	// minor version will be incompatible (it's expecting things that this
	// Metamod won't be supplying).  g_plugins that use an older minor
	// version will still work, as backward-compability within major
	// version is expected (forward-compatibility is not).
	//
	// Note, this check is done regardless of whether meta_query returns an
	// error.
	if (!m_info) {
		META_ERROR("dll: Failed query plugin '%s'; Empty info structure", m_desc);
		// caller will dlclose()
		return false;
	}

	if (!Q_strcmp(m_info->ifvers, META_INTERFACE_VERSION))
	{
		int mmajor = 0, mminor = 0, pmajor = 0, pminor = 0;
		META_DEBUG(3, "dll: Note: Plugin '%s' interface version didn't match; expected %s, found %s", m_desc, META_INTERFACE_VERSION, m_info->ifvers);
		sscanf(META_INTERFACE_VERSION, "%d:%d", &mmajor, &mminor);
		sscanf(m_info->ifvers, "%d:%d", &pmajor, &pminor);
		// If plugin has later interface version, it's incompatible
		// (update metamod).
		if (pmajor > mmajor || (pmajor == mmajor && pminor > mminor))
		{
			META_ERROR("dll: Plugin '%s' requires a newer version of Metamod (Metamod needs at least interface %s not the current %s)", m_desc, m_info->ifvers, META_INTERFACE_VERSION);
		}
		// If plugin has older major interface version, it's incompatible
		// (update plugin).
		else if (pmajor < mmajor)
		{
			META_ERROR("dll: Plugin '%s' is out of date and incompatible with this version of Metamod; please find a newer version of the plugin (plugin needs at least interface %s not the current %s)", m_desc, META_INTERFACE_VERSION, m_info->ifvers);
		}
		// Plugin has identical major, with older minor.  This is supposed to be
		// backwards compatible, so we warn, but we still accept it.
		else if (pmajor == mmajor && pminor < mminor)
			META_LOG("dll: Note: plugin '%s' is using an older interface version (%s), not the latest interface version (%s); there might be an updated version of the plugin", m_desc, m_info->ifvers, META_INTERFACE_VERSION);
		else
			META_LOG("dll: Plugin '%s': unexpected version comparision; metavers=%s, mmajor=%d, mminor=%d; plugvers=%s, pmajor=%d, pminor=%d", m_desc, META_INTERFACE_VERSION, mmajor, mminor, m_info->ifvers, pmajor, pminor);

		if (pmajor != mmajor) {
			META_ERROR("dll: Rejected plugin '%s' due to interface version incompatibility (mm=%s, pl=%s)", m_desc, META_INTERFACE_VERSION, m_info->ifvers);
			// caller will dlclose()
			return false;
		}
	}

	// Replace temporary desc with plugin's internal name.
	if (m_desc[0] == '<')
	{
		Q_strncpy(m_desc, m_info->name, sizeof m_desc - 1);
		m_desc[sizeof m_desc - 1] = '\0';
	}

	META_DEBUG(6, "dll: Plugin '%s': Query successful", m_desc);
	return true;
}

template<typename getter_t, typename table_t>
bool get_function_table_from_plugin(const char* pl_desc, int ifvers_mm, getter_t getter, const char* getter_name, table_t*& table, size_t table_size = sizeof(table_t))
{
	int ifvers_pl = ifvers_mm;

	if (getter) {
		if (!table)
			table = (table_t *)Q_calloc(1, table_size);
		if (getter(table, &ifvers_pl)) {
			META_DEBUG(3, "dll: Plugin '%s': Found %s", pl_desc, getter_name);
			return true;
		}

		META_ERROR("dll: Failure calling %s in plugin '%s'", getter_name, pl_desc);
		if (ifvers_pl != ifvers_mm)
			META_ERROR("dll: Interface version didn't match; expected %d, found %d", ifvers_mm, ifvers_pl);
	}
	else {
		META_DEBUG(5, "dll: Plugin '%s': No %s", pl_desc, getter_name);
		if (table)
			Q_free(table);
		table = nullptr;
	}

	return false;
}

template<typename getter_t, typename table_t>
bool get_function_table_from_plugin_old(const char* pl_desc, int ifvers_mm, getter_t getter, const char* getter_name, table_t*& table, size_t table_size = sizeof(table_t))
{
	if (getter) {
		if (!table)
			table = (table_t *)Q_calloc(1, table_size);
		if (getter(table, ifvers_mm)) {
			META_DEBUG(3, "dll: Plugin '%s': Found %s", pl_desc, getter_name);
			return true;
		}

		META_ERROR("dll: Failure calling %s in plugin '%s'", getter_name, pl_desc);
	}
	else {
		META_DEBUG(5, "dll: Plugin '%s': No %s", pl_desc, getter_name);
		if (table)
			Q_free(table);
		table = nullptr;
	}

	return false;
}

// Attach a plugin:
//	- dlsym() and call:
//	    Meta_Attach - get table of api tables, give meta_globals
//  - if provided by plugin, call various "Get" function pointers,
//    and store resulting function tables:
//		GetEntityAPI			(std)
//		GetEntityAPI2			(std sdk2)
//		GetNewDLLFunctions		(std sdk2)
//
//		GetEntityAPI_Post		(meta)
//		GetEntityAPI2_Post		(meta)
//		GetNewDLLFunctions_Post	(meta)
//
//		GetEngineFunctions		(meta)
//		GetEngineFunctions_Post	(meta)
bool MPlugin::attach(PLUG_LOADTIME now)
{
	// Make copy of gameDLL's function tables for each plugin, so we don't
	// risk the plugins screwing with the tables everyone uses.
	if (g_GameDLL.funcs.dllapi_table && !m_gamedll_funcs.dllapi_table) // TODO: check it
	{
		m_gamedll_funcs.dllapi_table = (DLL_FUNCTIONS *)Q_malloc(sizeof(DLL_FUNCTIONS));
		if (!m_gamedll_funcs.dllapi_table)
		{
			META_ERROR("dll: Failed attach plugin '%s': Failed malloc() for dllapi_table");
			return false;
		}
		Q_memcpy(m_gamedll_funcs.dllapi_table, g_GameDLL.funcs.dllapi_table, sizeof(DLL_FUNCTIONS));
	}
	if (g_GameDLL.funcs.newapi_table && !m_gamedll_funcs.newapi_table)
	{
		m_gamedll_funcs.newapi_table = (NEW_DLL_FUNCTIONS *)Q_calloc(1, sizeof(meta_new_dll_functions_t));
		if (!m_gamedll_funcs.newapi_table)
		{
			META_ERROR("dll: Failed attach plugin '%s': Failed malloc() for newapi_table");
			return false;
		}
		static_cast<meta_new_dll_functions_t *>(m_gamedll_funcs.newapi_table)->set_from(g_GameDLL.funcs.newapi_table);
	}
	auto pfn_attach = (META_ATTACH_FN)m_sys_module.getsym("Meta_Attach");
	if (!pfn_attach)
	{
		META_ERROR("dll: Failed attach plugin '%s': Couldn't find Meta_Attach(): %s", m_desc, "function not found");
		// caller will dlclose()
		return false;
	}

	META_FUNCTIONS meta_table = {};

	// get table of function tables,
	// give public meta globals
	qboolean ret = pfn_attach(now, &meta_table, &g_metaGlobals, &m_gamedll_funcs);
	if (ret != TRUE)
	{
		META_ERROR("dll: Failed attach plugin '%s': Error from Meta_Attach(): %d", m_desc, ret);
		// caller will dlclose()
		return false;
	}

	META_DEBUG(6, "dll: Plugin '%s': Called Meta_Attach() successfully", m_desc);

	// Look for API-NEW interface in plugin.  We do this before API2/API, because
	// that's what the engine appears to do..
	// [But we only do this if the gamedll provides these, so that we don't
	// give a plugin the idea that we'll call them, when in fact we won't
	// (since we don't export them to the engine when the gamedll doesn't
	// provide them).]
	// We now do this even when the gamedll doesn't provide these because
	// the plugins might want to use new functions like CvarValue() also
	// with older gamedlls which do not use the API-NEW themselves.
	// It is yet unknown if this causes any problems in the engine.
	get_function_table_from_plugin(m_desc, NEW_DLL_FUNCTIONS_VERSION, meta_table.pfnGetNewDLLFunctions, "GetNewDLLFunctions", m_newapi_table);
	get_function_table_from_plugin(m_desc, NEW_DLL_FUNCTIONS_VERSION, meta_table.pfnGetNewDLLFunctions_Post, "GetNewDLLFunctions_Post", m_newapi_post_table);

	// Look for API2 interface in plugin; preferred over API-1.
	get_function_table_from_plugin(m_desc, INTERFACE_VERSION, meta_table.pfnGetEntityAPI2, "GetEntityAPI2", m_dllapi_table);
	get_function_table_from_plugin(m_desc, INTERFACE_VERSION, meta_table.pfnGetEntityAPI2_Post, "GetEntityAPI2_Post", m_dllapi_post_table);

	// Look for old-style API in plugin, if API2 interface wasn't found.
	if (!m_dllapi_table && !m_dllapi_post_table) {
		get_function_table_from_plugin_old(m_desc, INTERFACE_VERSION, meta_table.pfnGetEntityAPI, "GetEntityAPI", m_dllapi_table);
		get_function_table_from_plugin_old(m_desc, INTERFACE_VERSION, meta_table.pfnGetEntityAPI_Post, "GetEntityAPI_Post", m_dllapi_post_table);
	}

	get_function_table_from_plugin(m_desc, ENGINE_INTERFACE_VERSION, meta_table.pfnGetEngineFunctions, "GetEngineFunctions", m_engine_table);
	get_function_table_from_plugin(m_desc, ENGINE_INTERFACE_VERSION, meta_table.pfnGetEngineFunctions_Post, "GetEngineFunctions_Post", m_engine_post_table);

	m_time_loaded = time(nullptr);
	return true;
}

// Unload a plugin.  Check time, detach.
bool MPlugin::unload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason, PL_UNLOAD_REASON real_reason)
{
	if (!check_input())
	{
		return false;
	}
	if (m_status < PL_RUNNING)
	{
		if (reason != PNL_CMD_FORCED && reason != PNL_RELOAD)
		{
			META_ERROR("dll: Not unloading plugin '%s'; already unloaded (status=%s)", m_desc, str_status());
			return false;
		}
	}
	if (m_action != PA_UNLOAD && m_action != PA_RELOAD)
	{
		META_WARNING("dll: Not unloading plugin '%s'; not marked for unload (action=%s)", m_desc, str_action());
		return false;
	}

	// Are we allowed to detach this plugin at this time?
	// If forcing unload, we disregard when plugin wants to be unloaded.
	if (m_info && m_info->unloadable < now)
	{
		if (reason == PNL_CMD_FORCED)
		{
			META_DEBUG(2, "dll: Forced unload plugin '%s' overriding allowed times: allowed=%s; now=%s", m_desc, str_unloadable(), str_loadtime(now, SL_SIMPLE));
		}
		else
		{
			if (m_info->unloadable > PT_STARTUP)
			{
				META_DEBUG(2, "dll: Delaying unload plugin '%s'; can't detach now: allowed=%s; now=%s", m_desc, str_unloadable(), str_loadtime(now, SL_SIMPLE));
				// caller should give message to user
				// try to unload again at next opportunity
				return false;
			}
			
			META_DEBUG(2, "dll: Failed unload plugin '%s'; can't detach now: allowed=%s; now=%s", m_desc, str_unloadable(), str_loadtime(now, SL_SIMPLE));
			// don't try to unload again later
			m_action = PA_NONE;
			return false;
		}
	}

	// If unloading during map, then I'd like to call plugin's
	// ServerDeactivate.  However, I don't want to do this until I start
	// calling ServerActivate when loading during map, since the SDK
	// indicates these two routines should match call for call.

	// detach plugin
	if (!detach(now, reason))
	{
		if (reason == PNL_RELOAD)
		{
			META_DEBUG(2, "dll: Reload plugin '%s' overriding failed detach", m_desc);
		}
		else if (reason == PNL_CMD_FORCED)
		{
			META_DEBUG(2, "dll: Forced unload plugin '%s' overriding failed detach");
		}
		else
		{
			META_WARNING("dll: Failed to detach plugin '%s'; ", m_desc);
			return false;
		}
	}

	g_plugins->clear_source_plugin_index(m_index);

	// successful detach, or forced unload

	// Unmark registered commands for this plugin (by index number).
	g_regCmds->remove(m_index);
	// Unmark registered cvars for this plugin (by index number).
	g_regCvars->disable(m_index);

	// Close the file.  Note: after this, attempts to reference any memory
	// locations in the file will produce a segfault.
	if (!m_sys_module.unload())
	{
		// If DLL cannot be closed, OS is badly broken or we are giving invalid handle.
		// So we don't return here but instead remove plugin from our listings.
		META_WARNING("dll: Couldn't close plugin file '%s': %s", m_file, "invalid handle");
	}

	if (m_action == PA_UNLOAD)
	{
		m_status = PL_EMPTY;
		clear();
	}
	else if (m_action == PA_RELOAD)
	{
		m_status = PL_VALID;
		m_action = PA_LOAD;
		clear();
	}

	META_LOG("dll: Unloaded plugin '%s' for reason '%s'", m_desc, str_reason(reason, real_reason));
	return true;
}

// Inform plugin we're going to unload it.
bool MPlugin::detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	int ret;
	META_DETACH_FN pfn_detach;

	// If we have no handle, i.e. no dll loaded, we return true because the
	// dll is obviously detached. We shouldn't call DLSYM() with a nullptr
	// handle since this will DLSYM() ourself.
	if (!m_sys_module.gethandle())
		return true;

	if (!(pfn_detach = (META_DETACH_FN)m_sys_module.getsym("Meta_Detach")))
	{
		META_ERROR("dll: Error detach plugin '%s': Couldn't find Meta_Detach(): %s", m_desc, "function not found");
		// caller will dlclose()
		return false;
	}

	ret = pfn_detach(now, reason);
	if (ret != TRUE)
	{
		META_ERROR("dll: Failed detach plugin '%s': Error from Meta_Detach(): %d", m_desc, ret);
		return false;
	}

	META_DEBUG(6, "dll: Plugin '%s': Called Meta_Detach() successfully", m_desc);
	return true;
}

// Reload a plugin; unload and load again.
bool MPlugin::reload(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	if (!check_input())
	{
		return false;
	}
	// Are we allowed to load this plugin at this time?
	// If we cannot load the plugin after unloading it, we keep it.
	if (m_info && m_info->loadable < now)
	{
		if (m_info->loadable > PT_STARTUP)
		{
			META_DEBUG(2, "dll: Delaying reload plugin '%s'; would not be able to reattach now: allowed=%s; now=%s", m_desc, str_loadable(), str_loadtime(now, SL_SIMPLE));
			// caller should give message to user
			// try to reload again at next opportunity
			return false;
		}
		else
		{
			META_DEBUG(2, "dll: Failed reload plugin '%s'; would not be able to reattach now: allowed=%s; now=%s", m_desc, str_loadable(), str_loadtime(now, SL_SIMPLE));
			// don't try to reload again later
			m_action = PA_NONE;
			return false;
		}
	}

	if (m_status < PL_RUNNING)
	{
		META_WARNING("dll: Plugin '%s' isn't running; Forcing unload plugin for reloading", m_desc);
		reason = PNL_RELOAD;
	}

	if (!unload(now, reason, reason))
	{
		META_WARNING("dll: Failed to unload plugin '%s' for reloading", m_desc);
		return false;
	}

	if (!load(now))
	{
		META_WARNING("dll: Failed to reload plugin '%s' after unloading", m_desc);
		return false;
	}

	return true;
}

// Pause a plugin; temporarily disabled for API routines.
bool MPlugin::pause()
{
	if (m_status == PL_PAUSED)
	{
		META_ERROR("Not pausing plugin '%s'; already paused", m_desc);
		return false;
	}
	if (m_status != PL_RUNNING)
	{
		META_ERROR("Cannot pause plugin '%s'; not currently running (status=%s)", m_desc, str_status());
		return false;
	}

	// are we allowed to pause this plugin?
	if (m_info->unloadable < PT_ANYPAUSE)
	{
		META_ERROR("Cannot pause plugin '%s'; not allowed by plugin (allowed=%s)", m_desc, str_unloadable());
		m_action = PA_NONE;
		return false;
	}

	m_status = PL_PAUSED;
	META_LOG("Paused plugin '%s'", m_desc);
	return true;
}

// Unpause a plugin.
bool MPlugin::unpause()
{
	if (m_status != PL_PAUSED)
	{
		META_ERROR("Cannot unpause plugin '%s'; not currently paused (status=%s)", m_desc, str_status());
		return false;
	}

	m_status = PL_RUNNING;
	META_LOG("Unpaused plugin '%s'", m_desc);
	return true;
}

// Retry pending action, presumably from a previous failure.
bool MPlugin::retry(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	if (m_action == PA_LOAD)
		return load(now);
	if (m_action == PA_ATTACH)
		return load(now);
	if (m_action == PA_UNLOAD)
		return unload(now, reason, reason);
	if (m_action == PA_RELOAD)
		return reload(now, reason);

	META_ERROR("No pending action to retry for plugin '%s'; (status=%s, action=%s)", m_desc, str_status(), str_action());
	return false;
}

// Clear a plugin (it failed a previous action and should be
// removed from the list, or it's being unloaded).
bool MPlugin::clear(void)
{
	if (m_status != PL_FAILED && m_status != PL_BADFILE && m_status != PL_EMPTY && m_status != PL_OPENED)
	{
		META_ERROR("Cannot clear plugin '%s'; not marked as failed, empty, or open (status=%s)", m_desc, str_status());
		return false;
	}
	// If file is open, close the file.  Note: after this, attempts to
	// reference any memory locations in the file will produce a segfault.
	if (!m_sys_module.unload())
	{
		META_ERROR("dll: Couldn't close plugin file '%s': %s", m_file, "invalid handle");
		m_status = PL_FAILED;
		return false;
	}

	if (m_gamedll_funcs.dllapi_table) Q_free(m_gamedll_funcs.dllapi_table);
	if (m_gamedll_funcs.newapi_table) Q_free(m_gamedll_funcs.newapi_table);
	if (m_dllapi_table) Q_free(m_dllapi_table);
	if (m_dllapi_post_table) Q_free(m_dllapi_post_table);
	if (m_newapi_table) Q_free(m_newapi_table);
	if (m_newapi_post_table) Q_free(m_newapi_post_table);
	if (m_engine_table) Q_free(m_engine_table);
	if (m_engine_post_table) Q_free(m_engine_post_table);

	m_status = PL_EMPTY;
	m_action = PA_NULL;
	m_info = nullptr;
	m_time_loaded = 0;
	m_dllapi_table = nullptr;
	m_dllapi_post_table = nullptr;
	m_newapi_table = nullptr;
	m_newapi_post_table = nullptr;
	m_engine_table = nullptr;
	m_engine_post_table = nullptr;
	m_gamedll_funcs.dllapi_table = nullptr;
	m_gamedll_funcs.newapi_table = nullptr;
	m_source_plugin_index = 0;
	m_unloader_index = 0;
	m_is_unloader = false;

	return true;
}

template<typename table_t, typename info_t>
void show_table(const char* table_name, table_t* table, info_t* info_begin, bool post)
{
	if (table) {
		META_CONS("%s functions:", table_name);

		size_t count = 0;

		for (auto n = info_begin; n->name[0] != '\0'; n++) {
			if (*(size_t *)(size_t(table) + n->offset)) {
				META_CONS("   %s%s", Q_strstr(n->name, "::") + 2, post ? "_Post" : "");
				count++;
			}
		}

		META_CONS("%d functions (%s)", count, table_name);
	}
	else
		META_CONS("No %s functions.", table_name);
}

// List information about plugin to console.
void MPlugin::show()
{
	char *cp, *tstr;
	const int width = 13;

	META_CONS("%*s: %s", width, "name", m_info ? m_info->name : "(nil)");
	META_CONS("%*s: %s", width, "desc", m_desc);
	META_CONS("%*s: %s", width, "status", str_status());
	META_CONS("%*s: %s", width, "action", str_action());
	META_CONS("%*s: %s", width, "filename", m_filename);
	META_CONS("%*s: %s", width, "file", m_file);
	META_CONS("%*s: %s", width, "pathname", m_pathname);
	META_CONS("%*s: %d", width, "index", m_index);
	META_CONS("%*s: %s", width, "source", str_source());
	META_CONS("%*s: %s", width, "loadable", str_loadable(SL_ALLOWED));
	META_CONS("%*s: %s", width, "unloadable", str_unloadable(SL_ALLOWED));
	META_CONS("%*s: %s", width, "version", m_info ? m_info->version : "(nil)");
	META_CONS("%*s: %s", width, "date", m_info ? m_info->date : "(nil)");
	META_CONS("%*s: %s", width, "author", m_info ? m_info->author : "(nil)");
	META_CONS("%*s: %s", width, "url", m_info ? m_info->url : "(nil)");
	META_CONS("%*s: %s", width, "logtag", m_info ? m_info->logtag : "(nil)");
	META_CONS("%*s: %s", width, "ifvers", m_info ? m_info->ifvers : "(nil)");

	// ctime() includes newline at EOL
	tstr = ctime(&m_time_loaded);
	if ((cp = Q_strchr(tstr, '\n')))
		*cp = '\0';

	META_CONS("%*s: %s", width, "last loaded", tstr);
	// XXX show file time ?

	show_table("DLLAPI", m_dllapi_table, &g_dllapi_info.pfnGameInit, false);
	show_table("DLLAPI Post", m_dllapi_post_table, &g_dllapi_info.pfnGameInit, true);

	show_table("NEWAPI", m_newapi_table, &g_newapi_info.pfnOnFreeEntPrivateData, false);
	show_table("NEWAPI Post", m_newapi_post_table, &g_newapi_info.pfnOnFreeEntPrivateData, true);

	show_table("Engine", m_engine_table, &g_engineapi_info.pfnPrecacheModel, false);
	show_table("Engine Post", m_engine_post_table, &g_engineapi_info.pfnPrecacheModel, true);

	g_regCmds->show(m_index);
	g_regCvars->show(m_index);

	if (g_plugins->found_child_plugins(m_index))
		g_plugins->show(m_index);
	else
		META_CONS("No child plugins.");
}

// Check whether the file on disk looks like it's been updated since we
// last loaded the plugin.
bool MPlugin::newer_file() const
{
	struct stat st;
	time_t file_time;

	if (stat(m_pathname, &st) != 0) {
		META_ERROR("ini: Skipping plugin, couldn't stat file '%s': %s", m_pathname, strerror(errno));
		return false;
	}

	file_time = st.st_ctime > st.st_mtime ? st.st_ctime : st.st_mtime;
	META_DEBUG(5, "newer_file? file=%s; load=%d, file=%d; ctime=%d, mtime=%d", m_file, m_time_loaded, file_time, st.st_ctime, st.st_mtime);
	if (file_time > m_time_loaded)
		return true;
	
	return false;
}

// Return a string describing status of plugin.
// SIMPLE is the default.
// SHOW is max 4 chars, for "show" output.
const char *MPlugin::str_status(STR_STATUS fmt) const
{
	switch (m_status)
	{
	case PL_EMPTY:
		if (fmt == ST_SHOW) return "empt";
		else return "empty";
	case PL_VALID:
		if (fmt == ST_SHOW) return"info";
		else return "valid";
	case PL_BADFILE:
		if (fmt == ST_SHOW) return "badf";
		else return "badfile";
	case PL_OPENED:
		if (fmt == ST_SHOW) return "open";
		else return "opened";
	case PL_FAILED:
		if (fmt == ST_SHOW) return "fail";
		else return "failed";
	case PL_RUNNING:
		if (fmt == ST_SHOW) return "RUN";
		else return "running";
	case PL_PAUSED:
		if (fmt == ST_SHOW) return "PAUS";
		else return "paused";
	default:
		if (fmt == ST_SHOW) return UTIL_VarArgs("UNK%d", m_status);
		return UTIL_VarArgs("unknown (%d)", m_status);
	}
}

// Return a string (one word) describing requested action for plugin.
// SIMPLE is the default.
// SHOW is max 4 chars, for "show" output.
const char *MPlugin::str_action(STR_ACTION fmt) const
{
	switch (m_action)
	{
	case PA_NULL:
		if (fmt == SA_SHOW) return "nullptr";
		else return "nullptr";
	case PA_NONE:
		if (fmt == SA_SHOW) return " -  ";
		else return "none";
	case PA_KEEP:
		if (fmt == SA_SHOW) return "keep";
		else return "keep";
	case PA_LOAD:
		if (fmt == SA_SHOW) return "load";
		else return "load";
	case PA_ATTACH:
		if (fmt == SA_SHOW) return "atch";
		else return "attach";
	case PA_UNLOAD:
		if (fmt == SA_SHOW) return "unld";
		else return "unload";
	case PA_RELOAD:
		if (fmt == SA_SHOW) return "relo";
		else return "reload";
	default:
		if (fmt == SA_SHOW) return UTIL_VarArgs("UNK%d", m_action);
		else return UTIL_VarArgs("unknown (%d)", m_action);
	}
}

// Return a string describing given plugin loadtime.
// SIMPLE is the default.
// SHOW is max 3 chars, for "show" output.
// ALLOWED is in terms of when it's allowed to load/unload.
// NOW is to describe current situation of load/unload attempt.
const char *MPlugin::str_loadtime(PLUG_LOADTIME ptime, STR_LOADTIME fmt)
{
	static const char *rPrintLoadTime[][26] = {
		// SL_SIMPLE	// SL_SHOW	// SL_ALLOWED			// SL_NOW
		{ "never",	"Never",	"never",			"never" },			// PT_NEVER
		{ "startup",	"Start",	"at server startup",		"during server startup" },	// PT_STARTUP
		{ "changelevel","Chlvl",	"at changelevel",		"during changelevel" },		// PT_CHANGELEVEL
		{ "anytime",	"ANY",		"at any time",			"during map" },			// PT_ANYTIME
		{ "pausable",	"Pause",	"at any time, and pausable",	"for requested pause" },	// PT_ANYPAUSE
	};

	if (ptime >= PT_NEVER || ptime <= PT_ANYPAUSE)
		return rPrintLoadTime[ptime][fmt];

	if (fmt == SL_SHOW)
		return UTIL_VarArgs("UNK-%d", ptime);

	return UTIL_VarArgs("unknown (%d)", ptime);
}

// Return a string describing why a plugin is to be unloaded.
const char *MPlugin::str_reason(PL_UNLOAD_REASON preason, PL_UNLOAD_REASON preal_reason) const
{
	char buf[128];

	if (preason == PNL_PLUGIN)
		preason = PNL_NULL;
	else if (preason == PNL_PLG_FORCED)
		preason = PNL_NULL;

	switch (preal_reason)
	{
	case PNL_NULL:
		return "null";
	case PNL_INI_DELETED:
		return "deleted from ini file";
	case PNL_FILE_NEWER:
		return "file on disk is newer";
	case PNL_COMMAND:
		return "server command";
	case PNL_CMD_FORCED:
		return "forced by server command";
	case PNL_PLUGIN:
	{
		Q_strncpy(buf, str_reason(PNL_NULL, preason), sizeof buf - 1);
		buf[sizeof buf - 1] = '\0';

		return UTIL_VarArgs("%s (request from plugin[%d])", buf, m_unloader_index);
	}
	case PNL_PLG_FORCED:
	{
		Q_strncpy(buf, str_reason(PNL_NULL, preason), sizeof buf - 1);
		buf[sizeof buf - 1] = '\0';

		return UTIL_VarArgs("%s (forced request from plugin[%d])", buf, m_unloader_index);
	}
	case PNL_RELOAD:
		return "reloading";
	default:
		return UTIL_VarArgs("unknown (%d)", preal_reason);
	}
}

// Return a string describing how the plugin was loaded.
const char *MPlugin::str_source(STR_SOURCE fmt) const
{
	switch (m_source)
	{
	case PS_INI:
		if (fmt == SO_SHOW) return "ini";
		else return "ini file";
	case PS_CMD:
		if (fmt == SO_SHOW) return "cmd";
		else return "console command";
	case PS_PLUGIN:
		if (m_source_plugin_index <= 0)
		{
			if (fmt == SO_SHOW) return "plUN";
			else return "unloaded plugin";
		}
		else
		{
			if (fmt == SO_SHOW) return UTIL_VarArgs("pl%d", m_source_plugin_index);
			else return UTIL_VarArgs("plugin [%s]", g_plugins->find(m_source_plugin_index)->m_desc);
		}
	default:
		if (fmt == SO_SHOW) return UTIL_VarArgs("UNK%d", m_source);
		else return UTIL_VarArgs("unknown (%d)", m_source);
	}
}
