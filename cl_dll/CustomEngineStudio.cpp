#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "com_model.h"
#include "studio.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "dlight.h"
#include "triangleapi.h"

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include "studio_util.h"
#include "CustomEngineStudio.h"

CCustomEngineStudio::CCustomEngineStudio()
{
}

void CCustomEngineStudio::Init(struct engine_studio_api_s *pstudio)
{
	m_pCvarCustomRenderer = gEngfuncs.pfnRegisterVariable("r_customrenderer", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);

	memcpy(&_engineStudio, pstudio, sizeof(_engineStudio));
}

void *CCustomEngineStudio::Mem_Calloc(int number, size_t size)
{
	return _engineStudio.Mem_Calloc(number, size);
}

void *CCustomEngineStudio::Cache_Check(struct cache_user_s *c)
{
	return _engineStudio.Cache_Check(c);
}

void CCustomEngineStudio::LoadCacheFile(char *path, struct cache_user_s *cu)
{
	_engineStudio.LoadCacheFile(path, cu);
}

struct model_s *CCustomEngineStudio::Mod_ForName(const char *name, int crash_if_missing)
{
	return _engineStudio.Mod_ForName(name, crash_if_missing);
}

void *CCustomEngineStudio::Mod_Extradata(struct model_s *mod)
{
	return _engineStudio.Mod_Extradata(mod);
}

struct model_s *CCustomEngineStudio::GetModelByIndex(int index)
{
	return _engineStudio.GetModelByIndex(index);
}

struct cl_entity_s *CCustomEngineStudio::GetCurrentEntity(void)
{
	return _engineStudio.GetCurrentEntity();
}

struct player_info_s *CCustomEngineStudio::PlayerInfo(int index)
{
	return _engineStudio.PlayerInfo(index);
}

struct entity_state_s *CCustomEngineStudio::GetPlayerState(int index)
{
	return _engineStudio.GetPlayerState(index);
}

struct cl_entity_s *CCustomEngineStudio::GetViewEntity(void)
{
	return _engineStudio.GetViewEntity();
}

void CCustomEngineStudio::GetTimes(int *framecount, double *current, double *old)
{
	_engineStudio.GetTimes(framecount, current, old);
}

struct cvar_s *CCustomEngineStudio::GetCvar(const char *name)
{
	return _engineStudio.GetCvar(name);
}

void CCustomEngineStudio::GetViewInfo(float *origin, float *upv, float *rightv, float *vpnv)
{
	_engineStudio.GetViewInfo(origin, upv, rightv, vpnv);
}

struct model_s *CCustomEngineStudio::GetChromeSprite(void)
{
	return _engineStudio.GetChromeSprite();
}

void CCustomEngineStudio::GetModelCounters(int **s, int **a)
{
	_engineStudio.GetModelCounters(s, a);
}

void CCustomEngineStudio::GetAliasScale(float *x, float *y)
{
	_engineStudio.GetAliasScale(x, y);
}

float ****CCustomEngineStudio::StudioGetBoneTransform(void)
{
	return _engineStudio.StudioGetBoneTransform();
}

float ****CCustomEngineStudio::StudioGetLightTransform(void)
{
	return _engineStudio.StudioGetLightTransform();
}

float ***CCustomEngineStudio::StudioGetAliasTransform(void)
{
	return _engineStudio.StudioGetAliasTransform();
}

float ***CCustomEngineStudio::StudioGetRotationMatrix(void)
{
	return _engineStudio.StudioGetRotationMatrix();
}

void CCustomEngineStudio::StudioSetupModel(int bodypart, void **ppbodypart, void **ppsubmodel)
{
	_engineStudio.StudioSetupModel(bodypart, ppbodypart, ppsubmodel);
}

int CCustomEngineStudio::StudioCheckBBox(void)
{
	return _engineStudio.StudioCheckBBox();
}

void CCustomEngineStudio::StudioDynamicLight(struct cl_entity_s *ent, struct alight_s *plight)
{
	_engineStudio.StudioDynamicLight(ent, plight);
}

