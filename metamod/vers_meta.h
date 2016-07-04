// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// vers_meta.h - version info, intended to be common among DLLs distributed
// with metamod.

/*
 * Copyright (c) 2001-2013 Will Day <willday@hpgx.net>
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

#ifndef VERS_META_H
#define VERS_META_H

#ifndef OPT_TYPE
	#define OPT_TYPE	"default"
#endif /* not OPT_TYPE */


#define VDATE 			"2013/05/30"
#define VPATCH_COPYRIGHT_YEAR   "2013"
#define VMETA_VERSION		"1.21"

#define VPATCH_NAME		"Metamod-P (mm-p)"
#define VPATCH_IVERSION		37
#define VPATCH_VERSION		"37"
#define VPATCH_AUTHOR		"Jussi Kivilinna"
#define VPATCH_WEBSITE		"http://metamod-p.sourceforge.net/"

#define VVERSION		VMETA_VERSION "p" VPATCH_VERSION
#define RC_VERS_DWORD		1,21,0,VPATCH_IVERSION	// Version Windows DLL Resources in res_meta.rc



#endif /* VERS_META_H */
