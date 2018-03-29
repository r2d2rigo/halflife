#if !defined( _RENDERERCVARS_H_ )
#define _RENDERERCVARS_H_
#if defined( _WIN32 )
#pragma once
#endif

#include "studio.h"
#include "r_studioint.h"

class CRendererCvars
{
public:
	cvar_t* mat_leafvis;
	cvar_t* r_visambient;

public:
	CRendererCvars(void);
	void RegisterCvars(void);
};

#endif