
#include "Trinacria/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Trinacria/Log.h"

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

std::string Trinacria::DSL::Shader::getStringFromFile(const std::string& path)
{
    std::stringstream streamFile;

    std::ifstream file(path);

    // TODO: handle with ifdefs
    if (!file.good())
    {
        TRCN_LOG("Problem with shader's file!\n");
        return "";
    }

    streamFile << file.rdbuf();

    return streamFile.str();
}

uint32_t Trinacria::DSL::Shader::compileAndCreateShader(const char* source, uint32_t shaderType)
{
    uint32_t id = glCreateShader(shaderType);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];

        glGetShaderInfoLog(id, 512, nullptr, infoLog);

        std::cout << "Vertex shader compilation failed:\n" << infoLog;
    }

    return id;
}

uint32_t Trinacria::DSL::Shader::linkAndCreateProgram(uint32_t fragID, uint32_t vertID)
{
    uint32_t out = glCreateProgram();

    glAttachShader(out, vertID);
    glAttachShader(out, fragID);

    glLinkProgram(out);

    int success;

    glGetProgramiv(out, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(out, 512, nullptr, infoLog);
        std::cout << "Program linking failed:\n" << infoLog;
    }

    return out;
}

void TRCN_CORE_NAMESPACE::Shader::LoadShader(const std::string& vert, const std::string& frag)
{
    std::string vertexSource = getStringFromFile(vert);
    std::string fragmentSource = getStringFromFile(frag);

    const char* vertexSource_ = vertexSource.c_str();
    const char* fragmentSource_ = fragmentSource.c_str();

    uint32_t vertexID = compileAndCreateShader(vertexSource_, GL_VERTEX_SHADER);
    uint32_t fragmentID = compileAndCreateShader(fragmentSource_, GL_FRAGMENT_SHADER);

    uint32_t out = linkAndCreateProgram(fragmentID, vertexID);

    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);

    _shaderProgram = out;
}

void Trinacria::DSL::Shader::LoadCoreShader(const std::string& vert, const std::string& frag, uint32_t maxTextures)
{
    std::string vertexSource = getStringFromFile(vert);
    std::string fragmentSource = getStringFromFile(frag);

    size_t versionPos = fragmentSource.find("#version");
    if (versionPos == std::string::npos)
    {
        TRCN_LOG("TRINACRIA_SHADER_ERROR: no #version directive found");
        return;
    }

    size_t versionLineEnd = fragmentSource.find('\n', versionPos);

    if (versionLineEnd == std::string::npos)
    {
        TRCN_LOG("TRINACRIA_SHADER_ERROR: unexpected end of file after #version");
        return;
    }

    versionLineEnd++; // go forward of one character

    fragmentSource.insert(versionLineEnd, std::format("\n#define MAX_TEXTURE_SLOTS {}\n", maxTextures));

    size_t texturesUniformPos = fragmentSource.find("uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];", versionLineEnd);
    size_t texturesUniformPosLineEnd = fragmentSource.find("\n", texturesUniformPos);
    texturesUniformPosLineEnd++;

    std::string function = "\nvec4 SampleTexture(int index, vec2 uv) {\n"
                           "switch (index) {\n";

    for (int i = 0; i < maxTextures; i++)
    {
        function.append(std::format("case {}: return texture(u_Textures[index], uv); break;", i));
    }
    function.append("}}\n");

    fragmentSource.insert(texturesUniformPosLineEnd, function);

    const char* vertexSource_ = vertexSource.c_str();
    const char* fragmentSource_ = fragmentSource.c_str();

    uint32_t vertexID = compileAndCreateShader(vertexSource_, GL_VERTEX_SHADER);
    uint32_t fragmentID = compileAndCreateShader(fragmentSource_, GL_FRAGMENT_SHADER);

    uint32_t out = linkAndCreateProgram(fragmentID, vertexID);

    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);

    _shaderProgram = out;
}