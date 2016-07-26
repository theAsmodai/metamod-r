#include "precompiled.h"

MPlayer::MPlayer() : isQueried(mFALSE)
{
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void MPlayer::set_cvar_query(const char *cvar)
{
	// Do not allow NULL as queried cvar since we use this as
	// return value in is_querying_cvar as indication if a
	// client cvar is queried.
	if (!cvar)
	{
		meta_errno = ME_ARGUMENT;
		return;
	}

	isQueried = mTRUE;
	Q_strncpy(cvarName, cvar, sizeof cvarName - 1);
	cvarName[sizeof cvarName - 1] = '\0';
}

// Unmark player as querying a client cvar
void MPlayer::clear_cvar_query(const char *cvar)
{
	isQueried = mFALSE;
	cvarName[0] = '\0';
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
const char *MPlayer::is_querying_cvar(void) const
{
	if (isQueried)
	{
		return cvarName;
	}

	return nullptr;
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is NULL
void MPlayerList::set_player_cvar_query(const edict_t *pEntity, const char *cvar)
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients)
		players[indx].set_cvar_query(cvar);
}

// Unmark player as querying a client cvar
void MPlayerList::clear_player_cvar_query(const edict_t *pEntity, const char *cvar)
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients)
		players[indx].clear_cvar_query(cvar);
}

void MPlayerList::clear_all_cvar_queries(void)
{
	for (int indx = 1; indx <= gpGlobals->maxClients; indx++)
	{
		players[indx].clear_cvar_query();
	}
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
// meta_errno values:
//  - ME_NOTFOUND  invalid entity
const char *MPlayerList::is_querying_cvar(const edict_t *pEntity) const
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients)
		return players[indx].is_querying_cvar();

	RETURN_ERRNO(NULL, ME_NOTFOUND);
}
