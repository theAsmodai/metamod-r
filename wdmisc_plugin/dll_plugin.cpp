// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dll_plugin.cpp - implementation of plugin's HL DLL routines

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
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

#include <extdll.h>
#include <sdk_util.h>

#include <meta_api.h>
#include <sdk_util.h>

#include "wdmisc.h"
#include "dll_plugin.h"
#include "log_plugin.h"

void wd_ServerDeactivate( void ) {
	bounce_check();
	RETURN_META(MRES_HANDLED);
}

static DLL_FUNCTIONS gFunctionTable = 
{
	NULL,						//! pfnGameInit()
	NULL,						//! pfnSpawn()
	NULL,						//! pfnThink()
	NULL,						//! pfnUse()
	NULL,						//! pfnTouch()
	NULL,						//! pfnBlocked()
	NULL,						//! pfnKeyValue()
	NULL,						//! pfnSave()
	NULL,						//! pfnRestore()
	NULL,						//! pfnSetAbsBox()

	NULL,						//! pfnSaveWriteFields()
	NULL,						//! pfnSaveReadFields()

	NULL,						//! pfnSaveGlobalState()
	NULL,						//! pfnRestoreGlobalState()
	NULL,						//! pfnResetGlobalState()

	NULL,						//! pfnClientConnect()
	NULL,						//! pfnClientDisconnect()
	NULL,						//! pfnClientKill()
	NULL,						//! pfnClientPutInServer()
	NULL,						//! pfnClientCommand()
	NULL,						//! pfnClientUserInfoChanged()
	NULL,						//! pfnServerActivate()
	wd_ServerDeactivate,		//! pfnServerDeactivate()

	NULL,						//! pfnPlayerPreThink()
	NULL,						//! pfnPlayerPostThink()

	NULL,						//! pfnStartFrame()
	NULL,						//! pfnParmsNewLevel()
	NULL,						//! pfnParmsChangeLevel()

	NULL,						//! pfnGetGameDescription()
	NULL,						//! pfnPlayerCustomization()

	NULL,						//! pfnSpectatorConnect()
	NULL,						//! pfnSpectatorDisconnect()
	NULL,						//! pfnSpectatorThink()
	
	NULL,						//! pfnSys_Error()

	NULL,						//! pfnPM_Move()
	NULL,						//! pfnPM_Init()
	NULL,						//! pfnPM_FindTextureType()
	
	NULL,						//! pfnSetupVisibility()
	NULL,						//! pfnUpdateClientData()
	NULL,						//! pfnAddToFullPack()
	NULL,						//! pfnCreateBaseline()
	NULL,						//! pfnRegisterEncoders()
	NULL,						//! pfnGetWeaponData()
	NULL,						//! pfnCmdStart()
	NULL,						//! pfnCmdEnd()
	NULL,						//! pfnConnectionlessPacket()
	NULL,						//! pfnGetHullBounds()
	NULL,						//! pfnCreateInstancedBaselines()
	NULL,						//! pfnInconsistentFile()
	NULL,						//! pfnAllowLagCompensation()
};

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, 
		int *interfaceVersion)
{
	LOG_DEVELOPER(PLID, "called: GetEntityAPI2; version=%d", *interfaceVersion);
	if(!pFunctionTable) {
		LOG_ERROR(PLID, "GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		LOG_ERROR(PLID, "GetEntityAPI2 version mismatch; requested=%d ours=%d",
				*interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is 
		//! out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return(TRUE);
}
