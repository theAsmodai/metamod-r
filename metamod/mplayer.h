// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mplayer.h - class to keep info about a player and a class listing all
//             players

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

#ifndef INCLUDE_METAMOD_PLAYER_H
#define INCLUDE_METAMOD_PLAYER_H

#include "plinfo.h"	   // plugin_info_t, etc
#include "mutil.h"         // query_callback_t
#include "types_meta.h"    // mBOOL
#include "new_baseclass.h" // class_metamod_new


// Numbers of players limit set by the engine
#define MAX_PLAYERS 32


// Info on an individual player
class MPlayer : public class_metamod_new
{
private:
	mBOOL isQueried;                         // is this player currently queried for a cvar value
	char *cvarName;                          // name of the cvar if getting queried
	
	MPlayer (const MPlayer&) DLLINTERNAL;
	MPlayer& operator=(const MPlayer&) DLLINTERNAL; 


public:
	MPlayer() DLLINTERNAL;
	~MPlayer() DLLINTERNAL;
	void        DLLINTERNAL set_cvar_query(const char *cvar);            // mark this player as querying a client cvar
	void        DLLINTERNAL clear_cvar_query(const char *cvar=NULL);     // unmark this player as querying a client cvar
	const char *DLLINTERNAL is_querying_cvar(void);                      // check if a player is querying a cvar. returns
	                                                                     //   NULL if not or the name of the cvar
};



// A list of players. The number of max players is fixed and small enough
// to use an array.
class MPlayerList
{
private:
	enum { NUM_SLOTS = MAX_PLAYERS + 1 };

	MPlayer players[NUM_SLOTS];              // array of players

	
public:
	void        DLLINTERNAL set_player_cvar_query(const edict_t *pEntity, const char *cvar);
	void        DLLINTERNAL clear_player_cvar_query(const edict_t *pEntity, const char *cvar=NULL);
	void        DLLINTERNAL clear_all_cvar_queries(void);
	const char *DLLINTERNAL is_querying_cvar(const edict_t *pEntity);
};


#endif /* INCLUDE_METAMOD_PLAYER_H */ 
