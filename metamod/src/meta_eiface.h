#pragma once

// We use our own versions of the engine/dll interface structs. We add a
// few dummy entries to the end and set them to 0. That way we are
// protected from updates to the HL SDK adding new functions which would
// cause a) the game dll copying arbitrary values from us and b) the game
// dll overwriting our memory when using an old Metamod with a new game
// dll.

// meta_new_dll_functions_t
struct meta_new_dll_functions_t: public NEW_DLL_FUNCTIONS
{
	meta_new_dll_functions_t();
	meta_new_dll_functions_t(
		void (*pfnOnFreeEntPrivateData)(edict_t *),
		void (*pfnGameShutdown)(),
		int (*pfnShouldCollide)(edict_t *, edict_t *),
		void (*pfnCvarValue)(const edict_t *, const char *),
		void (*pfnCvarValue2)(const edict_t *, int, const char *, const char *)
	);

	meta_new_dll_functions_t(const meta_new_dll_functions_t&);
	meta_new_dll_functions_t& operator=(const meta_new_dll_functions_t&);

	// Fill this object with pointers copied from a NEW_DLL_FUNCTIONS struct.
	void set_from(NEW_DLL_FUNCTIONS* pFuncs);

	// Copy the pointers from this object to a NEW_DLL_FUNCTIONS struct.
	void copy_to(NEW_DLL_FUNCTIONS* pFuncs);
};

inline meta_new_dll_functions_t::meta_new_dll_functions_t()
{
	Q_memset(this, 0, sizeof(meta_new_dll_functions_t));
}

inline meta_new_dll_functions_t::meta_new_dll_functions_t(const meta_new_dll_functions_t& _rhs)
{
	Q_memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
}

inline meta_new_dll_functions_t& meta_new_dll_functions_t::operator=(const meta_new_dll_functions_t& _rhs)
{
	Q_memcpy(this, &_rhs, sizeof(NEW_DLL_FUNCTIONS));
	return *this;
}

inline void meta_new_dll_functions_t::set_from(NEW_DLL_FUNCTIONS* _pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(NEW_DLL_FUNCTIONS));
}

// meta_enginefuncs_t
struct meta_enginefuncs_t : public enginefuncs_t
{
	meta_enginefuncs_t() {};
	meta_enginefuncs_t(enginefuncs_t* pFuncs)
	{
		set_from(pFuncs);
	};

	// Fill this object with pointers copied from an enginefuncs_t struct.
	void set_from(enginefuncs_t* pFuncs);

	// Copy the pointers from this object to an enginefuncs_t struct.
	void copy_to(enginefuncs_t* pFuncs);
};

inline void meta_enginefuncs_t::set_from(enginefuncs_t* _pFuncs)
{
	Q_memcpy(this, _pFuncs, sizeof(enginefuncs_t));
}

inline void meta_enginefuncs_t::copy_to(enginefuncs_t* _pFuncs)
{
	Q_memcpy(_pFuncs, this, sizeof(enginefuncs_t));
}

// This is a specialisation of the meta_enginefuncs_t struct which is only
// used for the initial copy of the engine functions, i.e. those we get
// passed from the HL engine right at the beginning.
// Since there is only one master copy of engine functions this could be
// implemented as a singleton. This is left as an option for later.
struct HL_enginefuncs_t: public meta_enginefuncs_t
{
	HL_enginefuncs_t() {};

	// Fill this object with pointers copied from an enginefuncs_t struct
	// and fixup the interface.
	// For this class this happens in the GiveFptrsToDll() function
	// with the pointers passed from the HL engine.
	void initialise_interface(enginefuncs_t* pFuncs);

private:
	// Moving copy_to() and set_from() to the private space.
	void set_from(enginefuncs_t* pFuncs)
	{
		meta_enginefuncs_t::set_from(pFuncs);
	}

	void copy_to(enginefuncs_t* pFuncs)
	{
		meta_enginefuncs_t::copy_to(pFuncs);
	}
};
