#include "Trinacria/HUD.h"
#include "Trinacria/Log.h"

#include <cstring>
#include <glad/glad.h>

#include "Trinacria/Assert.h"
#include "Trinacria/Renderer.h"

void TRCN_CORE_NAMESPACE::HUD::Init(const std::string& vertPath, const std::string& fragPath)
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HUDVertex) * MaxHUDVertices, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxHUDIndices, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex),
        (void*)offsetof(HUDVertex, Position));

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(HUDVertex),
       (void*)offsetof(HUDVertex, Color));

    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(HUDVertex),
        (void*)offsetof(HUDVertex, TextureIndex));

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(HUDVertex), (void*)offsetof(HUDVertex, TexCoord));
    glEnableVertexAttribArray(3);

    _shader.LoadCoreShader(vertPath, fragPath);
}

void Trinacria::DSL::HUD::Cleanup()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void Trinacria::DSL::HUD::EndHUD()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(HUDVertex) * _vertices.size(), _vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * _indices.size(), _indices.data());
}

void Trinacria::DSL::HUD::CreateHUDQuad(const HUDQuadData& HUDQuad)
{
    TRCN_DEPEND_START("Create HUD Quad");

    TRCN_DEPEND_RETURN_ASSERT_VOID(_vertices.size() < MaxHUDVertices);
    TRCN_DEPEND_RETURN_ASSERT_VOID(_indices.size() < MaxHUDIndices);

    uint32_t index = 0;

    if (_textures.empty())
    {
        index = 1;
	_textures.emplace_back(HUDQuad.texture, index);
    }
    else if (!findTextureIndex(index, HUDQuad.texture) && HUDQuad.texture)
    {
        index = _textures[_textures.size() - 1].second + 1;
	_textures.emplace_back(HUDQuad.texture, index);
    }


    // TODO: use .GetMatrix()

    createHUDQuad(HUDQuad.transform.Position, HUDQuad.Color, index, HUDQuad.transform.Scale,
        glm::mat4(1.f), HUDQuad.TexCoords);
}

void Trinacria::DSL::HUD::FlushBuffers()
{
    _vertices.clear();
    _indices.clear();
}

void Trinacria::DSL::HUD::draw()
{
    _shader.Bind();

    for (auto& tex : _textures)
    {
	std::string format = std::format("u_Textures[{}]", tex.second);

	_shader.SetUniformInt(format.c_str(), tex.second - 1);

	tex.first->Bind(tex.second + GL_TEXTURE0 - 1);
    }

    glBindVertexArray(_vao);

    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);

    Texture::ClearTextureSlots();
}

void Trinacria::DSL::HUD::createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex,
                                        const glm::vec2& scale, const glm::mat4& matrix, const QuadTexCoords& coord)
{
    glm::vec2 p0 = glm::vec2(matrix * glm::vec4(position, 0.f, 0.f));
    glm::vec2 p1 = glm::vec2(matrix * glm::vec4(position + glm::vec2(scale.x, 0.f), 0.f, 0.f));
    glm::vec2 p2 = glm::vec2(matrix * glm::vec4(position + scale, 0.f, 0.f));
    glm::vec2 p3 = glm::vec2(matrix * glm::vec4(position + glm::vec2(0.f, scale.y), 0.f, 0.f));

    _vertices.emplace_back(p0, color, textureIndex, coord.Coord0);
    _vertices.emplace_back(p1, color, textureIndex, coord.Coord1);
    _vertices.emplace_back(p2, color, textureIndex, coord.Coord2);
    _vertices.emplace_back(p3, color, textureIndex, coord.Coord3);

    size_t offset = _vertices.size() - 4;

    _indices.push_back(offset);
    _indices.push_back(offset + 1);
    _indices.push_back(offset + 2);
    _indices.push_back(offset + 2);
    _indices.push_back(offset + 3);
    _indices.push_back(offset);
}

bool TRCN_CORE_NAMESPACE::HUD::findTextureIndex(uint32_t& out, const Texture* textureToFind)
{
    for (auto& tex : _textures)
    {
        if (tex.first == textureToFind)
        {
            out = tex.second;
            return true;
        }
    }

    out = 0;

    return false;
}
