#pragma once

// Max length of line in config file.
#define MAX_CONF_LEN	1024

// Supported config value-types.
enum cf_type_t
{
	CF_NONE = 0,
	CF_INT,
	CF_BOOL,
	CF_STR,
	CF_PATH,
};

struct option_t
{
	char *name;		// option name
	cf_type_t type;		// option type
	void *dest;		// addr of destination variable, or handler function
	char *init;		// initial value, as a string, just as config file would
};

class MConfig
{
public:
	MConfig();

	void init(option_t *global_options);
	bool load(const char *filename);
	bool set(const char *key, const char *value) const;
	void show() const;

	int m_debuglevel;		// to use for meta_debug
	char *m_plugins_file;	// ie metamod.ini, plugins.ini
	char *m_exec_cfg;		// ie metaexec.cfg, exec.cfg

private:
	option_t *m_list;
	char *m_filename;

	option_t *find(const char *lookup) const;
	static bool set(option_t *setp, const char *value);
	// Private; to satisfy -Weffc++ "has pointer data members but does
	// not override" copy/assignment constructor.
	void operator=(const MConfig &src);
	MConfig(const MConfig &src);
};
