#include "Trinacria/LightSystem.h"
#include "Trinacria/Renderer.h"

void Trinacria::DSL::LightSystem::SetAmbientStrength(float strength)
{
    Renderer::ShaderProgram.SetUniformFloat("u_AmbientStrength", strength);
}

void Trinacria::DSL::LightSystem::SetupLight(const PointLightData& lightData)
{
    Renderer::ShaderProgram.SetUniformFloat("u_Attenuation", lightData.Attenuation);
    Renderer::ShaderProgram.SetUniformVec2("u_LightPos", lightData.LightPosition);
    Renderer::ShaderProgram.SetUniformVec2("u_ViewPos", lightData.ViewPosition);
    Renderer::ShaderProgram.SetUniformVec3("u_LightColor", lightData.LightColor);
}
