#include "hud.h"
#include "cl_util.h"

#include "RendererCvars.h"

CRendererCvars::CRendererCvars()
{
}

void CRendererCvars::RegisterCvars()
{
	mat_leafvis = gEngfuncs.pfnRegisterVariable("mat_leafvis", "0", FCVAR_CLIENTDLL);
}