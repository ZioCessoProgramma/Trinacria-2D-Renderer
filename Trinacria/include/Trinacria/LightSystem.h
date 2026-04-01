#pragma once

#include "Macros.h"
#include <glm/glm.hpp>

namespace TRCN_CORE_NAMESPACE
{
    struct PointLightData
    {
        glm::vec2 LightPosition;
        glm::vec2 ViewPosition;
        glm::vec3 LightColor;

        float Attenuation;
    };

    constexpr float WORLD_NOT_AFFECTED_BY_LIGHT = 1.f;
    constexpr float DEFAULT_USE_OF_LIGHT = 0.1f;
    constexpr float BRIGHT = 0.2f;
    constexpr float SUPER_BRIGHT = 0.4f;
    constexpr float DARK = 0.06f;
    constexpr float SUPER_DARK = 0.01f;

    class LightSystem
    {
    public:
        static void SetAmbientStrength(float strength);

        static void SetupLight(const PointLightData& lightData);
    };
}
