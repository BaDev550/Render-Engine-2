#ifndef SHADER_CLASS
#define SHADER_CLASS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VECore.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
private:
	unsigned int uboMatrices;
public:
	unsigned int shaderID;

	Shader() {}

	void loadShader(const std::string& vertexPath, const std::string& fragmentPath);
	void use() const;

	void setInt(const std::string& name, int value) const;
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec4(const std::string& name, glm::vec4 value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

	void createUBO();
	void setUBOBlockBinding(const std::string& name, int index);

	void setMat4UniformBuffer(int index, glm::mat4& value);
private:
	void checkForERR(unsigned int shader, const std::string& type);
};

#endif // !SHADER_CLASS
