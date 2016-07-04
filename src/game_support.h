#ifndef GAME_SUPPORT_H
#define GAME_SUPPORT_H

#include "types_meta.h"		// mBOOL
#include "metamod.h"		// gamedll_t

// Information we have about each game/mod DLL.
typedef struct game_modinfo_s {
	const char *name;		// name (the game dir)
	const char *linux_so;		// filename of linux shared lib
	const char *win_dll;		// filename of win32 dll
	const char *desc;		// our long-name description
} game_modinfo_t;

typedef game_modinfo_t game_modlist_t[];

const DLLINTERNAL game_modinfo_t *lookup_game(const char *name);
mBOOL DLLINTERNAL setup_gamedll(gamedll_t *gamedll);

#endif /* GAME_SUPPORT_H */
