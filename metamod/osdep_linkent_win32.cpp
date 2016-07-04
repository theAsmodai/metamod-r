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

#include <extdll.h>		// always
#include "osdep.h"

#include "log_meta.h"			// META_LOG, etc
#include "support_meta.h"


//
// Win32 code for dynamic linkents
//  -- by Jussi Kivilinna
//

// 
// Reads metamod.dll and game.dll function export tables and combines theim to 
// single table that replaces metamod.dll's original table.
// 

typedef struct sort_names_s {
	unsigned long  name;
	unsigned short nameOrdinal;
} sort_names_t;

//relative virtual address to virtual address
#define rva_to_va(base, rva) ((unsigned long)base + (unsigned long)rva)
//virtual address to relative virtual address
#define va_to_rva(base, va) ((unsigned long)va - (unsigned long)base)

//
// Checks module signatures and return ntheaders pointer for valid module
//
static IMAGE_NT_HEADERS * DLLINTERNAL_NOVIS get_ntheaders(HMODULE module)
{
	union { 
		unsigned long      mem;
		IMAGE_DOS_HEADER * dos;
		IMAGE_NT_HEADERS * pe;
	} mem;
	
	//Check if valid dos header
	mem.mem = (unsigned long)module;
	if(IsBadReadPtr(mem.dos, sizeof(*mem.dos)) || mem.dos->e_magic != IMAGE_DOS_SIGNATURE)
		return(0);
	
	//Get and check pe header
	mem.mem = rva_to_va(module, mem.dos->e_lfanew);
	if(IsBadReadPtr(mem.pe, sizeof(*mem.pe)) || mem.pe->Signature != IMAGE_NT_SIGNATURE)
		return(0);
	
	return(mem.pe);
}

