#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "com_model.h"
#include "studio.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "dlight.h"

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include "studio_util.h"
#include "CustomEngineStudio.h"

CCustomEngineStudio::CCustomEngineStudio()
{
}

GLuint shaderProgram;

void CCustomEngineStudio::Init(struct engine_studio_api_s *pstudio)
{
	glewInit();

	m_pCvarCustomRenderer = gEngfuncs.pfnRegisterVariable("r_customrenderer", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);

	GLuint vertexShader, fragmentShader;

	vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	int fileLength;
	const char* fileContents = (const char*)gEngfuncs.COM_LoadFile("shaders/SimpleShader.vert", 5, &fileLength);
	glShaderSourceARB(vertexShader, 1, &fileContents, &fileLength);
	glCompileShaderARB(vertexShader);
	gEngfuncs.COM_FreeFile((void*)fileContents);

	fileContents = (const char*)gEngfuncs.COM_LoadFile("shaders/SimpleShader.frag", 5, &fileLength);
	glShaderSourceARB(fragmentShader, 1, &fileContents, &fileLength);
	glCompileShaderARB(fragmentShader);
	gEngfuncs.COM_FreeFile((void*)fileContents);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	memcpy(&m_pEngineStudio, pstudio, sizeof(m_pEngineStudio));
}

void *CCustomEngineStudio::Mem_Calloc(int number, size_t size)
{
	return m_pEngineStudio.Mem_Calloc(number, size);
}

void *CCustomEngineStudio::Cache_Check(struct cache_user_s *c)
{
	return m_pEngineStudio.Cache_Check(c);
}

void CCustomEngineStudio::LoadCacheFile(char *path, struct cache_user_s *cu)
{
	m_pEngineStudio.LoadCacheFile(path, cu);
}

struct model_s *CCustomEngineStudio::Mod_ForName(const char *name, int crash_if_missing)
{
	return m_pEngineStudio.Mod_ForName(name, crash_if_missing);
}

void *CCustomEngineStudio::Mod_Extradata(struct model_s *mod)
{
	return m_pEngineStudio.Mod_Extradata(mod);
}

struct model_s *CCustomEngineStudio::GetModelByIndex(int index)
{
	return m_pEngineStudio.GetModelByIndex(index);
}

struct cl_entity_s *CCustomEngineStudio::GetCurrentEntity(void)
{
	m_pCurrentEntity = m_pEngineStudio.GetCurrentEntity();

	return m_pCurrentEntity;
}

struct player_info_s *CCustomEngineStudio::PlayerInfo(int index)
{
	return m_pEngineStudio.PlayerInfo(index);
}

struct entity_state_s *CCustomEngineStudio::GetPlayerState(int index)
{
	return m_pEngineStudio.GetPlayerState(index);
}

struct cl_entity_s *CCustomEngineStudio::GetViewEntity(void)
{
	return m_pEngineStudio.GetViewEntity();
}

void CCustomEngineStudio::GetTimes(int *framecount, double *current, double *old)
{
	m_pEngineStudio.GetTimes(framecount, current, old);
}

struct cvar_s *CCustomEngineStudio::GetCvar(const char *name)
{
	return m_pEngineStudio.GetCvar(name);
}

void CCustomEngineStudio::GetViewInfo(float *origin, float *upv, float *rightv, float *vpnv)
{
	m_pEngineStudio.GetViewInfo(origin, upv, rightv, vpnv);
}

struct model_s *CCustomEngineStudio::GetChromeSprite(void)
{
	return m_pEngineStudio.GetChromeSprite();
}

void CCustomEngineStudio::GetModelCounters(int **s, int **a)
{
	m_pEngineStudio.GetModelCounters(s, a);

	m_pStudioModelCount = *s;
	m_pModelsDrawn = *a;
}

void CCustomEngineStudio::GetAliasScale(float *x, float *y)
{
	m_pEngineStudio.GetAliasScale(x, y);
}

