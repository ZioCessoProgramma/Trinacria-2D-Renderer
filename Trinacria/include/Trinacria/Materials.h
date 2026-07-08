#pragma once

#include "Macros.h"
#include <glm/glm.hpp>

namespace TRCN_CORE_NAMESPACE
{
    class Material
    {
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float shininess;

        void SetUniforms() const;
    };

    inline constexpr Material SOLID_OPAQUE {glm::vec3(1.f), glm::vec3(1.f),
        glm::vec3(1.f), 32.f};

}