//
// Returns export table for valid module
//
static IMAGE_EXPORT_DIRECTORY * DLLINTERNAL_NOVIS get_export_table(HMODULE module)
{
	union { 
		unsigned long            mem;
		void *                   pvoid;
		IMAGE_DOS_HEADER       * dos;
		IMAGE_NT_HEADERS       * pe;
		IMAGE_EXPORT_DIRECTORY * export_dir;
	} mem;
	
	//Check module
	mem.pe = get_ntheaders(module);
	if(!mem.pe)
		return(0);
	
	//Check for exports
	if(!mem.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
		return(0);
	
	mem.mem = rva_to_va(module, mem.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if(IsBadReadPtr(mem.export_dir, sizeof(*mem.export_dir)))
		return(0);
	
	return(mem.export_dir);
}

//
// Sort function for qsort
//
static int sort_names_list(const sort_names_t * A, const sort_names_t * B)
{
	const char * str_A = (const char *)A->name;
	const char * str_B = (const char *)B->name;
	
	return(mm_strcmp(str_A, str_B));
}

//
// Combines moduleMM and moduleGame export tables and replaces moduleMM table with new one
//
static int DLLINTERNAL_NOVIS combine_module_export_tables(HMODULE moduleMM, HMODULE moduleGame)
{
	IMAGE_EXPORT_DIRECTORY * exportMM;
	IMAGE_EXPORT_DIRECTORY * exportGame;
	
	unsigned long    newNumberOfFunctions;
	unsigned long    newNumberOfNames;
	unsigned long  * newFunctions;
	unsigned long  * newNames;
	unsigned short * newNameOrdinals;
	sort_names_t   * newSort;
	
	unsigned long i;
	unsigned long u;
	unsigned long funcCount;
	unsigned long nameCount;
	unsigned long listFix;
	
	//Get export tables
	exportMM   = get_export_table(moduleMM);
	exportGame = get_export_table(moduleGame);
	if(!exportMM || !exportGame)
	{
		META_ERROR("Couldn't initialize dynamic linkents, exportMM: %i, exportGame: %i.  Exiting...", exportMM, exportGame);
		return(0);
	}
	
	//setup new export table
	newNumberOfFunctions = exportMM->NumberOfFunctions + exportGame->NumberOfFunctions;
	newNumberOfNames     = exportMM->NumberOfNames     + exportGame->NumberOfNames;
	
	//alloc lists
	*(void**)&newFunctions = calloc(1, newNumberOfFunctions * sizeof(*newFunctions));
	*(void**)&newSort      = calloc(1, newNumberOfNames * sizeof(*newSort));
	
	//copy moduleMM to new export
	for(funcCount = 0; funcCount < exportMM->NumberOfFunctions; funcCount++)
		newFunctions[funcCount] = rva_to_va(moduleMM, ((unsigned long*)rva_to_va(moduleMM, exportMM->AddressOfFunctions))[funcCount]);
	for(nameCount = 0; nameCount < exportMM->NumberOfNames; nameCount++)
	{
		//fix name address
		newSort[nameCount].name = rva_to_va(moduleMM, ((unsigned long*)rva_to_va(moduleMM, exportMM->AddressOfNames))[nameCount]);
		//ordinal is index to function list
		newSort[nameCount].nameOrdinal = ((unsigned short *)rva_to_va(moduleMM, exportMM->AddressOfNameOrdinals))[nameCount];
	}
	
	//copy moduleGame to new export
	for(i = 0; i < exportGame->NumberOfFunctions; i++)
		newFunctions[funcCount + i] = rva_to_va(moduleGame, ((unsigned long*)rva_to_va(moduleGame, exportGame->AddressOfFunctions))[i]);
	for(i = 0, listFix = 0; i < exportGame->NumberOfNames; i++)
	{
		const char * name = (const char *)rva_to_va(moduleGame, ((unsigned long*)rva_to_va(moduleGame, exportGame->AddressOfNames))[i]);
		//Check if name already in the list
		for(u = 0; u < nameCount; u++)
		{
			if(!strcasecmp(name, (const char*)newSort[u].name))
			{
				listFix -= 1;
				break;
			}
		}
		if(u < nameCount) //already in the list.. skip
			continue;
		
		newSort[nameCount + i + listFix].name = (unsigned long)name;
		newSort[nameCount + i + listFix].nameOrdinal = (unsigned short)funcCount + ((unsigned short *)rva_to_va(moduleGame, exportGame->AddressOfNameOrdinals))[i];
	}
	
	//set new number
	newNumberOfNames = nameCount + i + listFix;
	
	//sort names list
	qsort(newSort, newNumberOfNames, sizeof(*newSort), (int(*)(const void*, const void*))&sort_names_list);
	
	//make newNames and newNameOrdinals lists (VirtualAlloc so we dont waste heap memory to stuff that isn't freed)
	*(void**)&newNames        = VirtualAlloc(0, newNumberOfNames * sizeof(*newNames), MEM_COMMIT, PAGE_READWRITE);
	*(void**)&newNameOrdinals = VirtualAlloc(0, newNumberOfNames * sizeof(*newNameOrdinals), MEM_COMMIT, PAGE_READWRITE);
	
	for(i = 0; i < newNumberOfNames; i++)
	{
		newNames[i]        = newSort[i].name;
		newNameOrdinals[i] = newSort[i].nameOrdinal;
	}
	
	free(newSort);
	
	//translate VAs to RVAs
	for(i = 0; i < newNumberOfFunctions; i++)
		newFunctions[i] = va_to_rva(moduleMM, newFunctions[i]);
	for(i = 0; i < newNumberOfNames; i++)
	{
		newNames[i] = va_to_rva(moduleMM, newNames[i]);
		newNameOrdinals[i] = (unsigned short)va_to_rva(moduleMM, newNameOrdinals[i]);
	}
	
	DWORD OldProtect;
	if(!VirtualProtect(exportMM, sizeof(*exportMM), PAGE_READWRITE, &OldProtect))
	{
		META_ERROR("Couldn't initialize dynamic linkents, VirtualProtect failed: %i.  Exiting...", GetLastError());
		return(0);
	}
	
	exportMM->Base              = 1;
	exportMM->NumberOfFunctions = newNumberOfFunctions;
	exportMM->NumberOfNames     = newNumberOfNames;
	*(unsigned long*)&(exportMM->AddressOfFunctions)    = va_to_rva(moduleMM, newFunctions);
	*(unsigned long*)&(exportMM->AddressOfNames)        = va_to_rva(moduleMM, newNames);
	*(unsigned long*)&(exportMM->AddressOfNameOrdinals) = va_to_rva(moduleMM, newNameOrdinals);
	
	VirtualProtect(exportMM, sizeof(*exportMM), OldProtect, &OldProtect);
	return(1);
}

//
// ...
//
int DLLINTERNAL init_linkent_replacement(DLHANDLE moduleMetamod, DLHANDLE moduleGame)
{
	return(combine_module_export_tables(moduleMetamod, moduleGame));
}
