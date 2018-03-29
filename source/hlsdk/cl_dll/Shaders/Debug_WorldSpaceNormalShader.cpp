#include "hud.h"
#include "cl_util.h"

#include "Shader.h"
#include "Debug_WorldSpaceNormalShader.h"

CDebug_WorldSpaceNormalShader::CDebug_WorldSpaceNormalShader(void)
{
}

void CDebug_WorldSpaceNormalShader::Load()
{
	const char* vertexShaderSource = (const char*)gEngfuncs.COM_LoadFile("shaders/Debug_WorldSpaceNormal.vert", 5, NULL);
	const char* fragmentShaderSource = (const char*)gEngfuncs.COM_LoadFile("shaders/Debug_WorldSpaceNormal.frag", 5, NULL);

	Compile(vertexShaderSource, fragmentShaderSource);

	gEngfuncs.COM_FreeFile((void*)vertexShaderSource);
	gEngfuncs.COM_FreeFile((void*)fragmentShaderSource);
}

const void CDebug_WorldSpaceNormalShader::Apply() const
{
	CShader::Apply();
}