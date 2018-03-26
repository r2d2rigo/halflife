#if !defined( TEXTUREMANAGER_H )
#define TEXTUREMANAGER_H
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
#include "BspFile.h"

class CTextureManager
{
private:
	std::map<std::string, GLuint> _textureHandles;

public:
	CTextureManager(void);
	void Reset(void);

	const GLuint& LoadCubemap(const BspCubemap &cubemap);
};

#endif