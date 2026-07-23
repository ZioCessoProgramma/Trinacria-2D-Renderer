
#include "Trinacria/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>
#include <iostream>

void TRCN_CORE_NAMESPACE::Shader::Bind()
{
	glUseProgram(_shaderProgram);
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformMat4(const char* name, const glm::mat4& mat)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformInt(const char* name, int value)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniform1i(location, value);
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformFloat(const char* name, float value)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniform1f(location, value);
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformVec2(const char* name, const glm::vec2& value)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniform2f(location, value.x, value.y);
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformVec3(const char* name, const glm::vec3& value)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniform3f(location, value.x, value.y, value.z);
}

void TRCN_CORE_NAMESPACE::Shader::SetUniformVec4(const char* name, const glm::vec4& value)
{
	uint32_t location = glGetUniformLocation(_shaderProgram, name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void TRCN_CORE_NAMESPACE::Shader::LoadShader(const std::string& vert, const std::string& frag)
{
    std::stringstream vertexShaderStream;
    std::stringstream fragmentShaderStream;

    std::ifstream vertexFile(vert);
    std::ifstream fragmentFile(frag);

    if (!vertexFile.good())
    {
        std::cout << "Vertex file problems\n";
        return;
    }

    if (!fragmentFile.good())
    {
        std::cout << "Fragment file problems\n";
        return;
    }

    vertexShaderStream << vertexFile.rdbuf();
    fragmentShaderStream << fragmentFile.rdbuf();

    std::string vertexSource = vertexShaderStream.str();
    std::string fragmentSource = fragmentShaderStream.str();

    const char* vertexSource_ = vertexSource.c_str();
    const char* fragmentSource_ = fragmentSource.c_str();

    uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexID, 1, &vertexSource_, nullptr);
    glCompileShader(vertexID);


    int success;
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);

        std::cout << "Vertex shader compilation failed:\n" << infoLog;
    }

    glShaderSource(fragmentID, 1, &fragmentSource_, nullptr);
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetShaderInfoLog(fragmentID, 512, nullptr, infoLog);

        std::cout << "Fragment shader compilation failed:\n" << infoLog;
    }

    uint32_t out = glCreateProgram();

    glAttachShader(out, vertexID);
    glAttachShader(out, fragmentID);

    glLinkProgram(out);

    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);

    _shaderProgram = out;
}
