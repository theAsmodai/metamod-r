#pragma once

#include "types_meta.h"

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

class MConfig {
public:
	MConfig();

	int debuglevel;		// to use for meta_debug
	char *plugins_file;	// ie metamod.ini, plugins.ini
	char *exec_cfg;		// ie metaexec.cfg, exec.cfg

	void init(option_t *global_options);
	mBOOL load(const char *filename);
	mBOOL set(const char *key, const char *value);
	void show() const;

private:
	option_t *list;
	char *filename;

	option_t *find(const char *lookup) const;
	static mBOOL set(option_t *setp, const char *value);
	// Private; to satisfy -Weffc++ "has pointer data members but does
	// not override" copy/assignment constructor.
	void operator=(const MConfig &src);
	MConfig(const MConfig &src);
};
