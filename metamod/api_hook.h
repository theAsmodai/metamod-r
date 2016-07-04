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
#ifndef API_HOOK_H
#define API_HOOK_H

#include "ret_type.h"
#include "api_info.h"
#include "meta_api.h"
#include "osdep.h"		//OPEN_ARGS

// Compine 4 parts for single name
#define _COMBINE4(w,x,y,z) w##x##y##z
#define _COMBINE2(x,y) x##y

// simplified 'void' version of main hook function
void DLLINTERNAL main_hook_function_void(unsigned int api_info_offset, enum_api_t api, unsigned int func_offset, const void * packed_args);

// full return typed version of main hook function
void * DLLINTERNAL main_hook_function(const class_ret_t ret_init, unsigned int api_info_offset, enum_api_t api, unsigned int func_offset, const void * packed_args);

//
// API function args structures/classes
//
#define API_PACK_ARGS(type, args) \
	_COMBINE2(pack_args_type_, type) packed_args args;

#define PACK_ARGS_CLASS_HEADER(type, constructor_args) \
	class _COMBINE2(pack_args_type_, type) : public class_metamod_new { \
		public: inline _COMBINE2(pack_args_type_, type) constructor_args

#define PACK_ARGS_END };

#define VOID_ARG 0

PACK_ARGS_CLASS_HEADER(void, (int)) {};
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(i, (int _i1)): i1(_i1) {}; 
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2i, (int _i1, int _i2)): i1(_i1), i2(_i2) {}; 
	int i1,i2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(3i, (int _i1, int _i2, int _i3)): i1(_i1), i2(_i2), i3(_i3) {};
	int i1,i2,i3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(ui, (unsigned int _ui1)): ui1(_ui1) {}; 
	unsigned int ui1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(ul, (unsigned long _ul1)): ul1(_ul1) {}; 
	unsigned long ul1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(f, (float _f1)): f1(_f1) {}; 
	float f1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2f, (float _f1, float _f2)): f1(_f1), f2(_f2) {}; 
	float f1,f2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p, (const void *_p1)): p1(_p1) {}; 
	const void *p1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2p, (const void *_p1, const void *_p2)): p1(_p1), p2(_p2) {}; 
	const void *p1,*p2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(3p, (const void *_p1, const void *_p2, const void *_p3)): p1(_p1), p2(_p2), p3(_p3) {}; 
	const void *p1,*p2,*p3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(4p, (const void *_p1, const void *_p2, const void *_p3, const void *_p4)): p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}; 
	const void *p1,*p2,*p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pV, (const void *_p1, const void *_p2, const void *_str)): p1(_p1), p2(_p2), str(_str) {}; 
	const void *p1,*p2,*str;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(ipV, (int _i1, const void *_p1, const void *_str)): i1(_i1), p1(_p1), str(_str) {}; 
	int i1;
	const void *p1,*str;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2i2p, (int _i1, int _i2, const void *_p1, const void *_p2)): i1(_i1), i2(_i2), p1(_p1), p2(_p2) {}; 
	int i1,i2;
	const void *p1,*p2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2p2f, (const void *_p1, const void *_p2, float _f1, float _f2)): p1(_p1), p2(_p2), f1(_f1), f2(_f2) {}; 
	const void *p1,*p2;
	float f1,f2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2p2i2p, (const void *_p1, const void *_p2, int _i1, int _i2, const void *_p3, const void *_p4)): p1(_p1), p2(_p2), i1(_i1), i2(_i2), p3(_p3), p4(_p4) {}; 
	const void *p1,*p2;
	int i1,i2;
	const void *p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2p3fus2uc, (const void *_p1, const void *_p2, float _f1, float _f2, float _f3, unsigned short _us1, unsigned char _uc1, unsigned char _uc2)): p1(_p1), p2(_p2), f1(_f1), f2(_f2), f3(_f3), us1(_us1), uc1(_uc1), uc2(_uc2) {}; 
	const void *p1,*p2;
	float f1,f2,f3;
	unsigned int us1;
	unsigned int uc1,uc2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pf, (const void *_p1, const void *_p2, float _f1)): p1(_p1), p2(_p2), f1(_f1) {}; 
	const void *p1,*p2;
	float f1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pfi, (const void *_p1, const void *_p2, float _f1, int _i1)): p1(_p1), p2(_p2), f1(_f1), i1(_i1) {}; 
	const void *p1,*p2;
	float f1;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pi, (const void *_p1, const void *_p2, int _i1)): p1(_p1), p2(_p2), i1(_i1) {}; 
	const void *p1,*p2;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pi2p, (const void *_p1, const void *_p2, int _i1, const void *_p3, const void *_p4)): p1(_p1), p2(_p2), i1(_i1), p3(_p3), p4(_p4) {}; 
	const void *p1,*p2;
	int i1;
	const void *p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pif2p, (const void *_p1, const void *_p2, int _i1, float _f1, const void *_p3, const void *_p4)): p1(_p1), p2(_p2), i1(_i1), f1(_f1), p3(_p3), p4(_p4) {}; 
	const void *p1,*p2;
	int i1;
	float f1;
	const void *p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(3p2f2i, (const void *_p1, const void *_p2, const void *_p3, float _f1, float _f2, int _i1, int _i2)): p1(_p1), p2(_p2), p3(_p3), f1(_f1), f2(_f2), i1(_i1), i2(_i2) {}; 
	const void *p1,*p2,*p3;	
	float f1,f2;
	int i1,i2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(3pi2p, (const void *_p1, const void *_p2, const void *_p3, int _i1, const void *_p4, const void *_p5)): p1(_p1), p2(_p2), p3(_p3), i1(_i1), p4(_p4), p5(_p5) {}; 
	const void *p1,*p2,*p3;	
	int i1;
	const void *p4,*p5;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(i3p, (int _i1, const void *_p1, const void *_p2, const void *_p3)): i1(_i1), p1(_p1), p2(_p2), p3(_p3) {}; 
	int i1;
	const void *p1,*p2,*p3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(ip, (int _i1, const void *_p1)): i1(_i1), p1(_p1) {}; 
	int i1;
	const void *p1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(ipusf2p2f4i, (int _i1, const void *_p1, unsigned short _us1, float _f1, const void *_p2, const void *_p3, float _f2, float _f3, int _i2, int _i3, int _i4, int _i5)): i1(_i1), p1(_p1), us1(_us1), f1(_f1), p2(_p2), p3(_p3), f2(_f2), f3(_f3), i2(_i2), i3(_i3), i4(_i4), i5(_i5) {}; 
	int i1;
	const void *p1;
	unsigned int us1;
	float f1;
	const void *p2,*p3;
	float f2,f3;
	int i2,i3,i4,i5;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(3pi, (const void *_p1, const void *_p2, const void *_p3, int _i1)): p1(_p1), p2(_p2), p3(_p3), i1(_i1) {}; 
	const void *p1,*p2,*p3;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(4pi, (const void *_p1, const void *_p2, const void *_p3, const void *_p4, int _i1)): p1(_p1), p2(_p2), p3(_p3), p4(_p4), i1(_i1) {}; 
	const void *p1,*p2,*p3,*p4;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pf, (const void *_p1, float _f1)): p1(_p1), f1(_f1) {}; 
	const void *p1;
	float f1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pfp, (const void *_p1, float _f1, const void *_p2)): p1(_p1), f1(_f1), p2(_p2) {}; 
	const void *p1;
	float f1;
	const void *p2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pi, (const void *_p1, int _i1)): p1(_p1), i1(_i1) {}; 
	const void *p1;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pi2p, (const void *_p1, int _i1, const void *_p2, const void *_p3)): p1(_p1), i1(_i1), p2(_p2), p3(_p3) {}; 
	const void *p1;
	int i1;
	const void *p2, *p3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pip, (const void *_p1, int _i1, const void *_p2)): p1(_p1), i1(_i1), p2(_p2) {}; 
	const void *p1;
	int i1;
	const void *p2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pip2f2i, (const void *_p1, int _i1, const void *_p2, float _f1, float _f2, int _i2, int _i3)): p1(_p1), i1(_i1), p2(_p2), f1(_f1), f2(_f2), i2(_i2), i3(_i3) {}; 
	const void *p1;
	int i1;
	const void *p2;
	float f1,f2;
	int i2,i3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pip2f4i2p, (const void *_p1, int _i1, const void *_p2, float _f1, float _f2, int _i2, int _i3, int _i4, int _i5, const void *_p3, const void *_p4)): p1(_p1), i1(_i1), p2(_p2), f1(_f1), f2(_f2), i2(_i2), i3(_i3), i4(_i4), i5(_i5), p3(_p3), p4(_p4) {}; 
	const void *p1;
	int i1;
	const void *p2;
	float f1,f2;
	int i2,i3,i4,i5;
	const void *p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(puc, (const void *_p1, unsigned char _uc1)): p1(_p1), uc1(_uc1) {}; 
	const void *p1;
	unsigned int uc1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2i2pi2p, (int _i1, int _i2, const void *_p1, const void *_p2, int _i3, const void *_p3, const void *_p4)): i1(_i1), i2(_i2), p1(_p1), p2(_p2), i3(_i3), p3(_p3), p4(_p4) {}; 
	int i1,i2;
	const void *p1,*p2;
	int i3;
	const void *p3,*p4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(2pui, (const void *_p1, const void *_p2, unsigned int _ui1)): p1(_p1), p2(_p2), ui1(_ui1) {}; 
	const void *p1,*p2;
	unsigned int ui1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(i2p, (int _i1, const void *_p1, const void *_p2)): i1(_i1), p1(_p1), p2(_p2) {}; 
	int i1;
	const void *p1,*p2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p2f, (const void *_p1, float _f1, float _f2)): p1(_p1), f1(_f1), f2(_f2) {}; 
	const void *p1;
	float f1,f2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p2fi, (const void *_p1, float _f1, float _f2, int _i1)): p1(_p1), f1(_f1), f2(_f2), i1(_i1) {}; 
	const void *p1;
	float f1,f2;
	int i1;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p2i, (const void *_p1, int _i1, int _i2)): p1(_p1), i1(_i1), i2(_i2) {}; 
	const void *p1;
	int i1,i2;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p3i, (const void *_p1, int _i1, int _i2, int _i3)): p1(_p1), i1(_i1), i2(_i2), i3(_i3) {}; 
	const void *p1;
	int i1,i2,i3;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(p4i, (const void *_p1, int _i1, int _i2, int _i3, int _i4)): p1(_p1), i1(_i1), i2(_i2), i3(_i3), i4(_i4) {}; 
	const void *p1;
	int i1,i2,i3,i4;
