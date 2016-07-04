
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engineinfo.h - info about HL engine, like file used and 
//                text segment range
//

#ifndef MM_ENGINEINFO_H
#define MM_ENGINEINFO_H



#ifdef _WIN32
typedef void* MemAddr;
#else
#  include <link.h>           // ElfW(Addr/Phdr) macros
typedef void* MemAddr;
#endif /* _WIN32 */ 

#include "extdll.h"           // eiface.h: enginefuncs_t

#include "comp_dep.h"
#include "osdep.h"	//unlikely, OPEN_ARGS
#include "new_baseclass.h"


// What we return in is_valid_code_pointer() when the EngineInfo object is
// in an INVALID state, i.e. no code address range could be determined.
static const bool c_DefaultReturnOnInvalidState = true;

static const int  c_EngineInfo__typeLen = 10;


class EngineInfo : public class_metamod_new
{
	private:
	// data :

	MemAddr m_codeStart;
	MemAddr m_codeEnd;
        
        // State is either NULL  when not yet initialised,
        //                 VALID if a code range could be determined
        //              or INVALID when no valid range for code addresses
        //                 could be determined.
        char m_state;

        // Type of engine dso/dll used.
        // For Linux this specifies the architecture, i.e. 'i486', 'i686', 
        // 'amd', 'amd64' etc.
        // For Windows this is either 'sw' or 'hw' or 'swds' depending on
        // the server type.
	char m_type[c_EngineInfo__typeLen]; 

	// functions :


        // Check if string is valid name of engine dso/dll.
	bool DLLINTERNAL check_for_engine_module( const char* pName );
        
#ifdef _WIN32

        // Set info using the PE header found by module name.
        // Returns 0 on success, error code on failure.
        int DLLINTERNAL nthdr_module_name( void );

	int DLLINTERNAL vac_pe_approx( enginefuncs_t* pFuncs );

        // Set code segment start and end from PEheader. The base
        // address, that relative addresses are based on, is passed in
        // pBase.
        void DLLINTERNAL set_code_range( unsigned char* pBase, PIMAGE_NT_HEADERS pNThdr );

#else
        
        // Set info using the Programheader found via r_debug struct.
        // Returns 0 on success, error code on failure.
	int DLLINTERNAL phdr_r_debug( void );
        // Set info using the Programheader found with reference address
        // via dladdr(). Returns 0 on success, error code on failure.
	int DLLINTERNAL phdr_dladdr( void* pMem );
        // Set info using the Programheader found via ELF header passed as
        // pElfHdr. Return 0 on success, error code on failure.
	int DLLINTERNAL phdr_elfhdr( void* pElfHdr );
        // Set code segment start and end from Programheader. The base
        // address, that relative addresses are based on, is passed in
        // pBase.
        void DLLINTERNAL set_code_range( void* pBase, ElfW(Phdr)* pPhdr );
        
#endif /* _WIN32 */ 

	public:
	// codes :

	enum {
		STATE_NULL = 0,
		STATE_VALID,
		STATE_INVALID,
        
		MODULE_NAME_NOTFOUND = 5,
        	INVALID_DOS_SIGN,
        	INVALID_NT_SIGN,
        	INVALID_ARG,
        	HEADER_NOTFOUND,
        	NOTFOUND
	};

        
	// functions :

	EngineInfo() DLLINTERNAL;
        EngineInfo& operator=( const EngineInfo& ) DLLINTERNAL;
        EngineInfo( const EngineInfo& ) DLLINTERNAL;

        const char* DLLINTERNAL type( void );
        
        // Initilaise object, determining the bounds of the code segment of
        // the HL engine shared object.
	int DLLINTERNAL initialise( enginefuncs_t* pFuncs  = NULL );
        
        // Test if pMem is within bounds of the code segment.
	bool DLLINTERNAL is_valid_code_pointer( void* pMem );

        // Overloaded versions of above test to keep the ugly pointer
        // conversion stuff in here.
        bool DLLINTERNAL is_valid_code_pointer(      const char* (*fp) (edict_t*) );
        bool DLLINTERNAL is_valid_code_pointer( sequenceEntry_s* (*fp) (const char*, const char*) );
        bool DLLINTERNAL is_valid_code_pointer( sentenceEntry_s* (*fp) (const char*, int, int*) );
        bool DLLINTERNAL is_valid_code_pointer(              int (*fp) (char*) );
        bool DLLINTERNAL is_valid_code_pointer(     unsigned int (*fp) (const char*) );
        bool DLLINTERNAL is_valid_code_pointer(              int (*fp) (void) );
        bool DLLINTERNAL is_valid_code_pointer(              int (*fp) (const char*) );
        bool DLLINTERNAL is_valid_code_pointer(             void (*fp) (int) );
        bool DLLINTERNAL is_valid_code_pointer(              int (*fp) (int) );
        bool DLLINTERNAL is_valid_code_pointer(             void (*fp) (int*, int) );
        bool DLLINTERNAL is_valid_code_pointer(             void (*fp) (void) );
        bool DLLINTERNAL is_valid_code_pointer(             void (*fp) (const edict_t*, const char*) );
        bool DLLINTERNAL is_valid_code_pointer(             void (*fp) (const edict_t*, const char*, int) );
        bool DLLINTERNAL is_valid_code_pointer(              int (*fp) (const char *, char**) );
};



// We probably should run an initialisation here without a reference
// pointer so that the object has valid info in any case. 
inline EngineInfo::EngineInfo() :
	m_codeStart(NULL),
	m_codeEnd(NULL),
	m_state(STATE_NULL)
{
	m_type[0] = '\0';	
}


inline EngineInfo::EngineInfo( const EngineInfo& _rhs) :
	m_codeStart(_rhs.m_codeStart),
	m_codeEnd(_rhs.m_codeEnd),
	m_state(STATE_NULL)
{
	memcpy( m_type, _rhs.m_type, c_EngineInfo__typeLen );
}


inline EngineInfo& EngineInfo::operator=( const EngineInfo& _rhs)
{
	m_state = _rhs.m_state;
	m_codeStart = _rhs.m_codeStart;
	m_codeEnd = _rhs.m_codeEnd;
	memcpy( m_type, _rhs.m_type, c_EngineInfo__typeLen );
	return *this;
}


inline const char* EngineInfo::type( void )
{
	return m_type;
}

inline bool EngineInfo::is_valid_code_pointer( void* _pMem )
{
	if ( STATE_INVALID == m_state ) {
		return c_DefaultReturnOnInvalidState;
	}
	if ( NULL != _pMem && m_codeStart <= _pMem && _pMem <= m_codeEnd ) {
		return true;
	}

	return false;
}

inline bool EngineInfo::is_valid_code_pointer( const char* (*_fp) (edict_t*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( sequenceEntry_s* (*_fp) (const char*, const char*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( sentenceEntry_s* (*_fp) (const char*, int, int*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( int (*_fp) (char*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( unsigned int (*_fp) (const char*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( int (*_fp) (void) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( int (*_fp) (const char*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( void (*_fp) (int) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( int (*_fp) (int) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( void (*_fp) (int*, int) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( void (*_fp) (void) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( void (*_fp) (const edict_t*, const char*) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( void (*_fp) (const edict_t*, const char*, int) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

inline bool EngineInfo::is_valid_code_pointer( int (*_fp) (const char*, char**) )
{
    	return is_valid_code_pointer( (void*)_fp );
}

#endif /* MM_ENGINEINFO_H */

