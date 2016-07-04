
// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engineinfo.cpp - get info about HL engine, like file used
//                  and test segment range
//


#ifdef _WIN32
   // Don't include winspool.h; clashes with SERVER_EXECUTE from engine
#  define _WINSPOOL_H	
#  include <windows.h>
#  include <winnt.h>     // Header structures
#else 
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#  include <dlfcn.h>			// dladdr()
#  include <link.h>				// ElfW(Phdr/Ehdr) macros.
                  				// _DYNAMIC, r_debug, link_map, etc.
#endif /* _WIN32 */ 

#include <string.h>				// strlen(), strrchr(), strcmp()
#include <stdio.h>				// printf()

#include "engineinfo.h"			// me
#include "log_meta.h"			// META_DEV()


// Current mask for checking engine function addresses 
// in VAC protected engine dlls on Win32. This is gonna fail
// on Win64.
const unsigned long c_VacDllEngineFuncsRangeMask = 0xFFF00000;
const unsigned long c_VacDllEngineFuncsRangeMark = 0x01D00000;
void* const c_VacDllEngineFuncsRangeStart = (void*)0x01D00000;
void* const c_VacDllEngineFuncsRangeEnd = (void*)0x01E00000;


bool DLLINTERNAL EngineInfo::check_for_engine_module( const char* _pName )
{
	const char* pC;
	size_t size;

	if ( NULL == _pName ) return false;

#ifdef _WIN32

	// The engine module is either sw.dll or hw.dll or swds.dll
	pC = strrchr( _pName, '.' );

	pC -= 2;
	if ( 0 != strcmp(pC, "sw.dll") && 0 != strcmp(pC, "hw.dll") ) {
		pC -= 2;
		if ( 0 != strcmp(pC, "swds.dll") ) return false;
	}

	HMODULE hModule = GetModuleHandle( pC );
	if ( NULL == hModule ) {
		return false;
	}

	// Ok, we found the string sw(ds).dll, thus we deduct that this is the
	// name of the engine's shared object. We copy the string for future 
	// reference and return successfully.
	size = 0;
	while ( *pC != '.' && size < c_EngineInfo__typeLen-1 ) {
		m_type[size++] = *pC++;
	}
	m_type[size] = '\0';

#else /* _WIN32 */

	const char* pType;


	size = strlen( _pName );
	if ( size < 11 ) {
		// Forget it, this string is too short to even be 'engine_.so', so
		// it can't the name of the engine shared library.
		return false;
	}

	// Start parsing at the end. Since we know that the string is at least
	// 11 characters long we can safely do our parsing without being afraid
	// of leaving the string.
	pC = _pName + size -1;

	// First we see if the string ends in '.so'
	if ( *pC-- != 'o' || *pC-- != 's' || *pC-- != '.' ) return false;

	// Now find the '_' which would be the seperator between 'engine' and
	// the architecture.
	while ( *pC != '_' && pC != _pName ) --pC;
	if ( pC == _pName ) return false;

	// We are at the '_', thus the architecture identifier must start at
	// the next character.
	pType = pC +1;

	// Now we walk further back and check if we find the string 'engine'
	// backwards.
	--pC;
	if ( *pC-- != 'e' || *pC-- != 'n' || *pC-- != 'i' ||
		 *pC-- != 'g' || *pC-- != 'n' || *pC != 'e' ) {
		return false;
	}

	// Ok, we found the string engine_*.so, thus we deduct that this is the
	// name of the engine's shared object. We copy the architecture string
	// for future reference and return successfully.
	size = 0;
	while ( *pType != '.' && size < c_EngineInfo__typeLen-1 ) {
		m_type[size++] = *pType++;
	}
	m_type[size] = '\0';


#endif /* _WIN32 */


	return true;
}


#ifdef _WIN32

