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

    struct SpotLightData
    {
        glm::vec2 LightPosition;
        glm::vec2 ViewPosition;
        glm::vec3 LightColor;
        glm::vec2 LightDirection;
        float Attenuation;

        float InnerAngleInDegrees;
        float OuterAngleInDegrees;
    };

    struct DirectionalLightData
    {
        glm::vec2 LightDirection;
        glm::vec2 ViewPosition;
        glm::vec3 LightColor;
    };

    class LightSystem
    {
    public:
        static void SetAmbientStrength(float strength);

        static void SetupLight(const PointLightData& lightData);
        static void SetupLight(const SpotLightData& lightData);
        static void SetupLight(const DirectionalLightData& lightData);

        // ambient strength provided values
        static constexpr float WORLD_NOT_AFFECTED_BY_LIGHT = 1.f;
        static constexpr float DEFAULT_USE_OF_LIGHT = 0.1f;
        static constexpr float BRIGHT = 0.2f;
        static constexpr float SUPER_BRIGHT = 0.4f;
        static constexpr float DARK = 0.06f;
        static constexpr float SUPER_DARK = 0.01f;
    };
}
