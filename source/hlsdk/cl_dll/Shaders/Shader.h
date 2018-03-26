#if !defined( SHADER_H )
#define SHADER_H
#if defined( _WIN32 )
#pragma once
#endif

#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include <map>
#include <string>
#include "SDL2/SDL_opengl.h"

class CShader
{
private:
	GLuint _vertexShaderObject;
	GLuint _fragmentShaderObject;
	GLuint _shaderProgramObject;

protected:
	std::map<std::string, GLint> _uniformLocations;

protected:
	CShader(void);

protected:
	void Compile(const char *vertexShaderSource, const char* fragmentShaderSource);
	void CacheUniform(std::string uniformName);

	void SetParameter(std::string parameterName, GLint value);
	void SetParameter(std::string parameterName, GLuint value);
	void SetParameter(std::string parameterName, GLfloat value);
	void SetParameter(std::string parameterName, int stride, int count, GLfloat* values);

public:
	virtual void Apply();


	//TODO: remove this
	const GLuint ShaderProgram();
};

#endif