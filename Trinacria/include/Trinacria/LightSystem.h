#pragma once

#include "Macros.h"
#include <glm/glm.hpp>

#include "Texture.h"

namespace TRCN_CORE_NAMESPACE
{
    struct PointLightData
    {
        glm::vec3 LightColor;
        glm::vec2 LightPosition;

        float Attenuation;
    }; // 6 float ---> 2 texel

    struct SpotLightData
    {
        glm::vec3 LightColor;
        glm::vec2 LightPosition;
        glm::vec2 LightDirection;
        float Attenuation;

        float InnerAngleInDegrees;
        float OuterAngleInDegrees;
    }; // 10 float --> 3 texel

    struct DirectionalLightData
    {
        glm::vec3 LightColor;
        glm::vec2 LightDirection;
    }; // 5 float ---> 2 texel

    class LightSystem
    {
    public:
        static void Init(float strength);

        static void SetViewPos(glm::vec2 viewPos);

        static void SetupLight(const PointLightData& lightData);
        static void SetupLight(const SpotLightData& lightData);
        static void SetupLight(const DirectionalLightData& lightData);

        static void Done();

        // ambient strength provided values
        static constexpr float WORLD_NOT_AFFECTED_BY_LIGHT = 1.f;
        static constexpr float DEFAULT_USE_OF_LIGHT = 0.1f;
        static constexpr float BRIGHT = 0.2f;
        static constexpr float SUPER_BRIGHT = 0.4f;
        static constexpr float DARK = 0.06f;
        static constexpr float SUPER_DARK = 0.01f;

        static constexpr uint32_t MAX_POINT_LIGHTS = 64;
        static constexpr uint32_t MAX_SPOT_LIGHTS = 32;
        static constexpr uint32_t MAX_DIRECTIONAL_LIGHTS = 8;

    private:
        static Texture _pointTexture;
        static Texture _spotTexture;
        static Texture _directionalTexture;

        static std::array<float, MAX_POINT_LIGHTS * 8> _pointLights;
        static std::array<float, MAX_SPOT_LIGHTS * 12> _spotLights;
        static std::array<float, MAX_DIRECTIONAL_LIGHTS * 8> _dirLights;

        static int _pointLightIndex;
        static int _spotLightIndex;
        static int _dirLightIndex;
    };
}
