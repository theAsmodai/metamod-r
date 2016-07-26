#pragma once

#include "types_meta.h"

// Info on an individual player
class MPlayer
{
public:
	MPlayer();

	void set_cvar_query(const char *cvar);			// mark this player as querying a client cvar
	void clear_cvar_query(const char *cvar = nullptr);	// unmark this player as querying a client cvar
	const char *is_querying_cvar() const;			// check if a player is querying a cvar. returns

private:
	mBOOL isQueried;					// is this player currently queried for a cvar value
	char cvarName[64];					// name of the cvar if getting queried
};

// A list of players. The number of max players is fixed and small enough
// to use an array.
class MPlayerList
{
public:
	void set_player_cvar_query(const edict_t *pEntity, const char *cvar);
	void clear_player_cvar_query(const edict_t *pEntity, const char *cvar = nullptr);
	void clear_all_cvar_queries();
	const char *is_querying_cvar(const edict_t *pEntity) const;

private:
	int maxplayers = 32;
	MPlayer players[MAX_CLIENTS + 1];		// array of players
};