void CCustomEngineStudio::StudioEntityLight(struct alight_s *plight)
{
	_engineStudio.StudioEntityLight(plight);
}

void CCustomEngineStudio::StudioSetupLighting(struct alight_s *plighting)
{
	_engineStudio.StudioSetupLighting(plighting);
}

void CCustomEngineStudio::StudioDrawPoints(void)
{
	if (m_pCvarCustomRenderer->value < 1)
	{
		_engineStudio.StudioDrawPoints();

		return;
	}

	/*
	for (int i = 0; i < m_pStudioHeader->numbodyparts; i++)
	{
		if (m_fDoInterp)
		{
			// interpolation messes up bounding boxes.
			m_pCurrentEntity->trivial_accept = 0;
		}

		IEngineStudio.GL_SetRenderMode(rendermode);

		mstudiobodyparts_t   *bodyPart = (mstudiobodyparts_t *)((byte *)m_pStudioHeader + m_pStudioHeader->bodypartindex) + i;

		int index = m_pCurrentEntity->curstate.body / bodyPart->base;
		index = index % bodyPart->nummodels;

		// mstudiobodyparts_t *bodyPart = (mstudiobodyparts_t *)((byte *)m_pStudioHeader + m_pStudioHeader->bodypartindex) + i;
		mstudiotexture_t *ptexture;
		studiohdr_t *texturesHeader;

		if (m_pStudioHeader->numtextures > 0)
		{
			texturesHeader = m_pStudioHeader;
		}
		else
		{
			char texturename[256];
			strcpy(texturename, m_pStudioHeader->name);
			strcpy(&texturename[strlen(texturename) - 4], "T.mdl");

			model_t *texturesModel = IEngineStudio.Mod_ForName(texturename, false);
			texturesHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata(texturesModel);
		}

		ptexture = (mstudiotexture_t *)((byte *)texturesHeader + texturesHeader->textureindex);
		short *pskinref = (short *)((byte *)texturesHeader + texturesHeader->skinindex);
		if (m_pCurrentEntity->curstate.skin != 0 && m_pCurrentEntity->curstate.skin < texturesHeader->numskinfamilies)
			pskinref += (m_pCurrentEntity->curstate.skin * texturesHeader->numskinref);

		// for (int j = 0; j < bodyPart->nummodels; j++)
		{
			mstudiomodel_t *model = (mstudiomodel_t *)((byte *)m_pStudioHeader + bodyPart->modelindex) + index;
			// m_pmodel = (mstudiomodel_t *)((byte *)m_pstudiohdr + pbodypart->modelindex) + index;

			vec3_t *pstudioverts = (vec3_t *)((byte *)m_pStudioHeader + model->vertindex);
			byte *pvertbone = ((byte *)m_pStudioHeader + model->vertinfoindex);

			for (int z = 0; z < model->numverts; z++)
			{
				VectorTransform(pstudioverts[z], (*m_pbonetransform)[pvertbone[z]], _transformedVerts[z]);
			}

			for (int k = 0; k < model->nummesh; k++)
			{
				mstudiomesh_t *mesh = (mstudiomesh_t *)((byte *)m_pStudioHeader + model->meshindex) + k;
				short *triangles = (short *)((byte *)m_pStudioHeader + mesh->triindex);
				vec3_t *vertices = (vec3_t *)((byte *)m_pStudioHeader + model->vertindex);

				float s = 1.0 / (float)ptexture[pskinref[mesh->skinref]].width;
				float t = 1.0 / (float)ptexture[pskinref[mesh->skinref]].height;

				glBindTexture(GL_TEXTURE_2D, ptexture[pskinref[mesh->skinref]].index);

				if (ptexture[pskinref[mesh->skinref]].flags & STUDIO_NF_CHROME)
				{
					while (int tri = *(triangles++))
					{
						if (tri < 0)
						{
							// gEngfuncs.pTriAPI->Begin(TRI_TRIANGLE_FAN);
							glBegin(GL_TRIANGLE_FAN);
							tri = -tri;
						}
						else
						{
							// gEngfuncs.pTriAPI->Begin(TRI_TRIANGLE_STRIP);
							glBegin(GL_TRIANGLE_STRIP);
						}

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

						for (; tri > 0; tri--, triangles += 4)
						{
							glTexCoord2f(triangles[2] * s, triangles[3] * t);

							vec3_t *vertex = &_transformedVerts[triangles[0]];
							glVertex3fv(vertex[0]);
						}

						glEnd();
					}
				}
				else
				{
					while (int tri = *(triangles++))
					{
						if (tri < 0)
						{
							// gEngfuncs.pTriAPI->Begin(TRI_TRIANGLE_FAN);
							glBegin(GL_TRIANGLE_FAN);
							tri = -tri;
						}
						else
						{
							// gEngfuncs.pTriAPI->Begin(TRI_TRIANGLE_STRIP);
							glBegin(GL_TRIANGLE_STRIP);
						}

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

						for (; tri > 0; tri--, triangles += 4)
						{
							glTexCoord2f(triangles[2] * s, triangles[3] * t);

							vec3_t *vertex = &_transformedVerts[triangles[0]];
							glVertex3fv(vertex[0]);
						}

						glEnd();
					}
				}
			}
		}
	}
	*/
}

