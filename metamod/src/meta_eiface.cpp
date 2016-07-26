
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// meta_eiface.cpp - wrapper for engine/dll interface

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
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


// ------------------------------------------------------------------------
// meta_new_dll_functions_t 
// ------------------------------------------------------------------------

// static member initialisation
int meta_new_dll_functions_t::sm_version = 0;


meta_new_dll_functions_t::meta_new_dll_functions_t(
	void (*_pfnOnFreeEntPrivateData)(edict_t*),
	void (*_pfnGameShutdown)(void),
	int (*_pfnShouldCollide)(edict_t*, edict_t*),
	void (*_pfnCvarValue)(const edict_t*, const char*),
	void (*_pfnCvarValue2)(const edict_t*, int, const char*, const char*)
)
{
	pfnOnFreeEntPrivateData = _pfnOnFreeEntPrivateData;
	pfnGameShutdown = _pfnGameShutdown;
	pfnShouldCollide = _pfnShouldCollide;
	pfnCvarValue = _pfnCvarValue;
	pfnCvarValue2 = _pfnCvarValue2;
}


void meta_new_dll_functions_t::copy_to(NEW_DLL_FUNCTIONS* _pFuncs)
{
	// This is where the magic happens. We check what version of the
	// NEW_DLL_FUNCTIONS interface the engine has and calculate the size of
	// that interface. Then we only copy the function pointers present in
	// that version over to the receiver, so that we do not overwrite his
	// memory with functions that he doesn't know of in his copy of the
	// struct.
	size_t size = get_size();

	if (0 == size) {
		// Ok, this is a real problem and should *not* happen.
		// We try to work with NEW_DLL_FUNCTIONS without knowing what
		// interface the attached engine uses. This means that the classes
		// defined herein are not used in the way they are meant to because
		// someone forgot to first create a HL_enginefuncs_t object and
		// initialise it with the pointers passed from the engine.
		// We treat this as a major developer error and bluntly exit the
		// whole process, assuming that this will never happen on a
		// production server as it should have been caught by the developer
		// during testing.
		//
		// We use a printf() to complain since we do not know if we have
		// already attached to the engine and can use its alerting
		// functions. This should be augemnted with a windows version
		// popping open a message box.
		fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fprintf(stderr, "ERROR: INTERNAL ERROR.\n");
		fprintf(stderr, "       Attempt to use meta_new_dll_functions_t without"
		              " initialised engine interface version!\n");
		fprintf(stderr, "       %s at %d\n", __FILE__, __LINE__);
		exit(1);
	}

	memcpy(_pFuncs, this, size);
}


int meta_new_dll_functions_t::determine_interface_version(void)
{
	// If the meta_enginefuncs_t::version is 0, i.e. has not yet been
	// determined, that is a problem and an error. We should probably throw
	// a fit here or something.
	// For now we just return 0 and leave it to the caller to complain.
	if (meta_enginefuncs_t::version() == 0) return 0;

	// The default version is 1.
	sm_version = 1;

	// With the enginefuncs interface version 156 the function
	// pfnCvarValue() was added, which we call version 2.
	if (meta_enginefuncs_t::version() >= 156) sm_version = 2;

	// With the enginefuncs interface version 157 the function
	// pfnCvarValue2() was added, which we call version 3.
	if (meta_enginefuncs_t::version() >= 157) sm_version = 3;

	return sm_version;
}


size_t meta_new_dll_functions_t::get_size(int _version)
{
	size_t size = sizeof(NEW_DLL_FUNCTIONS);

	if (0 == _version) {
		// Use the current engine's interface version
		_version = version();

		// Error: meta_enginefuncs_t::version probably not yet set up.
		if (0 == _version) return 0;
	}

	switch (_version) {
	case 1:
		// Version 1 is missing all functions from CvarValue() on.
		size -= sizeof(FN_CVARVALUE);
	case 2:
		// Version 2 is missing all functions from CvarValue2() on.
		size -= sizeof(FN_CVARVALUE2);
	}

	return size;
}


// --------------------------------------------------------------
// meta_enginefuncs_t
// --------------------------------------------------------------

// static member initialisation
int meta_enginefuncs_t::sm_version = 0;

// -----------------------------------------------------------------
// HL_enginefuncs
// -----------------------------------------------------------------

void HL_enginefuncs_t::initialise_interface(enginefuncs_t* _pFuncs)
{
	set_from(_pFuncs);

	// Now the pfnAlertMessage is available and we trust it to be a valid
	// pointer, so flush the message buffer.
	flush_ALERT_buffer();

	determine_engine_interface_version();
	fixup_engine_interface();
}


// The following part (i.e. the end) of the enginefuncs_t struct  is 
// used to determine the engine interface version since it is the one
// that changed since SDK 212 engines. We call this the "signature" of
// the enginefuncs interface.
//
// Default version is 138. That's what the SDK says.
//
// 144:	const char *(*pfnGetPlayerAuthId)					( edict_t *e );
//
//		// PSV: Added for CZ training map
//	//	const char *(*pfnKeyNameForBinding)					( const char* pBinding );
//	
//		sequenceEntry_s*	(*pfnSequenceGet)				( const char* fileName, const char* entryName );
//  	sentenceEntry_s*	(*pfnSequencePickSentence)		( const char* groupName, int pickMethod, int *picked );
//	
//		// LH: Give access to filesize via filesystem
// 147:	int			(*pfnGetFileSize)						( const char *filename );
//
//		unsigned int (*pfnGetApproxWavePlayLen)				(const char *filepath);
//		// MDC: Added for CZ career-mode
//		int			(*pfnIsCareerMatch)						( void );
//
//		// BGC: return the number of characters of the localized string referenced by using "label"
//		int			(*pfnGetLocalizedStringLength)			(const char *label);
//
//		// BGC: added to facilitate persistent storage of tutor message decay values for
//		// different career game profiles.  Also needs to persist regardless of mp.dll being
//		// destroyed and recreated.
//		void		(*pfnRegisterTutorMessageShown)			(int mid);
//		int			(*pfnGetTimesTutorMessageShown)			(int mid);
//		void		(*pfnProcessTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
//		void		(*pfnConstructTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
// 155:	void		(*pfnResetTutorMessageDecayData)		( void );

