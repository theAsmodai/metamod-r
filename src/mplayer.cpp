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
	if(!cvar)
	{
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
	if(cvarName)
	{
		free(cvarName);
		cvarName = NULL;
	}
}

// Check if a client cvar is queried for this player
// Returns NULL if not
// or the name of the cvar.
const char * DLLINTERNAL MPlayer::is_querying_cvar(void)
{
	if(isQueried)
	{
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
	for(int indx=1; indx < MPlayerList::NUM_SLOTS; ++indx)
	{
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
	if(indx < 1 || indx > gpGlobals->maxClients)
	{
		RETURN_ERRNO(NULL, ME_NOTFOUND);
	}
	return(players[indx].is_querying_cvar());
}