PACK_ARGS_END

PACK_ARGS_CLASS_HEADER(pi2p2ip, (const void *_p1, int _i1, const void *_p2, const void *_p3, int _i2, int _i3, const void *_p4)): p1(_p1), i1(_i1), p2(_p2), p3(_p3), i2(_i2), i3(_i3), p4(_p4) {}; 
	const void *p1;
	int i1;
	const void *p2,*p3;
	int i2,i3;
	const void *p4;
PACK_ARGS_END

//
// API function callers.
//
#ifdef __METAMOD_BUILD__
	#define EXTERN_API_CALLER_FUNCTION(ret_type, args_code) \
		void * DLLINTERNAL _COMBINE4(api_caller_, ret_type, _args_, args_code)(const void * func, const void * packed_args)
#else
	#define EXTERN_API_CALLER_FUNCTION(ret_type, args_code) \
		static const api_caller_func_t _COMBINE4(api_caller_, ret_type, _args_, args_code) DLLHIDDEN = (api_caller_func_t)0
#endif

EXTERN_API_CALLER_FUNCTION(void, ipV);
EXTERN_API_CALLER_FUNCTION(void, 2pV);
EXTERN_API_CALLER_FUNCTION(void, void);
EXTERN_API_CALLER_FUNCTION(ptr, void);
EXTERN_API_CALLER_FUNCTION(int, void);
EXTERN_API_CALLER_FUNCTION(float, void);
EXTERN_API_CALLER_FUNCTION(float, 2f);
EXTERN_API_CALLER_FUNCTION(void, 2i);
EXTERN_API_CALLER_FUNCTION(int, 2i);
EXTERN_API_CALLER_FUNCTION(void, 2i2p);
EXTERN_API_CALLER_FUNCTION(void, 2i2pi2p);
EXTERN_API_CALLER_FUNCTION(void, 2p);
EXTERN_API_CALLER_FUNCTION(ptr, 2p);
EXTERN_API_CALLER_FUNCTION(int, 2p);
EXTERN_API_CALLER_FUNCTION(void, 2p2f);
EXTERN_API_CALLER_FUNCTION(void, 2p2i2p);
EXTERN_API_CALLER_FUNCTION(void, 2p3fus2uc);
EXTERN_API_CALLER_FUNCTION(ptr, 2pf);
EXTERN_API_CALLER_FUNCTION(void, 2pfi);
EXTERN_API_CALLER_FUNCTION(void, 2pi);
EXTERN_API_CALLER_FUNCTION(int, 2pi);
EXTERN_API_CALLER_FUNCTION(void, 2pui);
EXTERN_API_CALLER_FUNCTION(void, 2pi2p);
EXTERN_API_CALLER_FUNCTION(void, 2pif2p);
EXTERN_API_CALLER_FUNCTION(int, 3i);
EXTERN_API_CALLER_FUNCTION(void, 3p);
EXTERN_API_CALLER_FUNCTION(ptr, 3p);
EXTERN_API_CALLER_FUNCTION(int, 3p);
EXTERN_API_CALLER_FUNCTION(void, 3p2f2i);
EXTERN_API_CALLER_FUNCTION(int, 3pi2p);
EXTERN_API_CALLER_FUNCTION(void, 4p);
EXTERN_API_CALLER_FUNCTION(int, 4p);
EXTERN_API_CALLER_FUNCTION(void, 4pi);
EXTERN_API_CALLER_FUNCTION(int, 4pi);
EXTERN_API_CALLER_FUNCTION(void, f);
EXTERN_API_CALLER_FUNCTION(void, i);
EXTERN_API_CALLER_FUNCTION(ptr, i);
EXTERN_API_CALLER_FUNCTION(int, i);
EXTERN_API_CALLER_FUNCTION(ptr, ui);
EXTERN_API_CALLER_FUNCTION(uint, ui);
EXTERN_API_CALLER_FUNCTION(ulong, ul);
EXTERN_API_CALLER_FUNCTION(void, i2p);
EXTERN_API_CALLER_FUNCTION(int, i2p);
EXTERN_API_CALLER_FUNCTION(void, i3p);
EXTERN_API_CALLER_FUNCTION(void, ip);
EXTERN_API_CALLER_FUNCTION(ushort, ip);
EXTERN_API_CALLER_FUNCTION(int, ip);
EXTERN_API_CALLER_FUNCTION(void, ipusf2p2f4i);
EXTERN_API_CALLER_FUNCTION(void, p);
EXTERN_API_CALLER_FUNCTION(ptr, p);
EXTERN_API_CALLER_FUNCTION(char, p);
EXTERN_API_CALLER_FUNCTION(int, p);
EXTERN_API_CALLER_FUNCTION(uint, p);
EXTERN_API_CALLER_FUNCTION(float, p);
EXTERN_API_CALLER_FUNCTION(void, p2f);
EXTERN_API_CALLER_FUNCTION(int, p2fi);
EXTERN_API_CALLER_FUNCTION(void, p2i);
EXTERN_API_CALLER_FUNCTION(void, p3i);
EXTERN_API_CALLER_FUNCTION(void, p4i);
EXTERN_API_CALLER_FUNCTION(void, puc);
EXTERN_API_CALLER_FUNCTION(void, pf);
EXTERN_API_CALLER_FUNCTION(void, pfp);
EXTERN_API_CALLER_FUNCTION(void, pi);
EXTERN_API_CALLER_FUNCTION(ptr, pi);
EXTERN_API_CALLER_FUNCTION(int, pi);
EXTERN_API_CALLER_FUNCTION(void, pi2p);
EXTERN_API_CALLER_FUNCTION(int, pi2p2ip);
EXTERN_API_CALLER_FUNCTION(void, pip);
EXTERN_API_CALLER_FUNCTION(ptr, pip);
EXTERN_API_CALLER_FUNCTION(void, pip2f2i);
EXTERN_API_CALLER_FUNCTION(void, pip2f4i2p);

#endif /*API_HOOK_H*/
