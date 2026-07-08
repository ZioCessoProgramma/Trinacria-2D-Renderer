#include "Trinacria/Materials.h"
#include "Trinacria/Renderer.h"

void TRCN_CORE_NAMESPACE::Material::SetUniforms() const
{
    Renderer::ShaderProgram.SetUniformVec3("u_Material.ambient", ambient);
    Renderer::ShaderProgram.SetUniformVec3("u_Material.diffuse", diffuse);
    Renderer::ShaderProgram.SetUniformVec3("u_Material.specular", specular);
    Renderer::ShaderProgram.SetUniformFloat("u_Material.shininess", shininess);
}