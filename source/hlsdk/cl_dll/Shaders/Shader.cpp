#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
#endif

#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

#include "Shader.h"

CShader::CShader(void)
{
}

void CShader::Compile(const char *vertexShaderSource, const char* fragmentShaderSource)
{
	_uniformLocations.clear();

	_vertexShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	_fragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	glShaderSourceARB(_vertexShaderObject, 1, &vertexShaderSource, NULL);
	glCompileShaderARB(_vertexShaderObject);

	glShaderSourceARB(_fragmentShaderObject, 1, &fragmentShaderSource, NULL);

	_shaderProgramObject = glCreateProgram();

	glAttachShader(_shaderProgramObject, _vertexShaderObject);
	glAttachShader(_shaderProgramObject, _fragmentShaderObject);

	glLinkProgram(_shaderProgramObject);

	// TODO: do error logging
	GLint maxLength = 0;
	// glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

	char errorLog[2048];
	glGetProgramInfoLog(_shaderProgramObject, 2048, &maxLength, &errorLog[0]);
	int a = 0;
}

void CShader::CacheUniform(std::string uniformName)
{
	if (_uniformLocations.find(uniformName) != _uniformLocations.end())
	{
		return;
	}

	GLint uniformLocation = glGetUniformLocation(_shaderProgramObject, uniformName.c_str());
	_uniformLocations.emplace(uniformName, uniformLocation);
}

void CShader::SetParameter(std::string parameterName, GLint value)
{
	std::map<std::string, GLint>::iterator cachedLocation = _uniformLocations.find(parameterName);

	if (cachedLocation == _uniformLocations.end())
	{
		// TODO: error handling/logging
		return;
	}

	glUniform1i(cachedLocation->second, value);
}

void CShader::SetParameter(std::string parameterName, GLuint value)
{
	std::map<std::string, GLint>::iterator cachedLocation = _uniformLocations.find(parameterName);

	if (cachedLocation == _uniformLocations.end())
	{
		// TODO: error handling/logging
		return;
	}

	glUniform1ui(cachedLocation->second, value);
}

void CShader::SetParameter(std::string parameterName, GLfloat value)
{
	std::map<std::string, GLint>::iterator cachedLocation = _uniformLocations.find(parameterName);

	if (cachedLocation == _uniformLocations.end())
	{
		// TODO: error handling/logging
		return;
	}

	glUniform1f(cachedLocation->second, value);
}

void CShader::SetParameter(std::string parameterName, int stride, int count, GLfloat *values)
{
	std::map<std::string, GLint>::iterator cachedLocation = _uniformLocations.find(parameterName);

	if (cachedLocation == _uniformLocations.end())
	{
		// TODO: error handling/logging
		return;
	}

	switch (stride)
	{
		case 1: { glUniform1fv(cachedLocation->second, count, values); } break;
		case 2: { glUniform2fv(cachedLocation->second, count, values); } break;
		case 3: { glUniform3fv(cachedLocation->second, count, values); } break;
		case 4: { glUniform4fv(cachedLocation->second, count, values); } break;
	}
}


//void CShader::SetParameter(std::string parameterName, float value)
//{
//}

void CShader::Apply()
{
	glUseProgram(_shaderProgramObject);
}



const GLuint CShader::ShaderProgram()
{
	return _shaderProgramObject;
}