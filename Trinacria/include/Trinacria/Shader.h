#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Trinacria/Definitions.h"

namespace TRCN_CORE_NAMESPACE
{
    class Shader
    {
    public:
        Shader() = default;
        ~Shader();

        void LoadShader(const std::string& vert, const std::string& frag);

        void Bind();

        uint32_t GetShaderProgram() { return _shaderProgram; }

        void SetUniformMat4(const char* name, const glm::mat4& mat);
        void SetUniformInt(const char* name, int value);
        void SetUniformFloat(const char* name, float value);
        void SetUniformVec2(const char* name, const glm::vec2& value);
        void SetUniformVec3(const char* name, const glm::vec3& value);
        void SetUniformVec4(const char* name, const glm::vec4& value);

    private:
        uint32_t _shaderProgram;
    };
}