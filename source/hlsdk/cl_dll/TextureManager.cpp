#include "STB/stb_image.h"
#include "hud.h"
#include "TextureManager.h"

std::string const CTextureManager::FACE_NAMES[] =
{
	"lf",
	"rt",
	"ft",
	"bk",
	"dn",
	"up"
};

std::string const CTextureManager::CUBEMAPS_FOLDER = std::string("cubemaps/");
std::string const CTextureManager::IMAGE_EXTENSION = std::string(".png");

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

const GLuint& CTextureManager::LoadCubemap(const std::string &cubemapName)
{
	GLuint textureHandle;
	byte *textureFileContents;
	int textureLength;
	int textureWidth;
	int textureHeight;
	int channelCount;
	stbi_uc *textureData;

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
	
	for (int i = 0; i < 6; i++)
	{
		textureFileContents = gEngfuncs.COM_LoadFile((char*)(CTextureManager::CUBEMAPS_FOLDER + cubemapName + CTextureManager::FACE_NAMES[i] + CTextureManager::IMAGE_EXTENSION).c_str(), 5, &textureLength);
		textureData = stbi_load_from_memory((const stbi_uc*)textureFileContents, textureLength, &textureWidth, &textureHeight, &channelCount, 3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		stbi_image_free(textureData);
		gEngfuncs.COM_FreeFile(textureFileContents);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureHandle;
}

const GLuint& CTextureManager::LoadCubemap(const BspCubemap &cubemap)
{
	GLuint textureHandle;
	byte *textureFileContents;
	int textureLength;
	int textureWidth;
	int textureHeight;
	int channelCount;
	stbi_uc *textureData;

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

	for (int i = 0; i < 6; i++)
	{
		int faceOffset = i * (32 * 32 * 3);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, &(cubemap.Data[faceOffset]));
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureHandle;
}