float ****CCustomEngineStudio::StudioGetBoneTransform(void)
{
	m_pBoneTransforms = (float(*)[MAXSTUDIOBONES][3][4])m_pEngineStudio.StudioGetBoneTransform();

	return (float****)m_pBoneTransforms;
}

float ****CCustomEngineStudio::StudioGetLightTransform(void)
{
	m_pLightTransforms = (float(*)[MAXSTUDIOBONES][3][4])m_pEngineStudio.StudioGetLightTransform();

	return (float****)m_pLightTransforms;
}

float ***CCustomEngineStudio::StudioGetAliasTransform(void)
{
	return m_pEngineStudio.StudioGetAliasTransform();
}

float ***CCustomEngineStudio::StudioGetRotationMatrix(void)
{
	m_pRotationMatrix = (float(*)[3][4])m_pEngineStudio.StudioGetRotationMatrix();
	
	return (float***)m_pRotationMatrix;
}

void CCustomEngineStudio::StudioSetupModel(int bodypart, void **ppbodypart, void **ppsubmodel)
{
	m_pEngineStudio.StudioSetupModel(bodypart, ppbodypart, ppsubmodel);

	m_pBodyPart = (mstudiobodyparts_t *)((byte *)m_pStudioHeader + m_pStudioHeader->bodypartindex) + bodypart;

	int modelIndex = m_pCurrentEntity->curstate.body / m_pBodyPart->base;
	modelIndex = modelIndex % m_pBodyPart->nummodels;

	m_pSubModel = (mstudiomodel_t *)((byte *)m_pStudioHeader + m_pBodyPart->modelindex) + modelIndex;
}

int CCustomEngineStudio::StudioCheckBBox(void)
{
	return m_pEngineStudio.StudioCheckBBox();
}

void CCustomEngineStudio::StudioDynamicLight(struct cl_entity_s *ent, struct alight_s *plight)
{
	m_pEngineStudio.StudioDynamicLight(ent, plight);
}

void CCustomEngineStudio::StudioEntityLight(struct alight_s *plight)
{
	m_pEngineStudio.StudioEntityLight(plight);
}

void CCustomEngineStudio::StudioSetupLighting(struct alight_s *plighting)
{
	m_pEngineStudio.StudioSetupLighting(plighting);

	m_pLighting = plighting;

	for (int i = 0; i < m_pStudioHeader->numbones; i++)
	{
		VectorIRotate(m_pLighting->plightvec, (*m_pLightTransforms)[i], m_vBoneLightVectors[i]);
	}
}

void CCustomEngineStudio::StudioDrawPoints(void)
{
	if (m_pCvarCustomRenderer->value ==0 )
	{
		m_pEngineStudio.StudioDrawPoints();
	}
	else if (m_pCvarCustomRenderer->value == 1)
	{
		StudioDrawPointsFixedPipeline();
	}
	else if (m_pCvarCustomRenderer->value == 2)
	{
		StudioDrawPointsProgrammablePipeline();
	}
}

