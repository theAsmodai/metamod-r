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
