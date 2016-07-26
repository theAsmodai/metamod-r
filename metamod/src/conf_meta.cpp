#include "precompiled.h"

MConfig::MConfig()
	: list(nullptr), filename(nullptr), debuglevel(0),
	plugins_file(nullptr), exec_cfg(nullptr)
{
}

// Initialize default values from the stored options struct.  Has to happen
// _after_ constructor, so that all the fields are allocated (d'oh).
void MConfig::init(option_t* global_options)
{
	list = global_options;
	for (auto optp = list; optp->name; optp++)
		set(optp, optp->init);
}

option_t *MConfig::find(const char* lookup) const
{
	for (auto optp = list; optp->name; optp++)
	{
		if (!Q_strcmp(optp->name, lookup)) {
			return optp;
		}
	}
	
	RETURN_ERRNO(NULL, ME_NOTFOUND);
}

mBOOL MConfig::set(const char* key, const char* value)
{
	option_t* optp = find(key);
	if (optp)
		return set(optp, value);

	RETURN_ERRNO(mFALSE, ME_NOTFOUND);
}

mBOOL MConfig::set(option_t* setp, const char* setstr)
{
	char pathbuf[PATH_MAX ];
	int* optval = (int *) setp->dest;
	char** optstr = (char **) setp->dest;
	// cvar_t *optcvar = (cvar_t *) setp->dest;
	// SETOPT_FN optcmd = (SETOPT_FN) setp->dest;

	if (!setstr)
		return mTRUE;

	switch (setp->type)
	{
	case CF_INT:
		if (!isdigit(setstr[0]))
		{
			META_ERROR("option '%s' invalid format '%s'", setp->name, setstr);
			RETURN_ERRNO(mFALSE, ME_FORMAT);
		}
		*optval = Q_atoi(setstr);
		META_DEBUG(3, ("set config int: %s = %d", setp->name, *optval));
		break;
	case CF_BOOL:
		if (is_yes(setstr))
		{
			*optval = TRUE;
		}
		else if (is_no(setstr))
		{
			*optval = FALSE;
		}
		else
		{
			META_ERROR("option '%s' invalid format '%s'", setp->name,
			           setstr);
			RETURN_ERRNO(mFALSE, ME_FORMAT);
		}
		META_DEBUG(3, ("set config bool: %s = %s", setp->name, *optval ? "true" : "false"));
		break;
	case CF_STR:
		if (*optstr)
			Q_free(*optstr);
		*optstr = Q_strdup(setstr);
		META_DEBUG(3, ("set config string: %s = %s", setp->name, *optstr));
		break;
	case CF_PATH:
		if (*optstr)
			Q_free(*optstr);
		full_gamedir_path(setstr, pathbuf);
		*optstr = Q_strdup(pathbuf);
		META_DEBUG(3, ("set config path: %s = %s", setp->name, *optstr));
		break;
	default:
		META_ERROR("unrecognized config type '%d'", setp->type);
		RETURN_ERRNO(mFALSE, ME_ARGUMENT);
	}

	return mTRUE;
}

mBOOL MConfig::load(const char* fn)
{
	FILE* fp;
	char loadfile[PATH_MAX ];
	char line[MAX_CONF_LEN];
	char *optname, *optval;
	option_t* optp;
	int ln;

	// Make full pathname (from gamedir if relative, collapse "..",
	// backslashes, etc).
	full_gamedir_path(fn, loadfile);

	fp = fopen(loadfile, "r");
	if (!fp)
	{
		META_ERROR("unable to open config file '%s': %s", loadfile, strerror(errno));
		RETURN_ERRNO(mFALSE, ME_NOFILE);
	}

	META_DEBUG(2, ("Loading from config file: %s", loadfile));
	for (ln = 1; !feof(fp) && fgets(line, sizeof(line), fp); ln++)
	{
		if (line[0] == '#' || line[0] == ';' || !Q_strncmp(line, "//", 2))
			continue;

		if (!(optname = strtok(line, " \t\r\n")))
		{
			META_ERROR("'%s' line %d: bad config format: missing option", loadfile, ln);
			continue;
		}
		
		if (!(optval = strtok(NULL, "\r\n")))
		{
			META_ERROR("'%s' line %d: bad config format: missing value", loadfile, ln);
			continue;
		}

		if (!(optp = find(optname)))
		{
			META_ERROR("'%s' line %d: unknown option name '%s'", loadfile, ln, optname);
			continue;
		}

		if (!set(optp, optval))
		{
			META_ERROR("'%s' line %d: unable to set option '%s' value '%s'", loadfile, ln, optname, optval);
			continue;
		}
	}

	filename = Q_strdup(loadfile);
	fclose(fp);
	return mTRUE;
}

void MConfig::show() const
{
	META_CONS("Config options from localinfo and %s:", filename);

	for (auto optp = list; optp->name; optp++)
	{
		int *optval = (int *)optp->dest;
		char **optstr = (char **)optp->dest;

		// cvar_t *optcvar = (cvar_t *) optp->dest;
		// SETOPT_FN optcmd = (SETOPT_FN) optp->dest;
		switch (optp->type)
		{
		case CF_INT:
			printf("   %-20s\t%d\n", optp->name, *optval);
			break;
		case CF_BOOL:
			printf("   %-20s\t%s\n", optp->name, *optval ? "true" : "false");
			break;
		case CF_STR:
		case CF_PATH:
			printf("   %-20s\t%s\n", optp->name, *optstr ? *optstr : "");
			break;
		case CF_NONE:
			break;
		}
	}
}