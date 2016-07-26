// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mplayer.cpp - methods of individual player (class MPlayer) and 
//               list of players (class MPlayerList).

/*
 * Copyright (c) 2005 Will Day <willday@hpgx.net>
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

// Constructor
MPlayer::MPlayer() : isQueried(mFALSE)
{
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void MPlayer::set_cvar_query(const char* cvar)
{
	// Do not allow NULL as queried cvar since we use this as
	// return value in is_querying_cvar as indication if a
	// client cvar is queried.
	if (!cvar) {
		meta_errno = ME_ARGUMENT;
		return;
	}

	isQueried = mTRUE;
	strncpy(cvarName, cvar, sizeof cvarName - 1);
	cvarName[sizeof cvarName - 1] = '\0';
}

// Unmark player as querying a client cvar
void MPlayer::clear_cvar_query(const char* /*cvar*/)
{
	isQueried = mFALSE;
	cvarName[0] = '\0';
}


// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
const char* MPlayer::is_querying_cvar(void) const
{
	if (isQueried) {
		return cvarName;
	}

	return NULL;
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void MPlayerList::set_player_cvar_query(const edict_t* pEntity, const char* cvar)
{
	int indx = ENTINDEX(pEntity);

	if (indx >= 1 && indx <= maxplayers)
		players[indx].set_cvar_query(cvar);
}

// Unmark player as querying a client cvar
void MPlayerList::clear_player_cvar_query(const edict_t* pEntity, const char* cvar)
{
	int indx = ENTINDEX(pEntity);

	if (indx >= 1 && indx <= maxplayers)
		players[indx].clear_cvar_query(cvar);
}

void MPlayerList::clear_all_cvar_queries(void)
{
	for (int indx = 1; indx <= maxplayers; indx++) {
		players[indx].clear_cvar_query();
	}
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
// meta_errno values:
//  - ME_NOTFOUND  invalid entity
const char* MPlayerList::is_querying_cvar(const edict_t* pEntity) const
{
	int indx = ENTINDEX(pEntity);

	if (indx >= 1 && indx <= maxplayers)
		return players[indx].is_querying_cvar();

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}
