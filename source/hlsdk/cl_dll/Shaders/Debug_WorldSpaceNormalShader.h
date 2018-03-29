#if !defined( DEBUGWORLDSPACENORMALSHADER_H )
#define DEBUGWORLDSPACENORMALSHADER_H
#if defined( _WIN32 )
#pragma once
#endif

#include "Shader.h"

class CDebug_WorldSpaceNormalShader : public CShader
{
public:
	CDebug_WorldSpaceNormalShader(void);

public:
	void Load();

	const void Apply() const;
};

#endif