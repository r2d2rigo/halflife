//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"

#include "particleman.h"
#include "tri.h"

#define HARDWARE_MODE

#include "com_model.h"
#include "CustomEngineStudio.h"
#include "RendererCvars.h"
#include "GlAPI.h"

extern IParticleMan *g_pParticleMan;
extern CCustomEngineStudio IEngineStudio;
extern CRendererCvars RendererCvars;
extern CGlAPI GlAPI;

mnode_t	*PointInLeaf(mnode_t *node, vec3_t point)
{
	vec_t	d;

	if (node->contents)
		return node;

	d = DotProduct(node->plane->normal, point) - node->plane->dist;

	if (d > 0)
		return PointInLeaf(node->children[0], point);

	return PointInLeaf(node->children[1], point);
}

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void CL_DLLEXPORT HUD_DrawNormalTriangles(void)
{
	//	RecClDrawNormalTriangles();

	gHUD.m_Spectator.DrawOverview();


	if (RendererCvars.mat_leafvis->value > 0)
	{
		cl_entity_t *currentPlayer = gEngfuncs.GetLocalPlayer();
		model_s* worldModel = IEngineStudio.GetModelByIndex(1);

		if ((worldModel == NULL) || (worldModel->type != mod_brush))
		{
			return;
		}

		GlAPI.Disable(GlEnableMode::Texture2D);
		GlAPI.Disable(GlEnableMode::DepthTest);
		GlAPI.Disable(GlEnableMode::DepthWrite);

		GlAPI.Begin(GlBeginMode::Lines);
		GlAPI.Color4f(1.0f, 0.0f, 0.0f, 1.0f);

		mleaf_t* playerLeaf = (mleaf_t*)PointInLeaf(&worldModel->nodes[0], currentPlayer->origin);
		msurface_t **leafSurfaces = playerLeaf->firstmarksurface;

		for (int i = 0; i < playerLeaf->nummarksurfaces; i++)
		{
			msurface_t *currentSurface = (*leafSurfaces);

			for (int j = currentSurface->firstedge; j < currentSurface->firstedge + currentSurface->numedges; j++)
			{
				int surfaceEdge = worldModel->surfedges[j];
				medge_t edge = worldModel->edges[abs(surfaceEdge)];

				mvertex_t firstVertex = worldModel->vertexes[edge.v[0]];
				mvertex_t secondVertex = worldModel->vertexes[edge.v[1]];

				GlAPI.Vertex3fv(firstVertex.position);
				GlAPI.Vertex3fv(secondVertex.position);
			}

			leafSurfaces++;
		}

		GlAPI.End();

		GlAPI.Enable(GlEnableMode::DepthWrite);
		GlAPI.Enable(GlEnableMode::DepthTest);
		GlAPI.Enable(GlEnableMode::Texture2D);
	}
}

#if defined( _TFC )
void RunEventList( void );
#endif



/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void CL_DLLEXPORT HUD_DrawTransparentTriangles(void)
{
	//	RecClDrawTransparentTriangles();

#if defined( _TFC )
	RunEventList();
#endif

	if (g_pParticleMan)
		g_pParticleMan->Update();

}
