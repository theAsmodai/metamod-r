#include "precompiled.h"

// Another GET-API routine, another interface version.
//
// Added 5/2003 based on information from Leon Hartwig and Alfred Reynolds,
// in order to address observed problems with hitboxes not matching between
// client and server, in CS and DOD.
//
// Based around code apparently added to the g_engine around 1/2002,
// seemingly to address "the old CS hitbox problem", where "the client was
// doing custom player model blending and the server had no idea what was
// going on".
//
// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg01224.html
// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg02724.html

C_DLLEXPORT int Server_GetBlendingInterface(int version,
		struct sv_blending_interface_s **ppinterface,
		struct engine_studio_api_s *pstudio,
		float (*rotationmatrix)[3][4],
		float (*bonetransform)[MAXSTUDIOBONES][3][4]) 
{
	static GETBLENDAPI_FN getblend = NULL;
	static int missing = 0;

	// Note that we're not checking if
	// (version==SV_BLENDING_INTERFACE_VERSION) because at this point, we
	// don't really care, as we're not looking at or using the contents of
	// the function tables; we're only passing them through to the gamedll,
	// which presumably will check for version match, since it's the one
	// that cares and actually uses the function tables.
	//
	// Return(0) if the gamedll does not provide this routine, and the
	// g_engine will use its own builtin blending.  The g_engine will report
	// "Couldn't get server .dll studio model blending interface.  Version
	// mismatch?", but this will only show in "developer" (-dev) mode.

	META_DEBUG(6, ("called: Server_GetBlendingInterface; version=%d", version));
	if (missing)
	{
		META_DEBUG(6, ("Skipping Server_GetBlendingInterface; was previously found missing"));
		return 0;
	}
	if (!getblend)
	{
		META_DEBUG(6, ("Looking up Server_GetBlendingInterface"));
		getblend = (GETBLENDAPI_FN)GameDLL.sys_module.getsym("Server_GetBlendingInterface");
	}
	if (!getblend)
	{
		META_DEBUG(6, ("Couldn't find Server_GetBlendingInterface in game DLL '%s': %s", GameDLL.name, "function not found"));
		missing = 1;
		return 0;
	}

	META_DEBUG(6, ("Calling Server_GetBlendingInterface"));
	return (getblend)(version, ppinterface, pstudio, rotationmatrix, bonetransform);
}