int DLLINTERNAL EngineInfo::nthdr_module_name( void )
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	unsigned char* pBaseAddr;
	const char* pName = "sw.dll";

	if ( ! check_for_engine_module(pName) ) {
		pName = "hw.dll";
		if ( ! check_for_engine_module(pName) ) {
			pName = "swds.dll";
			if ( ! check_for_engine_module(pName) ) {
				return MODULE_NAME_NOTFOUND;
			}
		}
	}
	
	// Get the module handle for the engine dll we found.
	// This is also the modules base address.
	HMODULE hModule = GetModuleHandle( pName );

	pBaseAddr = (unsigned char*)hModule;


	// Check if we find a DOS header
	pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE ) {
		return INVALID_DOS_SIGN;
	}

	// Check if we find a PE header
	pNTHeader = (PIMAGE_NT_HEADERS)(pBaseAddr + pDosHeader->e_lfanew);
	if ( pNTHeader->Signature != IMAGE_NT_SIGNATURE ) {
		return INVALID_NT_SIGN;
	}

	set_code_range( pBaseAddr, pNTHeader );
	return 0;
}


int DLLINTERNAL EngineInfo::vac_pe_approx( enginefuncs_t* _pFuncs )
{
	if ( NULL == _pFuncs ) return INVALID_ARG;
	
	// There is really no good and easy way to do this. Right now what 
	// we do is assume that Steam listenservers will normally be 
	// up-to-date and hence have all function pointers set correctly.
	// So we just check for some anomality and then set some approximated
	// values.

	// The known addresses at the time of writing all start with 0x01D and
	// this lie in the range between 0x01D00000 and 0x01E00000. What we do
	// is check all functions pointers that are known to be good to start
	// with 0x01D. If that is the case then we know that the loading address
	// of the engine functions hasn't changed and we assume the above stated
	// range to be the range of valid engine function addresses.
	// If we find functions outside this range we can't determine a valid
	// range and have to just give up.

	unsigned long* pengfuncs = (unsigned long*)_pFuncs;
	unsigned int i, invals = 0;
	for ( i = 0, pengfuncs += i; i < 140; i++, pengfuncs++ ) {
		if ( ((*pengfuncs) & c_VacDllEngineFuncsRangeMask) != c_VacDllEngineFuncsRangeMark ) {
			invals++;
			break;
		}
	}

	if ( invals > 0 ) {
		META_DEV( "Found %d engine functions out of range 0x%08lx. "
			      "Unable to determine valid engine code address range.", 
			      invals, c_VacDllEngineFuncsRangeMark );

		strncpy( m_type, "vacdll+?", c_EngineInfo__typeLen );
        m_state = STATE_INVALID;
		return STATE_INVALID;
	}

	m_codeStart = c_VacDllEngineFuncsRangeStart;
	m_codeEnd = c_VacDllEngineFuncsRangeEnd;

	strncpy( m_type, "vacdll", c_EngineInfo__typeLen );

	m_state = STATE_VALID;

	return 0;
}


void DLLINTERNAL EngineInfo::set_code_range( unsigned char* _pBase, PIMAGE_NT_HEADERS _pNThdr )
{
	m_codeStart = _pBase + _pNThdr->OptionalHeader.BaseOfCode;
	m_codeEnd = _pBase + _pNThdr->OptionalHeader.BaseOfCode + _pNThdr->OptionalHeader.SizeOfCode;

	m_state = STATE_VALID;
}


#else /* _WIN32 */


int DLLINTERNAL EngineInfo::phdr_elfhdr( void* _pElfHdr )
{
	ElfW(Ehdr)* pEhdr = (ElfW(Ehdr)*)_pElfHdr;
	ElfW(Phdr)* pPhdr;

	if ( NULL == _pElfHdr ) return INVALID_ARG;
	
	if ( pEhdr->e_ident[0] == 0x7f
	  && pEhdr->e_ident[1] == 'E'
	  && pEhdr->e_ident[2] == 'L'
	  && pEhdr->e_ident[3] == 'F' ) {

		// Looking good, we found an ELF signature.
		// Let's see if the rest of the data at this address would fit an ELH header, too.
		if (  (pEhdr->e_ident[EI_CLASS] == ELFCLASS32 //ELFW(CLASS)
			|| pEhdr->e_ident[EI_CLASS] == ELFCLASS64)

			&& (pEhdr->e_ident[EI_DATA] == ELFDATA2LSB
			|| pEhdr->e_ident[EI_DATA] == ELFDATA2MSB)

			&& pEhdr->e_type == ET_DYN ) {

			// Ok, we believe that this is a shared object's ELF header.
			// Let's find the program header for the segment that includes
			// the text section and return it.
        
			pPhdr = (ElfW(Phdr) *) ((char *) pEhdr + pEhdr->e_phoff);
			for ( int i = 0; i < pEhdr->e_phnum; i++, pPhdr++ ) {

				if ( PT_LOAD == pPhdr->p_type 
					&& (pPhdr->p_flags & PF_R) 
					&& (pPhdr->p_flags & PF_X) ) {

					// The text section gets loaded and has read and
					// execute flags set. So this must be it.
					set_code_range( pEhdr, pPhdr );
					return 0;
				}
			}
		}
	}

	return HEADER_NOTFOUND;
}




