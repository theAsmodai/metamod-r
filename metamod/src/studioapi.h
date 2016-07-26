#pragma once

#include <studio.h>

// Typedef for Server_GetBlendingInterface() from Eric Smith on the hlcoders
// mailing list.
typedef int (*GETBLENDAPI_FN)(int version,
		struct sv_blending_interface_s **ppinterface,
		struct engine_studio_api_s *pstudio,
		float (*rotationmatrix)[3][4],
		float (*bonetransform)[MAXSTUDIOBONES][3][4]);

extern int mm_Server_GetBlendingInterface(int version,
		struct sv_blending_interface_s **ppinterface,
		struct engine_studio_api_s *pstudio,
		float (*rotationmatrix)[3][4],
		float (*bonetransform)[MAXSTUDIOBONES][3][4]);
