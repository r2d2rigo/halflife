#if !defined( VERTEXLITGENERICSHADER_H )
#define VERTEXLITGENERICSHADER_H
#if defined( _WIN32 )
#pragma once
#endif

#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "SDL2/SDL_opengl.h"
#include "util_vector.h"
#include "Shader.h"

class CVertexLitGenericShader : public CShader
{
private:
	GLuint _baseTextureSampler;
	GLuint _envMapSampler;

	Vector _lightDir;
	float _lightColor[4];

	float _cubemapAmount;
	Vector _cameraPos;
	float _ambientCube[18];

	float _envMapContrast;

public:
	CVertexLitGenericShader(void);

public:
	void Load();

	void SetBaseTexture(GLuint baseTextureSampler);
	void SetEnvMap(GLuint envMapSampler);

	void SetLightDir(const Vector &lightDir);
	void SetLightColor(const Vector &lightColor, const float &lightIntensity);
	void SetCubemapAmount(const float &cubemapAmount);
	void SetCameraPos(const Vector &cameraPos);
	void SetAmbientCube(float ambientCube[18]);

	void SetEnvMapContrast(const float &envMapContrast);

	// GLint shadeLightLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "ShadeLight");
	// GLint lightDirLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "LightDir");
	// GLint lightColorLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "LightColor");
	// GLint cubemapAmountLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "CubemapAmount");
	// GLint cameraPosLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "CameraPos");
	// GLint ambientCubeLocation = glGetUniformLocation(ShaderManager.VertexLitGeneric()->ShaderProgram(), "AmbientCube");

	void Apply();
	// void SetEnvMapSaturation();

};

#endif