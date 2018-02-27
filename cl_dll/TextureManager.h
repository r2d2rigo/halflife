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

class CTextureManager
{
private:
	static std::string const FACE_NAMES[];

	static std::string const CUBEMAPS_FOLDER;
	static std::string const IMAGE_EXTENSION;

private:
	std::map<std::string, GLuint> _textureHandles;

public:
	CTextureManager(void);

	const GLuint& LoadCubemap(const std::string &cubemapName);
};

#endif