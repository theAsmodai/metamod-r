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
 
#ifndef COMP_DEP_H
#define COMP_DEP_H

#define DECLSPEC(kw)
#if defined (_WIN32) && defined (_MSC_VER)
	#define ATTRIBUTE(kw) 
#else
	#define ATTRIBUTE(kw) __attribute__((kw))
#endif
#define MM_CDECL

// We use these macros to hide our internal globals from being exported 
// on ELF .so
#if defined(__GNUC__) && !defined(_WIN32) && __GNUC__ >= 3 && __GNUC_MINOR__ >= 3
	// Hidden data/function.
	#define DLLHIDDEN __attribute__((visibility("hidden")))
	// Hidden internal function.
	#if defined(__x86_64__) || defined(__amd64__)
		#define DLLINTERNAL __attribute__((visibility("internal")))
		#define DLLINTERNAL_NOVIS
	#else	
		#ifdef __INTERNALS_USE_REGPARAMS__
			#define DLLINTERNAL __attribute__((visibility("internal"), regparm(3)))
			#define DLLINTERNAL_NOVIS __attribute__((regparm(3)))
		#else
			#define DLLINTERNAL __attribute__((visibility("internal")))
			#define DLLINTERNAL_NOVIS
		#endif
	#endif
#else
	#define DLLHIDDEN
	#if defined (_WIN32) && defined (_MSC_VER)
		#define DLLINTERNAL_NOVIS
		#define DLLINTERNAL
	#else
		#ifdef __INTERNALS_USE_REGPARAMS__
			#define DLLINTERNAL_NOVIS __attribute__((regparm(3)))
			#define DLLINTERNAL DLLINTERNAL_NOVIS
		#else
			#define DLLINTERNAL_NOVIS 
			#define DLLINTERNAL 
		#endif
	#endif //defined WIN32
#endif

#if defined (_WIN32) && defined (_MSC_VER)
	// On x86 va_list is just a pointer.
	#define va_copy(dst,src) ((dst)=(src))
#else
	// Some systems that do not supply va_copy have __va_copy instead, since 
	// that was the name used in the draft proposal.
	#if !defined(__GNUC__) || __GNUC__ < 3
		#define va_copy __va_copy
	#endif
#endif

// Manual branch optimization for GCC 3.0.0 and newer
#if !defined(__GNUC__) || __GNUC__ < 3
	#define likely(x) (x)
	#define unlikely(x) (x)
#else
	#define likely(x) __builtin_expect((long int)(x), true)
	#define unlikely(x) __builtin_expect((long int)(x), false)
#endif

#endif /*COMP_DEP_H*/