// 156:	void		(*pfnQueryClientCvarValue)				( const edict_t *player, const char *cvarName );
// 157:	void		(*pfnQueryClientCvarValue2)				( const edict_t *player, const char *cvarName, int requestID );
// 158: int			(*pfnCheckParm)							( const char *pchCmdLineToke, char **ppnext );


void HL_enginefuncs_t::determine_engine_interface_version(void) const
{
	// We only need to do this once.
	if (0 != sm_version) {
		return;
	}

	// Now begins our heuristic, where we try to determine the engine
	// interface version.
	// As alluded to above we are currently only interested, and thus
	// only detect, versions 144, 147, 156 or 157 (defined by us).

	// The minimal default is 138.
	sm_version = 138;

	// If GetPlayerAuthId() is present, it is at least 144,
	// otherwise leave it at the default 138.
	// This may give incorrect results for *really* old engine versions,
	// i.e. pre 1.1.0.8. We live with that risk. No one uses them anymore.
	// Really.
	if (pfnGetPlayerAuthId == NULL) {
		return;
	}
	sm_version = 144;

	// The two function pointers for pfnSequenceGet() and
	// pfnSequencePickSentence() are only valid in a few engine versions
	// and are set to NULL in most other version, so they don't get
	// checked.

	// If pfnGetFileSize() is present, it is at least 147,
	// otherwise leave it at the so far determined value.
	if (pfnGetFileSize == NULL) {
		return;
	}
	sm_version = 147;

	// Now it gets a bit fuzzy. If all of the functions following GetFileSize()
	// but before QueryClientCvarValue() are valid, it is at least 155.
	// If even one of them is NULL, then our version can't be higher than 147,
	// so use 147. Actually, it could be that there exist engine
	// versions where one of them is NULL but the interface is still at
	// least 155. If such an engine is found in use, adaptions need to be
	// made.
	// (Yes, I know this could be done with a little hacky for() loop. We
	// don't need to do hacky here.)
	int cntInvals = 0;
	if (pfnGetApproxWavePlayLen == NULL) cntInvals++;
	if (pfnIsCareerMatch == NULL) cntInvals++;
	if (pfnGetLocalizedStringLength == NULL) cntInvals++;
	if (pfnRegisterTutorMessageShown == NULL) cntInvals++;
	if (pfnGetTimesTutorMessageShown == NULL) cntInvals++;
	if (pfnProcessTutorMessageDecayBuffer == NULL) cntInvals++;
	if (pfnConstructTutorMessageDecayBuffer == NULL) cntInvals++;
	if (pfnResetTutorMessageDecayData == NULL) cntInvals++;

	if (cntInvals > 0) {
		return;
	}
	sm_version = 155;

	// All functions up to QueryClientCvarValue() are valid.
	// If QueryClientCvarValue() is not valid, leave it at the so far 
	// determined version. Otherwise the version is at least 156.
	if (pfnQueryClientCvarValue == NULL) {
		return;
	}
	sm_version = 156;

	// All functions up to QueryClientCvarValue2() are valid.
	// If QueryClientCvarValue2() is not valid, leave it at the so far 
	// determined version.  Otherwise the version is at least 157.
	if (pfnQueryClientCvarValue2 == NULL) {
		return;
	}
	sm_version = 157;

	// All functions up to CheckParm() are valid.
	// If CheckParm() is not valid, leave it at the so far determined
	// version. Otherwise the version is at least 158.
	if (pfnEngCheckParm == NULL) {
		return;
	}
	sm_version = 158;
}


void HL_enginefuncs_t::fixup_engine_interface(void)
{
	// This function will make sure that all function pointers that aren't
	// valid are set to NULL, depending on the engine interface version.
	// Sometimes a pointer has a valid value although the function doesn't
	// exist in the interface version.

	switch (version()) {
	case 138:
		pfnGetPlayerAuthId = NULL;
	case 144:
		pfnSequenceGet = NULL;
		pfnSequencePickSentence = NULL;
		pfnGetFileSize = NULL;
	case 147:
		pfnGetApproxWavePlayLen = NULL;
		pfnIsCareerMatch = NULL;
		pfnGetLocalizedStringLength = NULL;
		pfnRegisterTutorMessageShown = NULL;
		pfnGetTimesTutorMessageShown = NULL;
		pfnProcessTutorMessageDecayBuffer = NULL;
		pfnConstructTutorMessageDecayBuffer = NULL;
		pfnResetTutorMessageDecayData = NULL;
	case 155:
		pfnQueryClientCvarValue = NULL;
	case 156:
		pfnQueryClientCvarValue2 = NULL;
	case 157:
		pfnEngCheckParm = NULL;
	}
}
