#ifndef CONF_META_H
#define CONF_META_H

#include "types_meta.h"		// mBOOL
#include "new_baseclass.h"
#include "comp_dep.h"

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

//typedef mBOOL (*SETOPT_FN) (char *key, char *value);

typedef struct option_s {
	char *name;		// option name
	cf_type_t type;	// option type
	void *dest;		// addr of destination variable, or handler function
	char *init;		// initial value, as a string, just as config file would
} option_t;

class MConfig : public class_metamod_new {
	private:
		// data
		option_t *list;
		char *filename;
		// functions
		option_t * DLLINTERNAL find(const char *lookup);
		mBOOL DLLINTERNAL set(option_t *setp, const char *value);
		// Private; to satisfy -Weffc++ "has pointer data members but does
		// not override" copy/assignment constructor.
		void operator=(const MConfig &src);
		MConfig(const MConfig &src);
	public:
		// contructor
		MConfig(void) DLLINTERNAL;
		// data
		int debuglevel;		// to use for meta_debug
		char *gamedll;		// string if specified in config.ini
		char *plugins_file;	// ie metamod.ini, plugins.ini
		char *exec_cfg;		// ie metaexec.cfg, exec.cfg
		int autodetect;		// autodetection of gamedll (Metamod-All-Support patch)
		int clientmeta;         // control 'meta' client-command
		// functions
		void DLLINTERNAL init(option_t *global_options);
		mBOOL DLLINTERNAL load(const char *filename);
		mBOOL DLLINTERNAL set(const char *key, const char *value);
		void DLLINTERNAL show(void);
};

#endif /* CONF_META_H */
