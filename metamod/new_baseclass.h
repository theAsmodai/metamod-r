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
#ifndef METAMOD_NEW_BASECLASS_H
#define METAMOD_NEW_BASECLASS_H

#include <malloc.h>

#include "comp_dep.h"

//new/delete operators with malloc/free to remove need for libstdc++

class class_metamod_new {
public:
	// Construction
	class_metamod_new(void) { };
	
	// Operators
	inline void * operator new(size_t size) {
		if(size==0)
			return(calloc(1, 1));
		return(calloc(1, size));
	}
	
	inline void * operator new[](size_t size) {
		if(size==0)
			return(calloc(1, 1));
		return(calloc(1, size));
	}
	
	inline void operator delete(void *ptr) {
		if(ptr)
			free(ptr); 
	}
	
	inline void operator delete[](void *ptr) {
		if(ptr)
			free(ptr); 
	}
};

#endif /*METAMOD_NEW_BASECLASS_H*/