void CCustomEngineStudio::StudioDrawPointsFixedPipeline(void)
{
	mstudiotexture_t * pTextures = (mstudiotexture_t *)((byte *)m_pTextureHeader + m_pTextureHeader->textureindex);
	short *pSkinReferences = (short *)((byte *)m_pTextureHeader + m_pTextureHeader->skinindex);

	if (m_pCurrentEntity->curstate.skin != 0 && m_pCurrentEntity->curstate.skin < m_pTextureHeader->numskinfamilies)
	{
		pSkinReferences += (m_pCurrentEntity->curstate.skin * m_pTextureHeader->numskinref);
	}

	vec3_t *pModelVertices = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->vertindex);
	byte *pVertexBoneIndices = ((byte *)m_pStudioHeader + m_pSubModel->vertinfoindex);

	vec3_t *pModelNormals = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->normindex);
	byte *pNormalBoneIndices = ((byte *)m_pStudioHeader + m_pSubModel->norminfoindex);

	for (int i = 0; i < m_pSubModel->numverts; i++)
	{
		VectorTransform(pModelVertices[i], (*m_pBoneTransforms)[pVertexBoneIndices[i]], m_vTransformedVertices[i]);
	}

	vec3_t *pLightValues = &m_vLightValues[0];

	for (int i = 0; i < m_pSubModel->nummesh; i++)
	{
		mstudiomesh_t *pSubModelMesh = (mstudiomesh_t *)((byte *)m_pStudioHeader + m_pSubModel->meshindex) + i;
		mstudiotexture_t *pMeshTexture = &pTextures[pSkinReferences[pSubModelMesh->skinref]];

		for (int j = 0; j < pSubModelMesh->numnorms; j++, pLightValues++, pModelNormals++, pNormalBoneIndices++)
		{
			float lightValue = m_pLighting->ambientlight;

			if (pMeshTexture->flags & STUDIO_NF_FLATSHADE)
			{
				lightValue += m_pLighting->shadelight * 0.8;
			}
			else
			{
				float r;
				float lightcos = DotProduct((*pModelNormals), m_vBoneLightVectors[*pNormalBoneIndices]); // -1 colinear, 1 opposite

				if (lightcos > 1.0)
					lightcos = 1;

				lightValue += m_pLighting->shadelight;

				r = 1.5;
				if (r <= 1.0) r = 1.0;

				lightcos = (lightcos + (r - 1.0)) / r; 		// do modified hemispherical lighting
				if (lightcos > 0.0)
				{
					lightValue -= m_pLighting->shadelight * lightcos;
				}
				if (lightValue <= 0)
					lightValue = 0;
			}

			if (lightValue > 255)
				lightValue = 255;

			lightValue = lightValue / 255.0;

			// FIX: move this check out of the inner loop
			if (pMeshTexture->flags & STUDIO_NF_CHROME)
			{
				float n;
				vec3_t origin, angles, forward, up, right;
				m_pEngineStudio.GetViewInfo((float*)&origin, (float*)&forward, (float*)&right, (float*)&up);

				if (m_iChromeAges[*pNormalBoneIndices] != *m_pStudioModelCount)
				{
					vec3_t chromeupvec;
					vec3_t chromerightvec;
					vec3_t tmp;
					VectorScale(origin, -1, tmp);
					tmp[0] += (*m_pBoneTransforms)[*pNormalBoneIndices][0][3];
					tmp[1] += (*m_pBoneTransforms)[*pNormalBoneIndices][1][3];
					tmp[2] += (*m_pBoneTransforms)[*pNormalBoneIndices][2][3];
					VectorNormalize(tmp);
					CrossProduct(tmp, right, chromeupvec);
					VectorNormalize(chromeupvec);
					CrossProduct(tmp, chromeupvec, chromerightvec);
					VectorNormalize(chromerightvec);

					VectorIRotate(chromeupvec, (*m_pBoneTransforms)[*pNormalBoneIndices], m_vChromeUpVectors[*pNormalBoneIndices]);
					VectorIRotate(chromerightvec, (*m_pBoneTransforms)[*pNormalBoneIndices], m_vChromeRightVectors[*pNormalBoneIndices]);

					m_iChromeAges[*pNormalBoneIndices] = *m_pStudioModelCount;
				}

				int *pChrome = m_iChromeTexCoords[pLightValues - &m_vLightValues[0]];
				n = DotProduct(*pModelNormals, m_vChromeRightVectors[*pNormalBoneIndices]);
				pChrome[0] = (n + 1.0) * 32;

				n = DotProduct(*pModelNormals, m_vChromeUpVectors[*pNormalBoneIndices]);
				pChrome[1] = (n + 1.0) * 32;
			}

			pLightValues->x = lightValue * m_pLighting->color[0];
			pLightValues->y = lightValue * m_pLighting->color[1];
			pLightValues->z = lightValue * m_pLighting->color[2];
		}

		short *pMeshTriangles = (short *)((byte *)m_pStudioHeader + pSubModelMesh->triindex);

		float s = 1.0 / (float)pMeshTexture->width;
		float t = 1.0 / (float)pMeshTexture->height;

		glBindTexture(GL_TEXTURE_2D, pMeshTexture->index);

		while (int triangleCount = *(pMeshTriangles++))
		{
			GLenum primitiveMode = GL_TRIANGLE_STRIP;

			if (triangleCount < 0)
			{
				primitiveMode = GL_TRIANGLE_FAN;
				triangleCount = -triangleCount;
			}

			glBegin(primitiveMode);

			while (triangleCount > 0)
			{
				vec3_t *vertexLightValue = &m_vLightValues[pMeshTriangles[1]];
				glColor4f(vertexLightValue->x, vertexLightValue->y, vertexLightValue->z, 1.0f);

				if (pMeshTexture->flags & STUDIO_NF_CHROME)
				{
					glTexCoord2f(m_iChromeTexCoords[pMeshTriangles[1]][0] * s, m_iChromeTexCoords[pMeshTriangles[1]][1] * t);
				}
				else
				{
					glTexCoord2f(pMeshTriangles[2] * s, pMeshTriangles[3] * t);
				}

				glVertex3fv(m_vTransformedVertices[pMeshTriangles[0]]);

				triangleCount--;
				pMeshTriangles += 4;
			}

			glEnd();
		}
	}
}