int DLLINTERNAL EngineInfo::phdr_dladdr( void* _pMem )
{
	Dl_info info;

	if ( 0 != dladdr(_pMem, &info) ) {
		// Check if this is the engine module
		if ( check_for_engine_module(info.dli_fname) ) {
		   return phdr_elfhdr( info.dli_fbase );
		}
	}

	return NOTFOUND;
}


int DLLINTERNAL EngineInfo::phdr_r_debug( void )
{
	ElfW(Dyn)* pDyn; 
	struct r_debug* pr_debug;
	struct link_map* pMap;


	// Search if we have a DT_DEBUG symbol in our DYNAMIC segment, which
	// ought to be set to the r_debug structure's address.
	for (pDyn = _DYNAMIC; pDyn->d_tag != DT_NULL; ++pDyn) {
		if (pDyn->d_tag == DT_DEBUG) {
			pr_debug = (struct r_debug *) pDyn->d_un.d_ptr;
			break;
		}
	}

	if ( DT_NULL == pDyn->d_tag ) {
	}
	else if ( NULL == pr_debug ) {
	}
	else {
		pMap = pr_debug->r_map;

		// Walk to the start of the list
		while ( pMap->l_prev != NULL ) pMap = pMap->l_prev;
		do {
			if ( check_for_engine_module(pMap->l_name) ) {
				return phdr_elfhdr( (void*)pMap->l_addr );
			}

			pMap = pMap->l_next;
		} while ( NULL != pMap );
        
	}

	return NOTFOUND;
}

void DLLINTERNAL EngineInfo::set_code_range( void* _pBase, ElfW(Phdr)* _pPhdr )
{
	unsigned char* pBase = (unsigned char*)_pBase;
	m_codeStart = pBase + _pPhdr->p_vaddr;
	m_codeEnd   = pBase + _pPhdr->p_vaddr + _pPhdr->p_memsz;

	m_state = STATE_VALID;
}


#endif /* _WIN32 */


int DLLINTERNAL EngineInfo::initialise( enginefuncs_t* _pFuncs )
{
	int ret = 0;

	// Have to do this only once.
	if ( NULL != m_codeStart ) {
		return 0;
	}

#ifdef _WIN32

	ret = nthdr_module_name();
	if ( MODULE_NAME_NOTFOUND == ret && (! _pFuncs->pfnIsDedicatedServer()) ) {
		// We could not find the engine dll by name and we are running on
		// a listen server. This usually means that that we are dealing with
		// a VAC protected engine dll. No other way than approximating the 
		// range of valid engine function pointers in this case.
		ret = vac_pe_approx( _pFuncs );
	}

#else /* _WIN32 */

	// If we have no reference pointer to start from we can only try to use
	// the r_debug symbol.
	if ( NULL == _pFuncs ) {
		ret = phdr_r_debug();
	}

	// If we have a refererence pointer we try to use it first.
	if ( 0 != phdr_dladdr(_pFuncs) ) {
		ret = phdr_r_debug();
	}

#endif /* _WIN32 */

	if ( 0 != ret ) {
		META_DEV( "Unable to determine engine code address range!" );
	}
	else {
		META_DEV( "Set engine code range: start address = %p, end address = %p", 
			 m_codeStart, m_codeEnd );
	}

	return 0;
}


