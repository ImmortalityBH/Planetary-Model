#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexFilePath, const char* fragmentFilePath, 
		const char* geometryFilePath = " ");
	~Shader();

	void use() const;

	void setBool(const char* name, bool value) const;
	void setFloat(const char* name, float value) const;
	void setInt(const char* name, int value) const;

	void setVec2(const char* name, const glm::vec2& vec2) const;
	void setVec3(const char* name, const glm::vec3& vec3) const;
	void setVec4(const char* name, const glm::vec4& vec4) const;

	void setMat2(const char* name, const glm::mat2& mat2) const;
	void setMat3(const char* name, const glm::mat3& mat3) const;
	void setMat4(const char* name, const glm::mat4& mat4) const;

private:
	unsigned int programID;

	void checkForCompileErrors(unsigned int shaderType, const char* type);
};