void CCustomEngineStudio::StudioDrawPointsProgrammablePipeline(void)
{
	glUseProgram(shaderProgram);

	mstudiotexture_t * pTextures = (mstudiotexture_t *)((byte *)m_pTextureHeader + m_pTextureHeader->textureindex);
	short *pSkinReferences = (short *)((byte *)m_pTextureHeader + m_pTextureHeader->skinindex);

	if (m_pCurrentEntity->curstate.skin != 0 && m_pCurrentEntity->curstate.skin < m_pTextureHeader->numskinfamilies)
	{
		pSkinReferences += (m_pCurrentEntity->curstate.skin * m_pTextureHeader->numskinref);
	}

	vec3_t *pModelVertices = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->vertindex);
	byte *pVertexBoneIndices = ((byte *)m_pStudioHeader + m_pSubModel->vertinfoindex);

	vec3_t *pModelNormals = (vec3_t *)((byte *)m_pStudioHeader + m_pSubModel->normindex);
	byte *pNormalBoneIndices = ((byte *)m_pStudioHeader + m_pSubModel->norminfoindex);

	for (int i = 0; i < m_pSubModel->numverts; i++)
	{
		VectorTransform(pModelVertices[i], (*m_pBoneTransforms)[pVertexBoneIndices[i]], m_vTransformedVertices[i]);
	}

	for (int i = 0; i < m_pSubModel->numnorms; i++)
	{
		VectorRotate(pModelNormals[i], (*m_pBoneTransforms)[pNormalBoneIndices[i]], m_vTransformedNormals[i]);
	}

	vec3_t *pLightValues = &m_vLightValues[0];

	for (int i = 0; i < m_pSubModel->nummesh; i++)
	{
		mstudiomesh_t *pSubModelMesh = (mstudiomesh_t *)((byte *)m_pStudioHeader + m_pSubModel->meshindex) + i;
		mstudiotexture_t *pMeshTexture = &pTextures[pSkinReferences[pSubModelMesh->skinref]];

		for (int j = 0; j < pSubModelMesh->numnorms; j++, pLightValues++, pModelNormals++, pNormalBoneIndices++)
		{
			// FIX: move this check out of the inner loop
			if (pMeshTexture->flags & STUDIO_NF_CHROME)
			{
				float n;
				vec3_t origin, angles, forward, up, right;
				m_pEngineStudio.GetViewInfo((float*)&origin, (float*)&forward, (float*)&right, (float*)&up);

				if (m_iChromeAges[*pNormalBoneIndices] != *m_pStudioModelCount)
				{
					vec3_t chromeupvec;
					vec3_t chromerightvec;
					vec3_t tmp;
					VectorScale(origin, -1, tmp);
					tmp[0] += (*m_pBoneTransforms)[*pNormalBoneIndices][0][3];
					tmp[1] += (*m_pBoneTransforms)[*pNormalBoneIndices][1][3];
					tmp[2] += (*m_pBoneTransforms)[*pNormalBoneIndices][2][3];
					VectorNormalize(tmp);
					CrossProduct(tmp, right, chromeupvec);
					VectorNormalize(chromeupvec);
					CrossProduct(tmp, chromeupvec, chromerightvec);
					VectorNormalize(chromerightvec);

					VectorIRotate(chromeupvec, (*m_pBoneTransforms)[*pNormalBoneIndices], m_vChromeUpVectors[*pNormalBoneIndices]);
					VectorIRotate(chromerightvec, (*m_pBoneTransforms)[*pNormalBoneIndices], m_vChromeRightVectors[*pNormalBoneIndices]);

					m_iChromeAges[*pNormalBoneIndices] = *m_pStudioModelCount;
				}

				int *pChrome = m_iChromeTexCoords[pLightValues - &m_vLightValues[0]];
				n = DotProduct(*pModelNormals, m_vChromeRightVectors[*pNormalBoneIndices]);
				pChrome[0] = (n + 1.0) * 32;

				n = DotProduct(*pModelNormals, m_vChromeUpVectors[*pNormalBoneIndices]);
				pChrome[1] = (n + 1.0) * 32;
			}
		}

		short *pMeshTriangles = (short *)((byte *)m_pStudioHeader + pSubModelMesh->triindex);

		float s = 1.0 / (float)pMeshTexture->width;
		float t = 1.0 / (float)pMeshTexture->height;

		glBindTexture(GL_TEXTURE_2D, pMeshTexture->index);

		GLint ambientLightLocation = glGetUniformLocation(shaderProgram, "AmbientLight");
		GLint shadeLightLocation = glGetUniformLocation(shaderProgram, "ShadeLight");
		GLint lightDirLocation = glGetUniformLocation(shaderProgram, "LightDir");
		GLint lightColorLocation = glGetUniformLocation(shaderProgram, "LightColor");

		/*
		* TODO: take into account flat shaded surfaces
		if (pMeshTexture->flags & STUDIO_NF_FLATSHADE)
		{
			lightValue += m_pLighting->shadelight * 0.8;
		}
		*/

		glUniform1f(ambientLightLocation, m_pLighting->ambientlight / 255.0f);
		glUniform1f(shadeLightLocation, m_pLighting->shadelight / 255.0f);
		glUniform3fv(lightDirLocation, 1, m_pLighting->plightvec);
		glUniform3fv(lightColorLocation, 1, m_pLighting->color);

		while (int triangleCount = *(pMeshTriangles++))
		{
			GLenum primitiveMode = GL_TRIANGLE_STRIP;

			if (triangleCount < 0)
			{
				primitiveMode = GL_TRIANGLE_FAN;
				triangleCount = -triangleCount;
			}

			glBegin(primitiveMode);

			while (triangleCount > 0)
			{
				vec3_t *vertexLightValue = &m_vLightValues[pMeshTriangles[1]];

				if (pMeshTexture->flags & STUDIO_NF_CHROME)
				{
					glTexCoord2f(m_iChromeTexCoords[pMeshTriangles[1]][0] * s, m_iChromeTexCoords[pMeshTriangles[1]][1] * t);
				}
				else
				{
					glTexCoord2f(pMeshTriangles[2] * s, pMeshTriangles[3] * t);
				}

				glNormal3fv(m_vTransformedNormals[pMeshTriangles[1]]);
				glVertex3fv(m_vTransformedVertices[pMeshTriangles[0]]);

				triangleCount--;
				pMeshTriangles += 4;
			}

			glEnd();
		}
	}

	glUseProgram(0);
}

