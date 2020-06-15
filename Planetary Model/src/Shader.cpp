#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexFilePath);
		fragmentFile.open(fragmentFilePath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexFile.rdbuf();
		fragmentShaderStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
			std::cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESFULLY" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexID, fragmentID;

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vShaderCode, nullptr);
	glCompileShader(vertexID);
	checkForCompileErrors(vertexID, "VERTEX");

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentID);
	checkForCompileErrors(fragmentID, "FRAGMENT");

	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);
	checkForCompileErrors(programID, "PROGRAM");

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::use() const
{
	glUseProgram(programID);
}

void Shader::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name), (int)value);
}

void Shader::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name), value);
}

void Shader::setInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name), value);
}

void Shader::setVec2(const char* name, const glm::vec2& vec2) const
{
	glUniform2fv(glGetUniformLocation(programID, name), 1, glm::value_ptr(vec2));
}

void Shader::setVec3(const char* name, const glm::vec3& vec3) const
{
	glUniform2fv(glGetUniformLocation(programID, name), 1, glm::value_ptr(vec3));
}

void Shader::setVec4(const char* name, const glm::vec4& vec4) const
{
	glUniform4fv(glGetUniformLocation(programID, name), 1, glm::value_ptr(vec4));
}

void Shader::setMat2(const char* name, const glm::mat2& mat2) const
{
	glUniformMatrix2fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(mat2));
}

void Shader::setMat3(const char* name, const glm::mat3& mat3) const
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(mat3));
}

void Shader::setMat4(const char* name, const glm::mat4& mat4) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(mat4));
}
	// shader parameter is used for the shader program if a type other than "PROGRAM" is used
void Shader::checkForCompileErrors(unsigned int shader, const char* type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type <<
				"\n -- -- --------------------------------------------------- --";
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type <<
				"\n -- -- --------------------------------------------------- --";
		}
	}
}

