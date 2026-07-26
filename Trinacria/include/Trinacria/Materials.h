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

        void SetUniforms(int materialIndex) const;

        bool operator==(const Material& material) const {return this->ambient == material.ambient &&
            this->diffuse == material.diffuse && this->specular == material.specular;}
    };

    inline constexpr Material SOLID_OPAQUE {glm::vec3(1.f), glm::vec3(1.f),
        glm::vec3(1.f), 32.f};

    inline constexpr Material METAL {glm::vec3(0.01f), glm::vec3(0.7f),
        glm::vec3(1.f), 256};

}