void CCustomEngineStudio::StudioDrawHulls(void)
{
	m_pEngineStudio.StudioDrawHulls();
}

void CCustomEngineStudio::StudioDrawAbsBBox(void)
{
	m_pEngineStudio.StudioDrawAbsBBox();
}

void CCustomEngineStudio::StudioDrawBones(void)
{
	m_pEngineStudio.StudioDrawBones();
}

void CCustomEngineStudio::StudioSetupSkin(void *ptexturehdr, int index)
{
	m_pEngineStudio.StudioSetupSkin(ptexturehdr, index);
}

void CCustomEngineStudio::StudioSetRemapColors(int top, int bottom)
{
	m_pEngineStudio.StudioSetRemapColors(top, bottom);
}

struct model_s *CCustomEngineStudio::SetupPlayerModel(int index)
{
	return m_pEngineStudio.SetupPlayerModel(index);
}

void CCustomEngineStudio::StudioClientEvents(void)
{
	return m_pEngineStudio.StudioClientEvents();
}

int CCustomEngineStudio::GetForceFaceFlags(void)
{
	return m_pEngineStudio.GetForceFaceFlags();
}

void CCustomEngineStudio::SetForceFaceFlags(int flags)
{
	m_pEngineStudio.SetForceFaceFlags(flags);
}

