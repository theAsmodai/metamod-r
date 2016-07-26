// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// linkent.cpp - export entities from mod "games" back to the HL engine

/*
 * Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game g_engine ("HL
 *    g_engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL g_engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include "precompiled.h"

// Function to perform common code of LINK_ENTITY_TO_GAME, rather than
// duplicating the code in ~2000 expanded macros.  Based on code from Jussi
// Kivilinna <kijuhe00@students.oamk.fi>.
void do_link_ent(ENTITY_FN *pfnEntity, int *missing, const char *entStr, entvars_t *pev)
{;
	if(*missing) {
		META_DEBUG(9, ("Skipping entity '%s'; was previously found missing", entStr));
		return;
	}
	if(!*pfnEntity) {
		META_DEBUG(9, ("Looking up game entity '%s'", entStr));
		*pfnEntity = (ENTITY_FN) DLSYM(GameDLL.handle, entStr);
	}
	if(!*pfnEntity) {
		META_ERROR("Couldn't find game entity '%s' in game DLL '%s': %s", entStr, GameDLL.name, DLERROR());
		*missing=1;
		return;
	}
	META_DEBUG(8, ("Linking game entity '%s'", entStr));
	(*pfnEntity)(pev);
}
