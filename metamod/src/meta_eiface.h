
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// meta_eiface.h - wrapper for engine/dll interface

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

#ifndef MM_META_EIFACE_H
#define MM_META_EIFACE_H

#include <extdll.h> // NEW_DLL_FUNCTIONS, enginefuncs_t

#include <string.h> // memset()



// We use our own versions of the engine/dll interface structs. We add a
// few dummy entries to the end and set them to 0. That way we are
// protected from updates to the HL SDK adding new functions which would
// cause a) the game dll copying arbitrary values from us and b) the game
// dll overwriting our memory when using an old Metamod with a new game
// dll. 

// --------------------------------------------------------------------
// meta_new_dll_functions_t
// --------------------------------------------------------------------

struct meta_new_dll_functions_t : public NEW_DLL_FUNCTIONS
{
	meta_new_dll_functions_t();
	meta_new_dll_functions_t(
		void (*pfnOnFreeEntPrivateData)(edict_t*),
		void (*pfnGameShutdown)(void),
		int (*pfnShouldCollide)(edict_t*, edict_t*),
		void (*pfnCvarValue)(const edict_t*, const char*),
		void (*pfnCvarValue2)(const edict_t*, int, const char*, const char*)
	);

	meta_new_dll_functions_t(const meta_new_dll_functions_t&);
	meta_new_dll_functions_t& operator=(const meta_new_dll_functions_t&);

	// Fill this object with pointers copied from a NEW_DLL_FUNCTIONS struct.
	void set_from(NEW_DLL_FUNCTIONS* pFuncs);

	// Copy the pointers from this object to a NEW_DLL_FUNCTIONS struct.
	void copy_to(NEW_DLL_FUNCTIONS* pFuncs);

	// return the engine's version of NEW_DLL_FUNCTIONS
	int version(void);


private:

	// data :

	// The NEW_DLL_FUNCTIONS struct also changed, but the version
	// number did not change. That begs the question why to have 
	// it versioned in the first place, but whaddaya know.
	// While the official version is left at 1, we internally
	// calculate a different version of the engine's NEW_DLL_FUNCTIONS
	// struct since we know that the engine lies to us about the
	// version that it uses.
	//
	// The default version is 1.
	// 
	// With the enginefuncs interface version 156 the function
	// pfnCvarValue() was added, which we call version 2.
	//
	// With the enginefuncs interface version 157 the function
	// pfnCvarValue2() was added, which we call version 3.
	//
	// If Valve ever decides to change the version of the
	// NEW_DLL_FUNCTIONS interface in the future (haha),
	// we are in trouble and will need to change our
	// internal versions.

	static int sm_version;

	// functions :

	// Calculates our idea of the engine's version of the
	// NEW_DLL_FUNCTIONS interface. Stores this version for future
	// reference in m_version and returns it.
	static int determine_interface_version(void);

	// Comfort function to determine the size of the NEW_DLL_FUNCTIONS
	// struct for the different versions.
	// If passed a version number other than 0, the size for that
	// specific version is returned. 
	// If passed 0 as version number (default) the size for the version
	// that was determined to be the version of the currently connected
	// engine's interface. Should that version have not yet been
	// determined (via the enginefuncs_t interface), 0 is returned to
	// indicated this error state.
	size_t get_size(int version = 0);
};


// Inline functions

inline meta_new_dll_functions_t::meta_new_dll_functions_t()
{
	memset(this, 0, sizeof(meta_new_dll_functions_t));
}


inline meta_new_dll_functions_t::meta_new_dll_functions_t(const meta_new_dll_functions_t& _rhs)
{
	memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
}


inline meta_new_dll_functions_t& meta_new_dll_functions_t::operator=(const meta_new_dll_functions_t& _rhs)
{
	memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
	return *this;
}


inline void meta_new_dll_functions_t::set_from(NEW_DLL_FUNCTIONS* _pFuncs)
{
	memcpy(this, _pFuncs, sizeof(NEW_DLL_FUNCTIONS));
}


inline int meta_new_dll_functions_t::version(void)
{
	return sm_version ? sm_version : determine_interface_version();
}


// No meta version of DLL_FUNCTIONS because that won't be changing anymore.


// --------------------------------------------------------------------
// meta_enginefuncs_t
// --------------------------------------------------------------------


// meta_enginefuncs_t
struct meta_enginefuncs_t : public enginefuncs_t
{
	meta_enginefuncs_t()
	{
	};

	meta_enginefuncs_t(enginefuncs_t* pFuncs)
	{
		set_from(pFuncs);
	};

	// Fill this object with pointers copied from an enginefuncs_t struct.
	void set_from(enginefuncs_t* pFuncs);

	// Copy the pointers from this object to an enginefuncs_t struct.
	void copy_to(enginefuncs_t* pFuncs);

	// The version of the engine functions interface. It is frozen at 138. But no one knows
	// when that was and what it looked like then. So we simply interprete it as the
	// number of functions that the enginefuncs struct contains.
	// 
	// That means we get gaps inbetween versions and right now we can detect only
	// about five different versions anyway, but that suffices for the current itches
	// to get scratched.
	//
	// The default is hence 138.
	// A value of 0 means "not yet determined".
	// Other possible versions currently detectable:
	// 144: engine versions after 1.1.0.9 build 1996
	// 147: engine versions after build 2384 with pfnGetFileSize()
	// 155: all versions between build 2384 and the one 
	//      including pfnQueryClientCvarValue()
	// 156: includes pfnQueryClientCvarValue()
	// 157: includes pfnQueryClientCvarValue2()
	// 158: includes pfnCheckParm()
	static int sm_version;
	static int version(void);
};

//
// Inline functions
//

inline void meta_enginefuncs_t::set_from(enginefuncs_t* _pFuncs)
{
	memcpy(this, _pFuncs, sizeof(enginefuncs_t));
}


inline void meta_enginefuncs_t::copy_to(enginefuncs_t* _pFuncs)
{
	memcpy(_pFuncs, this, sizeof(enginefuncs_t));
}


inline int meta_enginefuncs_t::version(void)
{
	return sm_version;
}


// --------------------------------------------------------------------
// HL_enginefuncs_t
// --------------------------------------------------------------------

//
// This is a specialisation of the meta_enginefuncs_t struct which is only
// used for the initial copy of the engine functions, i.e. those we get
// passed from the HL engine right at the beginning. 
// This specialisation does some extra initialisation when getting set up
// like calculating the engine interface version and fixing up any invalid
// pointers.
// Since there is only one master copy of engine functions this could be
// implemented as a singleton. This is left as an option for later.
//
struct HL_enginefuncs_t : public meta_enginefuncs_t
{
	// functions :

	HL_enginefuncs_t()
	{
	};

	// Fill this object with pointers copied from an enginefuncs_t struct
	// and fixup the interface.
	// For this class this happens in the GiveFptrsToDll() function
	// with the pointers passed from the HL engine.
	void initialise_interface(enginefuncs_t* pFuncs);


private:

	// functions :

	// Moving copy_to() and set_from() to the private space.
	void set_from(enginefuncs_t* pFuncs)
	{
		meta_enginefuncs_t::set_from(pFuncs);
	};

	void copy_to(enginefuncs_t* pFuncs)
	{
		meta_enginefuncs_t::copy_to(pFuncs);
	};


	// Determine the version of the engine interface from the
	// enginefuncs signature.
	void determine_engine_interface_version(void) const;

	// Fixup the enginefuncs pointers according to the determined
	// version as some pointers may be invalid.
	void fixup_engine_interface(void);
};


#endif /* META_EIFACE_H */