void CCustomEngineStudio::StudioSetHeader(void *header)
{
	m_pEngineStudio.StudioSetHeader(header);

	m_pStudioHeader = (studiohdr_t *)header;
	m_pTextureHeader = m_pStudioHeader;

	if (m_pTextureHeader->numtextures == 0)
	{
		char texturesFilename[256];
		strcpy(texturesFilename, m_pTextureHeader->name);
		strcpy(&texturesFilename[strlen(texturesFilename) - 4], "T.mdl");

		model_t *texturesModel = m_pEngineStudio.Mod_ForName(texturesFilename, false);
		m_pTextureHeader = (studiohdr_t *)m_pEngineStudio.Mod_Extradata(texturesModel);
	}
}

void CCustomEngineStudio::SetRenderModel(struct model_s *model)
{
	m_pEngineStudio.SetRenderModel(model);
}

void CCustomEngineStudio::SetupRenderer(int rendermode)
{
	m_pEngineStudio.SetupRenderer(rendermode);
}

void CCustomEngineStudio::RestoreRenderer(void)
{
	m_pEngineStudio.RestoreRenderer();
}

void CCustomEngineStudio::SetChromeOrigin(void)
{
	m_pEngineStudio.SetChromeOrigin();
}

int CCustomEngineStudio::IsHardware(void)
{
	return m_pEngineStudio.IsHardware();
}

void CCustomEngineStudio::GL_StudioDrawShadow(void)
{
	m_pEngineStudio.GL_StudioDrawShadow();
}

void CCustomEngineStudio::GL_SetRenderMode(int mode)
{
	m_pEngineStudio.GL_SetRenderMode(mode);
}

void CCustomEngineStudio::StudioSetRenderamt(int iRenderamt)
{
	m_pEngineStudio.StudioSetRenderamt(iRenderamt);
}

void CCustomEngineStudio::StudioSetCullState(int iCull)
{
	m_pEngineStudio.StudioSetCullState(iCull);
}

void CCustomEngineStudio::StudioRenderShadow(int iSprite, float *p1, float *p2, float *p3, float *p4)
{
	m_pEngineStudio.StudioRenderShadow(iSprite, p1, p2, p3, p4);
}