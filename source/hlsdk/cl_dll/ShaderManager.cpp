#include "ShaderManager.h"

CShaderManager::CShaderManager(void)
{
}

void CShaderManager::Reset(void)
{
	// TODO: proper reinitialize/cleanup
	_vertexLitGeneric.Load();
}

CVertexLitGenericShader* CShaderManager::VertexLitGeneric()
{
	return &_vertexLitGeneric;
}
