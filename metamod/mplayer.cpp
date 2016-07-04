// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mplayer.cpp - methods of individual player (class MPlayer) and 
//               list of players (class MPlayerList).

/*
 * Copyright (c) 2005-2006 Will Day <willday@hpgx.net>
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
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <string.h>         // strdup()

#include <extdll.h>			// always

#include "mplayer.h"		// me
#include "sdk_util.h"       // ENTINDEX()
#include "metamod.h"        // gpGlobals


// Constructor
MPlayer::MPlayer()
	: isQueried(mFALSE),
	  cvarName(NULL)
{
}


// Destructor
MPlayer::~MPlayer()
{
	if(cvarName) {
		free(cvarName);
	}
}


// Copy constructor
MPlayer::MPlayer(const MPlayer& rhs)
	: isQueried(rhs.isQueried),
	  cvarName(NULL)
{
	if(rhs.cvarName) {
		cvarName = strdup(rhs.cvarName);
	}
}


// Assignment operator
MPlayer& DLLINTERNAL MPlayer::operator=(const MPlayer& rhs)
{
	isQueried = rhs.isQueried;

	if(cvarName) {
		free(cvarName);
	}

	cvarName = NULL;

	if(rhs.cvarName) {
		cvarName = strdup(rhs.cvarName);
	}

	return *this;
}


// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void DLLINTERNAL MPlayer::set_cvar_query(const char *cvar)
{
	// Do not allow NULL as queried cvar since we use this as
	// return value in is_querying_cvar as indication if a
	// client cvar is queried.
	if(!cvar) {
		meta_errno = ME_ARGUMENT;
		return;
	}

	isQueried = mTRUE;
	if(cvarName) {
		free(cvarName);
	}

	cvarName = strdup(cvar);
}


// Unmark player as querying a client cvar
void DLLINTERNAL MPlayer::clear_cvar_query(const char* /*cvar*/)
{
	isQueried = mFALSE;
	if(cvarName) {
		free(cvarName);
		cvarName = NULL;
	}
}


// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
const char * DLLINTERNAL MPlayer::is_querying_cvar(void)
{
	if(isQueried) {
		return(cvarName);
	}

	return(NULL);
}



// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void DLLINTERNAL MPlayerList::set_player_cvar_query(const edict_t *pEntity, const char *cvar)
{
	int indx = ENTINDEX(const_cast<edict_t*>(pEntity));

	if(indx < 1 || indx >= MPlayerList::NUM_SLOTS)
		return;	//maybe output a message?

	players[indx].set_cvar_query(cvar);
}


// Unmark player as querying a client cvar
void DLLINTERNAL MPlayerList::clear_player_cvar_query(const edict_t *pEntity, const char *cvar)
{
	int indx = ENTINDEX(const_cast<edict_t*>(pEntity));

	if(indx < 1 || indx >= MPlayerList::NUM_SLOTS)
		return;	//maybe output a message?

	players[indx].clear_cvar_query(cvar);
}


void DLLINTERNAL MPlayerList::clear_all_cvar_queries(void)
{
	for(int indx=1; indx < MPlayerList::NUM_SLOTS; ++indx) {
		players[indx].clear_cvar_query();
	}
}



// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
// meta_errno values:
//  - ME_NOTFOUND  invalid entity
const char* DLLINTERNAL MPlayerList::is_querying_cvar(const edict_t *pEntity)
{
	int indx = ENTINDEX(const_cast<edict_t*>(pEntity));

	if(indx < 1 || indx > gpGlobals->maxClients) {
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}
 
	return(players[indx].is_querying_cvar());
}
