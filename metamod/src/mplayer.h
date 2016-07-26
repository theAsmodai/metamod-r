// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mplayer.h - class to keep info about a player and a class listing all
//             players

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

#ifndef INCLUDE_METAMOD_PLAYER_H
#define INCLUDE_METAMOD_PLAYER_H

#include "types_meta.h"    // mBOOL

// Numbers of players limit set by the engine
#define MAX_PLAYERS 32

// Info on an individual player
class MPlayer
{
public:
	MPlayer();

	void        set_cvar_query(const char *cvar);            // mark this player as querying a client cvar
	void        clear_cvar_query(const char *cvar=NULL);     // unmark this player as querying a client cvar
	const char *is_querying_cvar(void) const;                      // check if a player is querying a cvar. returns
	          
private:
	mBOOL isQueried;						    // is this player currently queried for a cvar value
	char cvarName[64];                          // name of the cvar if getting queried
};

// A list of players. The number of max players is fixed and small enough
// to use an array.
class MPlayerList
{	
public:
	void        set_player_cvar_query(const edict_t *pEntity, const char *cvar);
	void        clear_player_cvar_query(const edict_t *pEntity, const char *cvar=NULL);
	void        clear_all_cvar_queries(void);
	const char *is_querying_cvar(const edict_t *pEntity) const;

private:
	int maxplayers = 32;
	MPlayer players[MAX_PLAYERS + 1];              // array of players
};

#endif /* INCLUDE_METAMOD_PLAYER_H */ 
