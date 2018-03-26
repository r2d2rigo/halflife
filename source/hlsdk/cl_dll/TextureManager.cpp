#include "STB/stb_image.h"
#include "hud.h"
#include "TextureManager.h"

CTextureManager::CTextureManager(void)
{
}

void CTextureManager::Reset(void)
{
	for (std::map<std::string, GLuint>::iterator textureHandle = _textureHandles.begin(); textureHandle != _textureHandles.end(); ++textureHandle)
	{
		glDeleteTextures(1, &textureHandle->second);
	}

	_textureHandles.clear();
}

const GLuint& CTextureManager::LoadCubemap(const BspCubemap &cubemap)
{
	GLuint textureHandle;

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

	for (int i = 0; i < 6; i++)
	{
		int faceOffset = i * (cubemap.Size * cubemap.Size * 3);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, cubemap.Size, cubemap.Size, 0, GL_RGB, GL_UNSIGNED_BYTE, &(cubemap.Data[faceOffset]));
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::string cubemapName;
	cubemapName.append(std::to_string(cubemap.Position[0]));
	cubemapName.append("_");
	cubemapName.append(std::to_string(cubemap.Position[1]));
	cubemapName.append("_");
	cubemapName.append(std::to_string(cubemap.Position[2]));
	_textureHandles.emplace(cubemapName, textureHandle);

	return textureHandle;
}