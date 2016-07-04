#include <extdll.h>				// always
#include "osdep_p.h"				// is_gamedll, ...
#include "game_autodetect.h"			// me
#include "support_meta.h"			// full_gamedir_path,


// Search gamedir/dlls/*.dll for gamedlls
//TODO: add META_DEBUG
const char * DLLINTERNAL autodetect_gamedll(const gamedll_t *gamedll, const char *knownfn)
{
	static char buf[256];
	char dllpath[256];
	char fnpath[256];
	DIR *dir;
	struct dirent *ent;
	unsigned int fn_len;
	// Generate dllpath
	safevoid_snprintf(buf, sizeof(buf), "%s/dlls", gamedll->gamedir);
	if(!full_gamedir_path(buf, dllpath)) {
		//whine & return
		META_WARNING("GameDLL-Autodetection: Directory '%s' doesn't exist.", buf);
		return(0);
	}
	// Generate knownfn path
	safevoid_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, knownfn);
	// Check if knownfn exists and is valid gamedll
	if(is_gamedll(fnpath))
	{
		// knownfn exists and is loadable gamedll, return 0.
		return(0);
	}
	// Open directory
	if(!(dir = opendir(dllpath)))
	{
		//whine & return
		META_WARNING("GameDLL-Autodetection: Couldn't open directory '%s'.", dllpath);
		return(0);
	}
	while((ent = readdir(dir)) != 0)
	{
		fn_len = Q_strlen(ent->d_name);
		if(fn_len <= Q_strlen(PLATFORM_DLEXT))
		{
			// Filename is too short
			continue;
		}
		// Compare end of filename with PLATFORM_DLEXT
		if(!strcasematch(&ent->d_name[fn_len - Q_strlen(PLATFORM_DLEXT)], PLATFORM_DLEXT))
		{
			// File isn't dll
			continue;
		}
		// Exclude all metamods
		if(strncasematch(ent->d_name, "metamod", Q_strlen("metamod")))
		{
			continue;
		}
		// Exclude all bots
		STRNCPY(buf, ent->d_name, sizeof(buf));
		strlwr(buf);
		if(Q_strstr(buf, "bot."))
		{
			continue;
		}
#ifdef linux
		//bot_iX86.so, bot_amd64.so, bot_x86_64.so
		if(Q_strstr(buf, "bot_i") || Q_strstr(buf, "bot_amd64.so") || Q_strstr(buf, "bot_x86"))
		{
			continue;
		}
#endif
		// Generate full path
		safevoid_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, ent->d_name);
		// Check if dll is gamedll
		if(is_gamedll(fnpath))
		{
			META_DEBUG(8, ("is_gamedll(%s): ok.", fnpath));
			//gamedll detected
			STRNCPY(buf, ent->d_name, sizeof(buf));
			closedir(dir);
			return(buf);
		}
		META_DEBUG(8, ("is_gamedll(%s): failed.", fnpath));
	}
	//not found
	META_WARNING("GameDLL-Autodetection: Couldn't find gamedll in '%s'.", dllpath);
	closedir(dir);
	return(0);
}
