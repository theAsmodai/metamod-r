/*
 * Copyright (c) 2004-2006 Jussi Kivilinna
 *
 *    This file is part of "Metamod All-Mod-Support"-patch for Metamod.
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
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <extdll.h>			// always

#include "osdep_p.h"			// me
#include "support_meta.h"		// STRNCPY

//check for invalid handle values
#define is_invalid_handle(X) ((X)==0 || (X)==INVALID_HANDLE_VALUE)

//relative virtual address to virtual address
#define rva_to_va(base, rva) ((unsigned long)base + (unsigned long)rva)
//virtual address to relative virtual address
#define va_to_rva(base, va) ((unsigned long)va - (unsigned long)base)

//
static unsigned long DLLINTERNAL_NOVIS va_to_mapaddr(void * mapview, IMAGE_SECTION_HEADER * sections, int num_sects, unsigned long vaddr) {
	for(int i = 0; i < num_sects; i++)
		if(vaddr >= sections[i].VirtualAddress && vaddr < sections[i].VirtualAddress + sections[i].SizeOfRawData)
			return(rva_to_va(mapview, (vaddr - sections[i].VirtualAddress + sections[i].PointerToRawData)));
	
	return(0);
}

// Checks module signatures and return ntheaders pointer for valid module
static IMAGE_NT_HEADERS * DLLINTERNAL_NOVIS get_ntheaders(void * mapview) {
	union { 
		unsigned long      mem;
		IMAGE_DOS_HEADER * dos;
		IMAGE_NT_HEADERS * pe;
	} mem;
	
	//Check if valid dos header
	mem.mem = (unsigned long)mapview;
	if(IsBadReadPtr(mem.dos, sizeof(*mem.dos)) || mem.dos->e_magic != IMAGE_DOS_SIGNATURE)
		return(0);
	
	//Get and check pe header
	mem.mem = rva_to_va(mapview, mem.dos->e_lfanew);
	if(IsBadReadPtr(mem.pe, sizeof(*mem.pe)) || mem.pe->Signature != IMAGE_NT_SIGNATURE)
		return(0);
	
	return(mem.pe);
}

// Returns export table for valid module
static IMAGE_EXPORT_DIRECTORY * DLLINTERNAL_NOVIS get_export_table(void * mapview, IMAGE_NT_HEADERS * ntheaders, IMAGE_SECTION_HEADER * sections, int num_sects) {
	union { 
		unsigned long            mem;
		void *                   pvoid;
		IMAGE_DOS_HEADER       * dos;
		IMAGE_NT_HEADERS       * pe;
		IMAGE_EXPORT_DIRECTORY * export_dir;
	} mem;
	
	mem.pe = ntheaders;
	
	//Check for exports
	if(!mem.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
		return(0);
	
	mem.mem = va_to_mapaddr(mapview, sections, num_sects, mem.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if(IsBadReadPtr(mem.export_dir, sizeof(*mem.export_dir)))
		return(0);
	
	return(mem.export_dir);
}

mBOOL DLLINTERNAL is_gamedll(const char *filename) {
	HANDLE hFile;
	HANDLE hMap;
	void * mapview;
	IMAGE_NT_HEADERS * ntheaders;
	IMAGE_SECTION_HEADER * sections;
	int num_sects;
	IMAGE_EXPORT_DIRECTORY * exports;

	int has_GiveFnptrsToDll = 0;
	int has_GetEntityAPI2 = 0;
	int has_GetEntityAPI = 0;

	// Try open file for read
	hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(is_invalid_handle(hFile)) {
		META_DEBUG(3, ("is_gamedll(%s): CreateFile() failed.", filename));
		return(mFALSE);
	}
	
	//
	hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(is_invalid_handle(hMap)) {
		META_DEBUG(3, ("is_gamedll(%s): CreateFileMapping() failed.", filename));
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	//
	mapview = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if(!mapview) {
		META_DEBUG(3, ("is_gamedll(%s): MapViewOfFile() failed.", filename));
		CloseHandle(hMap);
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	ntheaders = get_ntheaders(mapview);
	if(!ntheaders) {
		META_DEBUG(3, ("is_gamedll(%s): get_ntheaders() failed.", filename));
		UnmapViewOfFile(mapview);
		CloseHandle(hMap);
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	//Sections for va_to_mapaddr function
	sections = IMAGE_FIRST_SECTION(ntheaders);
	num_sects = ntheaders->FileHeader.NumberOfSections;
	if(IsBadReadPtr(sections, num_sects * sizeof(IMAGE_SECTION_HEADER))) {
		META_DEBUG(3, ("is_gamedll(%s): IMAGE_FIRST_SECTION() failed.", filename));
		UnmapViewOfFile(mapview);
		CloseHandle(hMap);
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	//
	exports = get_export_table(mapview, ntheaders, sections, num_sects);
	if(!exports) {
		META_DEBUG(3, ("is_gamedll(%s): get_export_table() failed.", filename));
		UnmapViewOfFile(mapview);
		CloseHandle(hMap);
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	//
	unsigned long * names = (unsigned long *)va_to_mapaddr(mapview, sections, num_sects, exports->AddressOfNames);
	if(IsBadReadPtr(names, exports->NumberOfNames * sizeof(unsigned long))) {
		META_DEBUG(3, ("is_gamedll(%s): Pointer to exported function names is invalid.", filename));
		UnmapViewOfFile(mapview);
		CloseHandle(hMap);
		CloseHandle(hFile);
		return(mFALSE);
	}
	
	for(unsigned int i = 0; i < exports->NumberOfNames; i++) {
		//get function name with valid address
		char * funcname = (char *)va_to_mapaddr(mapview, sections, num_sects, names[i]);
		if(IsBadStringPtrA(funcname, 128))
			continue;
		
		// Check
		// Fast check for 'G' first
		if(funcname[0] == 'G') {
			// Collect export information
			if(!has_GiveFnptrsToDll)
				has_GiveFnptrsToDll = strmatch(funcname, "GiveFnptrsToDll");
			if(!has_GetEntityAPI2)
				has_GetEntityAPI2   = strmatch(funcname, "GetEntityAPI2");
	  		if(!has_GetEntityAPI)
	  			has_GetEntityAPI    = strmatch(funcname, "GetEntityAPI");
	  	}
		// Check if metamod plugin
		else if(funcname[0] == 'M') {
			if(strmatch(funcname, "Meta_Init") || 
			   strmatch(funcname, "Meta_Query") || 
			   strmatch(funcname, "Meta_Attach") || 
			   strmatch(funcname, "Meta_Detach")) {
				// Metamod plugin.. is not gamedll
				META_DEBUG(5, ("is_gamedll(%s): Detected Metamod plugin, library exports [%s].", filename, funcname));
		   		
				UnmapViewOfFile(mapview);
				CloseHandle(hMap);
				CloseHandle(hFile);
				
				return(mFALSE);
			}
		}
	}
	
	UnmapViewOfFile(mapview);
	CloseHandle(hMap);
	CloseHandle(hFile);
	
	// Check if gamedll
	if(has_GiveFnptrsToDll && (has_GetEntityAPI2 || has_GetEntityAPI)) {
		// This is gamedll!
		META_DEBUG(5, ("is_gamedll(%s): Detected GameDLL.", filename));

		return(mTRUE);
	} else {
		META_DEBUG(5, ("is_gamedll(%s): Library isn't GameDLL.", filename));
		
		return(mFALSE);
	}
}
