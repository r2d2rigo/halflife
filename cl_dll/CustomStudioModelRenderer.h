#if !defined( CUSTOMSTUDIOMODELRENDERER_H )
#define CUSTOMSTUDIOMODELRENDERER_H
#if defined( _WIN32 )
#pragma once
#endif

/*
====================
CGameStudioModelRenderer

====================
*/
class CCustomStudioModelRenderer : public CStudioModelRenderer
{
public:
	CCustomStudioModelRenderer(void);

public:
	void StudioRenderFinal_Hardware(void);
};

#endif // CUSTOMSTUDIOMODELRENDERER_H