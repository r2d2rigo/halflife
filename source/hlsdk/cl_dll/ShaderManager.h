#if !defined( SHADERMANAGER_H )
#define SHADERMANAGER_H
#if defined( _WIN32 )
#pragma once
#endif

#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include <vector>
#include <map>
#include "SDL2/SDL_opengl.h"

#include "Shaders\Shader.h"
#include "Shaders\VertexLitGenericShader.h"

class CShaderManager
{
private:
	CVertexLitGenericShader _vertexLitGeneric;

public:
	CShaderManager(void);
	void Reset(void);

	CVertexLitGenericShader* VertexLitGeneric();
};

#endif