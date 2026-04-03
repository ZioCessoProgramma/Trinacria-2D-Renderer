#include "Trinacria/LightSystem.h"
#include "Trinacria/Renderer.h"

void Trinacria::DSL::LightSystem::SetAmbientStrength(float strength)
{
    Renderer::ShaderProgram.SetUniformFloat("u_AmbientStrength", strength);
}

void Trinacria::DSL::LightSystem::SetupLight(const PointLightData& lightData)
{
    Renderer::ShaderProgram.SetUniformFloat("u_PointLight.u_Attenuation", lightData.Attenuation);
    Renderer::ShaderProgram.SetUniformVec2("u_PointLight.u_LightPos", lightData.LightPosition);
    Renderer::ShaderProgram.SetUniformVec2("u_PointLight.u_ViewPos", lightData.ViewPosition);
    Renderer::ShaderProgram.SetUniformVec3("u_PointLight.u_LightColor", lightData.LightColor);
}

void Trinacria::DSL::LightSystem::SetupLight(const SpotLightData& lightData)
{
    Renderer::ShaderProgram.SetUniformFloat("u_SpotLight.u_Attenuation", lightData.Attenuation);
    Renderer::ShaderProgram.SetUniformVec2("u_SpotLight.u_LightPos", lightData.LightPosition);
    Renderer::ShaderProgram.SetUniformVec2("u_SpotLight.u_ViewPos", lightData.ViewPosition);
    Renderer::ShaderProgram.SetUniformVec3("u_SpotLight.u_LightColor", lightData.LightColor);
    Renderer::ShaderProgram.SetUniformVec2("u_SpotLight.u_LightDirection", lightData.LightDirection);

    Renderer::ShaderProgram.SetUniformFloat("u_SpotLight.u_InnerCutOff",
        glm::cos(glm::radians(lightData.InnerAngleInDegrees)));

    Renderer::ShaderProgram.SetUniformFloat("u_SpotLight.u_OuterCutOff",
        glm::cos(glm::radians(lightData.OuterAngleInDegrees)));

}

void Trinacria::DSL::LightSystem::SetupLight(const DirectionalLightData& lightData)
{
    Renderer::ShaderProgram.SetUniformVec2("u_DirectionalLight.u_LightDirection", lightData.LightDirection);
    Renderer::ShaderProgram.SetUniformVec3("u_DirectionalLight.u_LightColor", lightData.LightColor);
    Renderer::ShaderProgram.SetUniformVec2("u_DirectionalLight.u_ViewPos", lightData.ViewPosition);
}
