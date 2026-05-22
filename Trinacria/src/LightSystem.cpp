#include "Trinacria/LightSystem.h"
#include "Trinacria/Renderer.h"


TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_pointTexture;
TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_spotTexture;
TRCN_CORE_NAMESPACE::Texture TRCN_CORE_NAMESPACE::LightSystem::_directionalTexture;
std::vector<float> TRCN_CORE_NAMESPACE::LightSystem::_pointLights;
std::vector<float> TRCN_CORE_NAMESPACE::LightSystem::_spotLights;
std::vector<float> TRCN_CORE_NAMESPACE::LightSystem::_dirLights;

void Trinacria::DSL::LightSystem::setupUniform(const std::string& uniformName, Texture& texture, std::vector<float>& data, size_t textureWidth)
{
    {
        uint32_t id = texture.GetId();
        glDeleteTextures(1, &id);
    }

    texture.Init();

    texture.BoundTexImage(GL_RGBA32F, GL_RGBA, textureWidth, 1,
                                GL_FLOAT, data.data(), GL_NEAREST);

    Renderer::ShaderProgram.SetUniformInt(uniformName.c_str(),
        texture.GetTextureChosenSlot() - GL_TEXTURE0);

    data.clear();
}

void Trinacria::DSL::LightSystem::Init(float strength)
{
    Renderer::ShaderProgram.SetUniformFloat("u_AmbientStrength", strength);

    _pointLights.reserve(MAX_POINT_LIGHTS * 8);      // 8 float = 2 texel
    _spotLights.reserve(MAX_SPOT_LIGHTS * 12);       // 12 float = 3 texel
    _dirLights.reserve(MAX_DIRECTIONAL_LIGHTS * 8);  // 8 float = 2 texel
}

void Trinacria::DSL::LightSystem::SetViewPos(glm::vec2 viewPos)
{
    Renderer::ShaderProgram.SetUniformVec2("u_ViewPos", viewPos);
}

void Trinacria::DSL::LightSystem::SetupLight(const PointLightData& lightData)
{
    _pointLights.push_back(lightData.LightColor.r);
    _pointLights.push_back(lightData.LightColor.g);
    _pointLights.push_back(lightData.LightColor.b);
    _pointLights.push_back(lightData.LightPosition.x);
    _pointLights.push_back(lightData.LightPosition.y);
    _pointLights.push_back(lightData.Attenuation);
    _pointLights.push_back(0.f);
    _pointLights.push_back(0.f);
}

void Trinacria::DSL::LightSystem::SetupLight(const SpotLightData& lightData)
{
    _spotLights.push_back(lightData.LightColor.r);
    _spotLights.push_back(lightData.LightColor.g);
    _spotLights.push_back(lightData.LightColor.b);
    _spotLights.push_back(lightData.LightPosition.x);
    _spotLights.push_back(lightData.LightPosition.y);
    _spotLights.push_back(lightData.LightDirection.x);
    _spotLights.push_back(lightData.LightDirection.y);
    _spotLights.push_back(lightData.Attenuation);
    _spotLights.push_back(glm::cos(glm::radians(lightData.InnerAngleInDegrees)));
    _spotLights.push_back(glm::cos(glm::radians(lightData.OuterAngleInDegrees)));
    _spotLights.push_back(0.f);
    _spotLights.push_back(0.f);
}

void Trinacria::DSL::LightSystem::SetupLight(const DirectionalLightData& lightData)
{
    _dirLights.push_back(lightData.LightColor.r);
    _dirLights.push_back(lightData.LightColor.g);
    _dirLights.push_back(lightData.LightColor.b);
    _dirLights.push_back(lightData.LightDirection.x);
    _dirLights.push_back(lightData.LightDirection.y);
    _dirLights.push_back(0.f);
    _dirLights.push_back(0.f);
    _dirLights.push_back(0.f);
}

void Trinacria::DSL::LightSystem::Done()
{
    setupUniform("u_PointLights", _pointTexture, _pointLights, MAX_POINT_LIGHTS * 2);
    setupUniform("u_SpotLights", _spotTexture, _spotLights, MAX_SPOT_LIGHTS * 3);
    setupUniform("u_DirLights", _directionalTexture, _dirLights, MAX_DIRECTIONAL_LIGHTS * 2);
}