void CCustomEngineStudio::StudioDrawHulls(void)
{
	_engineStudio.StudioDrawHulls();
}

void CCustomEngineStudio::StudioDrawAbsBBox(void)
{
	_engineStudio.StudioDrawAbsBBox();
}

void CCustomEngineStudio::StudioDrawBones(void)
{
	_engineStudio.StudioDrawBones();
}

void CCustomEngineStudio::StudioSetupSkin(void *ptexturehdr, int index)
{
	_engineStudio.StudioSetupSkin(ptexturehdr, index);
}

void CCustomEngineStudio::StudioSetRemapColors(int top, int bottom)
{
	_engineStudio.StudioSetRemapColors(top, bottom);
}

struct model_s *CCustomEngineStudio::SetupPlayerModel(int index)
{
	return _engineStudio.SetupPlayerModel(index);
}

void CCustomEngineStudio::StudioClientEvents(void)
{
	return _engineStudio.StudioClientEvents();
}

int CCustomEngineStudio::GetForceFaceFlags(void)
{
	return _engineStudio.GetForceFaceFlags();
}

void CCustomEngineStudio::SetForceFaceFlags(int flags)
{
	_engineStudio.SetForceFaceFlags(flags);
}

void CCustomEngineStudio::StudioSetHeader(void *header)
{
	_engineStudio.StudioSetHeader(header);
}

void CCustomEngineStudio::SetRenderModel(struct model_s *model)
{
	_engineStudio.SetRenderModel(model);
}

void CCustomEngineStudio::SetupRenderer(int rendermode)
{
	_engineStudio.SetupRenderer(rendermode);
}

void CCustomEngineStudio::RestoreRenderer(void)
{
	_engineStudio.RestoreRenderer();
}

void CCustomEngineStudio::SetChromeOrigin(void)
{
	_engineStudio.SetChromeOrigin();
}

int CCustomEngineStudio::IsHardware(void)
{
	return _engineStudio.IsHardware();
}

void CCustomEngineStudio::GL_StudioDrawShadow(void)
{
	_engineStudio.GL_StudioDrawShadow();
}

void CCustomEngineStudio::GL_SetRenderMode(int mode)
{
	_engineStudio.GL_SetRenderMode(mode);
}

void CCustomEngineStudio::StudioSetRenderamt(int iRenderamt)
{
	_engineStudio.StudioSetRenderamt(iRenderamt);
}

void CCustomEngineStudio::StudioSetCullState(int iCull)
{
	_engineStudio.StudioSetCullState(iCull);
}

void CCustomEngineStudio::StudioRenderShadow(int iSprite, float *p1, float *p2, float *p3, float *p4)
{
	_engineStudio.StudioRenderShadow(iSprite, p1, p2, p3, p4);
}