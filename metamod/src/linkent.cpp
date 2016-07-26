#include "precompiled.h"

// Function to perform common code of LINK_ENTITY_TO_GAME, rather than
// duplicating the code in ~2000 expanded macros.  Based on code from Jussi
// Kivilinna <kijuhe00@students.oamk.fi>.
void do_link_ent(ENTITY_FN *pfnEntity, int *missing, const char *entStr, entvars_t *pev)
{
	if (*missing)
	{
		META_DEBUG(9, ("Skipping entity '%s'; was previously found missing", entStr));
		return;
	}

	if (!*pfnEntity)
	{
		META_DEBUG(9, ("Looking up game entity '%s'", entStr));
		*pfnEntity = (ENTITY_FN)DLSYM(GameDLL.handle, entStr);
	}

	if (!*pfnEntity)
	{
		META_ERROR("Couldn't find game entity '%s' in game DLL '%s': %s", entStr, GameDLL.name, DLERROR());
		*missing = 1;
		return;
	}

	META_DEBUG(8, ("Linking game entity '%s'", entStr));
	(*pfnEntity)(pev);
}
