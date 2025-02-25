#include "Shader.h"

void Shader::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile(ENGINE_PATH + vertexPath), fShaderFile(ENGINE_PATH + fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkForERR(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkForERR(fragment, "FRAGMENT");

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);
    checkForERR(shaderID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(shaderID);
}

void Shader::setInt(const std::string& name, int value) const
{
    use();
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) const
{
    use();
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    use();
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    use();
    glUniform2f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    use();
    glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
    use();
    glUniform4f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    use();
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::createUBO()
{
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}

void Shader::setUBOBlockBinding(const std::string& name, int index)
{
    use();
    glUniformBlockBinding(shaderID, glGetUniformBlockIndex(shaderID, name.c_str()), index);
}

void Shader::setMat4UniformBuffer(int index, glm::mat4& value)
{
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(value));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::checkForERR(unsigned int shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            SEND_MSG(ERROR, "SHADER", ("Shader Compilation(" + type + ")\n" + infoLog).c_str());
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            SEND_MSG(ERROR, "SHADER", "Shader Linking: ");
            SEND_MSG(ERROR, "SHADER", infoLog);
        }
    }
}
