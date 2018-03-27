#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "hud.h"
#include "cl_util.h"

#include "Shader.h"
#include "VertexLitGenericShader.h"

CVertexLitGenericShader::CVertexLitGenericShader(void)
{
}

void CVertexLitGenericShader::Load()
{
	const char* vertexShaderSource = (const char*)gEngfuncs.COM_LoadFile("shaders/VertexLitGeneric.vert", 5, NULL);
	const char* fragmentShaderSource = (const char*)gEngfuncs.COM_LoadFile("shaders/VertexLitGeneric.frag", 5, NULL);

	Compile(vertexShaderSource, fragmentShaderSource);

	gEngfuncs.COM_FreeFile((void*)vertexShaderSource);
	gEngfuncs.COM_FreeFile((void*)fragmentShaderSource);

	CacheUniform("BaseTexture");
	CacheUniform("EnvMap");

	CacheUniform("LightDir");
	CacheUniform("LightColor");
	CacheUniform("CubemapAmount");
	CacheUniform("CameraPos");
	CacheUniform("AmbientCube");
	CacheUniform("EnvMapContrast");
}

void CVertexLitGenericShader::SetBaseTexture(GLuint baseTextureSampler)
{
	_baseTextureSampler = baseTextureSampler;
}

void CVertexLitGenericShader::SetEnvMap(GLuint envMapSampler)
{
	_envMapSampler = envMapSampler;
}

void CVertexLitGenericShader::SetLightDir(const Vector &lightDir)
{
	_lightDir = lightDir;
}

void CVertexLitGenericShader::SetLightColor(const Vector &lightColor, const float &lightIntensity)
{
	_lightColor[0] = lightColor.x;
	_lightColor[1] = lightColor.y;
	_lightColor[2] = lightColor.z;
	_lightColor[3] = lightIntensity;
}

void CVertexLitGenericShader::SetCubemapAmount(const float &cubemapAmount)
{
	_cubemapAmount = cubemapAmount;
}

void CVertexLitGenericShader::SetCameraPos(const Vector &cameraPos)
{
	_cameraPos = cameraPos;
}

void CVertexLitGenericShader::SetAmbientCube(float ambientCube[6])
{
	_ambientCube[0] = ambientCube[0];
	_ambientCube[1] = ambientCube[1];
	_ambientCube[2] = ambientCube[2];
	_ambientCube[3] = ambientCube[3];
	_ambientCube[4] = ambientCube[4];
	_ambientCube[5] = ambientCube[5];
}

void CVertexLitGenericShader::SetEnvMapContrast(const float &envMapContrast)
{
	_envMapContrast = envMapContrast;
}

void CVertexLitGenericShader::Apply()
{
	CShader::Apply();

	SetParameter("BaseTexture", 0);
	SetParameter("EnvMap", 1);

	SetParameter("LightDir", 3, 1, _lightDir);
	SetParameter("LightColor", 4, 1, _lightColor);

	SetParameter("CameraPos", 3, 1, (float*)_cameraPos);
	SetParameter("CubemapAmount", _cubemapAmount);
	SetParameter("AmbientCube", 6, 1, _ambientCube);
	SetParameter("EnvMapContrast", _envMapContrast);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _baseTextureSampler);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _envMapSampler);
}