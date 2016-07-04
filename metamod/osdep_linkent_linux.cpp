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

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <dlfcn.h>
#include <sys/mman.h>
#define PAGE_SIZE 4096UL
#define PAGE_MASK (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr) (((addr)+PAGE_SIZE-1)&PAGE_MASK)
#include <pthread.h>
#include <link.h>

#include "osdep.h"
#include "osdep_p.h"
#include "log_meta.h"			// META_LOG, etc
#include "support_meta.h"

//
// Linux code for dynamic linkents
//  -- by Jussi Kivilinna
//

//opcode, e9, + sizeof pointer
#define BYTES_SIZE (1 + sizeof(void*))

typedef void * (*dlsym_func)(void * module, const char * funcname);

static void * gamedll_module_handle = 0;
static void * metamod_module_handle = 0;

//pointer to original dlsym
static dlsym_func dlsym_original;

//contains jmp to replacement_dlsym @dlsym_original
static unsigned char dlsym_new_bytes[BYTES_SIZE];

//contains original bytes of dlsym
static unsigned char dlsym_old_bytes[BYTES_SIZE];

//Mutex for our protection
static pthread_mutex_t mutex_replacement_dlsym = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

//constructs new jmp forwarder
inline void construct_jmp_instruction(void *x, void *place, void* target)
{
	((unsigned char *)x)[0] = 0xe9;
	*(unsigned long *)((char *)x + 1) = (unsigned long)target - ((unsigned long)place + 5);
}

//checks if pointer x points to jump forwarder
inline bool is_code_trampoline_jmp_opcode(void *x) 
{ 
	return (((unsigned char *)x)[0] == 0xff || ((unsigned char *)x)[1] == 0x25); 
}

//extracts pointer from "jmp dword ptr[pointer]"
inline void * extract_function_pointer_from_trampoline_jmp(void *x)
{
	return (**(void***)((char *)(x) + 2));
}

//
//restores old dlsym
//
inline void DLLINTERNAL restore_original_dlsym(void)
{
	//Copy old dlsym bytes back
	memcpy((void*)dlsym_original, dlsym_old_bytes, BYTES_SIZE);
}

//
//resets new dlsym
//
inline void DLLINTERNAL reset_dlsym_hook(void)
{
	//Copy new dlsym bytes back
	memcpy((void*)dlsym_original, dlsym_new_bytes, BYTES_SIZE);
}

//
// Replacement dlsym function
//
static void * __replacement_dlsym(void * module, const char * funcname)
{
	//these are needed in case dlsym calls dlsym, default one doesn't do
	//it but some LD_PRELOADed library that hooks dlsym might actually
	//do so.
	static int is_original_restored = 0;
	int was_original_restored = is_original_restored;
	
	//Lock before modifing original dlsym
	pthread_mutex_lock(&mutex_replacement_dlsym);
	
	//restore old dlsym
	if(!is_original_restored)
	{
		restore_original_dlsym();
		
		is_original_restored = 1;
	}
		
	//check if we should hook this call
	if(module != metamod_module_handle || !metamod_module_handle || !gamedll_module_handle)
	{
		//no metamod/gamedll module? should we remove hook now?
		void * retval = dlsym_original(module, funcname);
		
		if(metamod_module_handle && gamedll_module_handle)
		{
			if(!was_original_restored)
			{
				//reset dlsym hook
				reset_dlsym_hook();
				
				is_original_restored = 0;
			}
		}
		else
		{
			//no metamod/gamedll module? should we remove hook now by not reseting it back?
		}
		
		//unlock
		pthread_mutex_unlock(&mutex_replacement_dlsym);
		
		return(retval);
	}
	
	//dlsym on metamod module
	void * func = dlsym_original(module, funcname);
	
	if(!func)
	{
		//function not in metamod module, try gamedll
		func = dlsym_original(gamedll_module_handle, funcname);
	}
	
	if(!was_original_restored)
	{
		//reset dlsym hook
		reset_dlsym_hook();
		
		is_original_restored = 0;
	}
	
	//unlock
	pthread_mutex_unlock(&mutex_replacement_dlsym);
	
	return(func);
}

//
// Initialize
//
int DLLINTERNAL init_linkent_replacement(DLHANDLE MetamodHandle, DLHANDLE GameDllHandle)
{
	metamod_module_handle = MetamodHandle;
	gamedll_module_handle = GameDllHandle;
	
	// dlsym is already known to be pointing to valid function, we loaded gamedll using it earlier!
	void * sym_ptr = (void*)&dlsym;
	while(is_code_trampoline_jmp_opcode(sym_ptr)) {
		sym_ptr = extract_function_pointer_from_trampoline_jmp(sym_ptr);
	}
	
	dlsym_original = (dlsym_func)sym_ptr;
	
	//Backup old bytes of "dlsym" function
	memcpy(dlsym_old_bytes, (void*)dlsym_original, BYTES_SIZE);
	
	//Construct new bytes: "jmp offset[replacement_sendto] @ sendto_original"
	construct_jmp_instruction((void*)&dlsym_new_bytes[0], (void*)dlsym_original, (void*)&__replacement_dlsym);
	
	//Check if bytes overlap page border.	
	unsigned long start_of_page = PAGE_ALIGN((long)dlsym_original) - PAGE_SIZE;
	unsigned long size_of_pages = 0;
	
	if((unsigned long)dlsym_original + BYTES_SIZE > PAGE_ALIGN((unsigned long)dlsym_original))
	{
		//bytes are located on two pages
		size_of_pages = PAGE_SIZE*2;
	}
	else
	{
		//bytes are located entirely on one page.
		size_of_pages = PAGE_SIZE;
	}
	
	//Remove PROT_READ restriction
	if(mprotect((void*)start_of_page, size_of_pages, PROT_READ|PROT_WRITE|PROT_EXEC))
	{
		META_ERROR("Couldn't initialize dynamic linkents, mprotect failed: %i.  Exiting...", errno);
		return(0);
	}
	
	//Write our own jmp-forwarder on "dlsym"
	reset_dlsym_hook();
		
	//done
	return(1);
}
