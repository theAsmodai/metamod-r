#include "precompiled.h"

// Constructor
MPlayer::MPlayer()
	: isQueried(mFALSE),
	  cvarName(nullptr)
{
}

// Destructor
MPlayer::~MPlayer()
{
	if (cvarName) {
		free(cvarName);
	}
}

// Copy constructor
MPlayer::MPlayer(const MPlayer& rhs)
	: isQueried(rhs.isQueried),
	  cvarName(nullptr)
{
	if (rhs.cvarName) {
		cvarName = strdup(rhs.cvarName);
	}
}

// Assignment operator
MPlayer& MPlayer::operator=(const MPlayer& rhs)
{
	isQueried = rhs.isQueried;

	if (cvarName) {
		free(cvarName);
	}
	cvarName = nullptr;
	if (rhs.cvarName) {
		cvarName = strdup(rhs.cvarName);
	}
	return *this;
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
	if (cvarName) {
		free(cvarName);
	}
	cvarName = strdup(cvar);
}


// Unmark player as querying a client cvar
void MPlayer::clear_cvar_query(const char* /*cvar*/)
{
	isQueried = mFALSE;
	if (cvarName)
	{
		free(cvarName);
		cvarName = nullptr;
	}
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
const char *MPlayer::is_querying_cvar()
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
	int indx = ENTINDEX(const_cast<edict_t *>(pEntity));
	if (indx < 1 || indx > gpGlobals->maxClients)
		return;	//maybe output a message?

	players[indx].set_cvar_query(cvar);
}

// Unmark player as querying a client cvar
void MPlayerList::clear_player_cvar_query(const edict_t *pEntity, const char *cvar)
{
	int indx = ENTINDEX(const_cast<edict_t *>(pEntity));
	if (indx < 1 || indx > gpGlobals->maxClients)
		return;	//maybe output a message?

	players[indx].clear_cvar_query(cvar);
}

void MPlayerList::clear_all_cvar_queries()
{
	for (int indx = 1; indx < gpGlobals->maxClients; ++indx)
	{
		players[indx].clear_cvar_query();
	}
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
// meta_errno values:
//  - ME_NOTFOUND  invalid entity
const char *MPlayerList::is_querying_cvar(const edict_t *pEntity)
{
	int indx = ENTINDEX(const_cast<edict_t *>(pEntity));
	if (indx < 1 || indx > gpGlobals->maxClients)
	{
		RETURN_ERRNO(nullptr, ME_NOTFOUND);
	}

	return players[indx].is_querying_cvar();
}
