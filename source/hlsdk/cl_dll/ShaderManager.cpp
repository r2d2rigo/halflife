#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "ShaderManager.h"

CShaderManager::CShaderManager(void)
{
}

void CShaderManager::Reset(void)
{
	// TODO: proper reinitialize/cleanup
	_debugWorldSpaceNormal.Load();

	_vertexLitGeneric.Load();
}

void CShaderManager::RestoreState(void)
{
	glUseProgram(0);
}

CVertexLitGenericShader& CShaderManager::VertexLitGeneric() 
{
	return _vertexLitGeneric;
}

CDebug_WorldSpaceNormalShader& CShaderManager::Debug_WorldSpaceNormal()
{
	return _debugWorldSpaceNormal;
}
