#include "precompiled.h"

MPlayer::MPlayer() : m_isQueried(false)
{
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is nullptr
void MPlayer::set_cvar_query(const char* cvar)
{
	// Do not allow nullptr as queried cvar since we use this as
	// return value in is_querying_cvar as indication if a
	// client cvar is queried.
	if (!cvar) {
		return;
	}

	m_isQueried = true;
	Q_strlcpy(g_cvarName, cvar);
}

// Unmark player as querying a client cvar
void MPlayer::clear_cvar_query(const char* cvar)
{
	m_isQueried = false;
	g_cvarName[0] = '\0';
}

// Check if a client cvar is queried for this player
// Returns nullptr if not
// or the name of the cvar.
const char* MPlayer::is_querying_cvar() const
{
	if (m_isQueried) {
		return g_cvarName;
	}

	return nullptr;
}

// Mark a player as querying a client cvar and stores the cvar name
// meta_errno values:
//  - ME_ARGUMENT  cvar is nullptr
void MPlayerList::set_player_cvar_query(const edict_t* pEntity, const char* cvar)
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients) {
		m_players[indx].set_cvar_query(cvar);
	}
}

// Unmark player as querying a client cvar
void MPlayerList::clear_player_cvar_query(const edict_t* pEntity, const char* cvar)
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients) {
		m_players[indx].clear_cvar_query(cvar);
	}
}

void MPlayerList::clear_all_cvar_queries()
{
	for (int indx = 1; indx <= gpGlobals->maxClients; indx++) {
		m_players[indx].clear_cvar_query();
	}
}

// Check if a client cvar is queried for this player
// Returns nullptr if not
// or the name of the cvar.
// meta_errno values:
//  - ME_NOTFOUND  invalid entity
const char* MPlayerList::is_querying_cvar(const edict_t* pEntity) const
{
	int indx = ENTINDEX(pEntity);
	if (indx >= 1 && indx <= gpGlobals->maxClients) {
		return m_players[indx].is_querying_cvar();
	}

	return nullptr;
}
