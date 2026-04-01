#include "Trinacria/Renderer.h"
#include <glad/glad.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <format>

#include "Trinacria/Animation.h"
#include "Trinacria/Sprite.h"
#include "Trinacria/Log.h"

#include "Trinacria/ShapesData.h"

std::vector<TRCN_CORE_NAMESPACE::Vertex> TRCN_CORE_NAMESPACE::Renderer::_quadBuffer;
std::vector<uint32_t> TRCN_CORE_NAMESPACE::Renderer::_quadIndexBuffer;

uint32_t TRCN_CORE_NAMESPACE::Renderer::_vao;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_vbo;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_ebo;

TRCN_CORE_NAMESPACE::Shader TRCN_CORE_NAMESPACE::Renderer::ShaderProgram;

const size_t TRCN_CORE_NAMESPACE::Renderer::MaxQuads;
const size_t TRCN_CORE_NAMESPACE::Renderer::MaxQuadVertices;
const size_t TRCN_CORE_NAMESPACE::Renderer::MaxQuadIndices;

std::vector<TRCN_CORE_NAMESPACE::Vertex> TRCN_CORE_NAMESPACE::Renderer::_triangleBuffer;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_triangleVbo;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_triangleVao;

std::vector<std::pair<TRCN_CORE_NAMESPACE::Texture*, uint32_t>> TRCN_CORE_NAMESPACE::Renderer::_textures;


void TRCN_CORE_NAMESPACE::Renderer::Init()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, MaxQuadVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxQuadIndices * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Position));
 
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TextureIndex));

    attribPointer(2, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TexCoords));
    attribPointer(3, 3, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Color));

    // triangle setup

    glGenVertexArrays(1, &_triangleVao);
    glBindVertexArray(_triangleVao);

    glGenBuffers(1, &_triangleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);

    glBufferData(GL_ARRAY_BUFFER, MaxTrianglesVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TextureIndex));

    attribPointer(2, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TexCoords));
    attribPointer(3, 3, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Color));

    _quadBuffer.reserve(MaxQuadVertices);
    _quadIndexBuffer.reserve(MaxQuadIndices);

    _triangleBuffer.reserve(MaxTrianglesVertices);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateQuad(const glm::vec2& position, Texture* texture, const glm::vec2& size, const glm::vec3& color, const glm::mat4& transform, const QuadTexCoords& texCoords)
{
    if (_quadBuffer.size() > MaxQuadVertices)
    {
        TRCN_LOG("Vertex buffer overflowed please start a new batch!");
        return;
    }

    if (_quadIndexBuffer.size() > MaxQuadIndices)
    {
        TRCN_LOG("Index buffer overflowed please start a new batch!");
        return;
    }

    uint32_t textureIndex;

    if (!findTextureIndex(textureIndex, texture) && texture != nullptr)
    {
        if (!_textures.empty())
        {
            textureIndex = _textures[_textures.size() - 1].second + 1;
        }
        else
        {
            textureIndex = 1;
        }

        _textures.emplace_back(texture, textureIndex);

    }
    else if (texture == nullptr)
    {
        textureIndex = 0;
    }

    glm::vec2 p0 = glm::vec2(transform * glm::vec4(position, 1.f, 1.f));
    glm::vec2 p1 = glm::vec2(transform * glm::vec4(position + glm::vec2(size.x, 0.f), 1.f, 1.f));
    glm::vec2 p2 = glm::vec2(transform * glm::vec4(position + size, 1.f, 1.f));
    glm::vec2 p3 = glm::vec2(transform * glm::vec4(position + glm::vec2(0.f, size.y), 1.f, 1.f));

    _quadBuffer.emplace_back(p0, textureIndex, texCoords.Coord0, color);
    _quadBuffer.emplace_back(p1, textureIndex, texCoords.Coord1, color);
    _quadBuffer.emplace_back(p2, textureIndex, texCoords.Coord2, color);
    _quadBuffer.emplace_back(p3, textureIndex, texCoords.Coord3, color);

    const size_t offset = _quadBuffer.size() - 4;

    _quadIndexBuffer.push_back(offset);
    _quadIndexBuffer.push_back(offset + 1);
    _quadIndexBuffer.push_back(offset + 2);

    _quadIndexBuffer.push_back(offset + 2);
    _quadIndexBuffer.push_back(offset + 3);
    _quadIndexBuffer.push_back(offset);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateQuad(const glm::vec2& position, Sprite* sprite, const glm::vec2& size, const glm::vec3& color, const glm::mat4& transform)
{
    QuadTexCoords texCoords = sprite->GetTexCoords();
    texCoords.Normalize(sprite->GetParent()->GetWidth(), sprite->GetParent()->GetHeight());

    CreateQuad(position, sprite->GetParent(), size, color, transform, texCoords);
}

void Trinacria::DSL::Renderer::CreateQuad(const Transform& transform, Sprite* texture, const glm::vec3& color)
{
    CreateQuad(-transform.Pivot, texture, glm::vec2(1.f), color, transform.GetMatrix());
}

void Trinacria::DSL::Renderer::CreateTriangle(const struct TriangleData& triangleData)
{
    if (triangleData.Texture != nullptr)
    {
        CreateTriangle(triangleData.Transform, triangleData.Texture, triangleData.Orientation,
            triangleData.Color, triangleData.TexCoords);
    }
    else if (triangleData.Sprite != nullptr)
    {
        CreateTriangle(triangleData.Transform, triangleData.Sprite, triangleData.Orientation,
            triangleData.Color);
    }
    // using plain color
    else
    {
        CreateTriangle(triangleData.Transform, Texture::NO_TEXTURE, triangleData.Orientation,
            triangleData.Color);
    }
}

void Trinacria::DSL::Renderer::CreateQuad(const Transform& transform, Texture* texture, const glm::vec3& color, const QuadTexCoords& texCoords)
{
    CreateQuad(-transform.Pivot, texture, glm::vec2(1.f), color, transform.GetMatrix(), texCoords);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateTriangle(const glm::vec2& position, Texture* texture, TriangleOrientation orientation, const glm::vec2& size,
                                                   const glm::vec3& color, const glm::mat4& transform, const TriangleTexCoords& texCoords)
{
    if (_triangleBuffer.size() > MaxTrianglesVertices)
    {
        TRCN_LOG("Triangle vertex buffer overflowed please start a new batch!");
        return;
    }

    uint32_t textureIndex;

    if (!findTextureIndex(textureIndex, texture) && texture != nullptr)
    {
        if (!_textures.empty())
        {
            textureIndex = _textures[_textures.size() - 1].second + 1;
        }
        else
        {
            textureIndex = 1;
        }

        _textures.emplace_back(texture, textureIndex);

    }
    else if (texture == nullptr)
    {
        textureIndex = 0;
    }

    glm::vec2 p0 = transform * glm::vec4(position, 1.f, 1.f);
    glm::vec2 p3 = transform * glm::vec4(position.x, position.y + size.y, 1.f, 1.f);


    _triangleBuffer.emplace_back(p0, textureIndex, texCoords.Coord0, color);

    if (orientation == TriangleOrientation::Orientation_LEFT)
    {
        glm::vec2 p1 = transform * glm::vec4(position.x - size.x, position.y, 1.f, 1.f);
        _triangleBuffer.emplace_back(p1, textureIndex,
            texCoords.Coord1, color);
    }
    else if (orientation == TriangleOrientation::Orientation_RIGHT)
    {
        glm::vec2 p2 = transform * glm::vec4(position.x + size.x, position.y, 1.f, 1.f);
        _triangleBuffer.emplace_back(p2, textureIndex,
            texCoords.Coord1, color);
    }

    _triangleBuffer.emplace_back(p3, textureIndex,
        texCoords.Coord2, color);
}

void Trinacria::DSL::Renderer::CreateTriangle(const glm::vec2& position, Sprite* sprite,
    TriangleOrientation orientation, const glm::vec2& size, const glm::vec3& color, const glm::mat4& transform)
{
    TriangleTexCoords texCoords = sprite->GetTriangleTexCoords();
    texCoords.Normalize(sprite->GetParent()->GetWidth(), sprite->GetParent()->GetHeight());

    CreateTriangle(position, sprite->GetParent(), orientation, size, color, transform, texCoords);
}

void Trinacria::DSL::Renderer::CreateTriangle(const Transform& transform, Texture* texture,
    TriangleOrientation orientation, const glm::vec3& color, const TriangleTexCoords& texCoords)
{
    CreateTriangle(-transform.Pivot, texture, orientation,
        glm::vec2(0.5f),color, transform.GetMatrix(), texCoords);
}

void Trinacria::DSL::Renderer::CreateTriangle(const Transform& transform, Sprite* sprite,
    TriangleOrientation orientation, const glm::vec3& color)
{
    CreateTriangle(-transform.Pivot, sprite, orientation,
        glm::vec2(1.f), color, transform.GetMatrix());
}

void TRCN_CORE_NAMESPACE::Renderer::EndScene()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _quadBuffer.size() * sizeof(Vertex), _quadBuffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _quadIndexBuffer.size() * sizeof(uint32_t), _quadIndexBuffer.data());

    glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _triangleBuffer.size() * sizeof(Vertex), _triangleBuffer.data());
}

void TRCN_CORE_NAMESPACE::Renderer::Draw()
{
    glUseProgram(ShaderProgram.GetShaderProgram());
 
    for (auto& element : _textures)
    {
        std::string uniformName = std::string("u_Textures[") + std::to_string(element.second) + std::string("]");

        ShaderProgram.SetUniformInt(uniformName.c_str(), element.second - 1);
    }

    for (auto& element : _textures)
    {
        element.first->Bind(element.second + GL_TEXTURE0 - 1);
    }

    glBindVertexArray(_vao);

    glDrawElements(GL_TRIANGLES, _quadIndexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(_triangleVao);
    glDrawArrays(GL_TRIANGLES, 0, _triangleBuffer.size());
}

void TRCN_CORE_NAMESPACE::Renderer::FlushBuffers()
{
    _quadBuffer.clear();
    _quadIndexBuffer.clear();

    _triangleBuffer.clear();
}

void TRCN_CORE_NAMESPACE::Renderer::CleanUp()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);

    glDeleteVertexArrays(1, &_triangleVao);

    // deleting program in shader's deconstructor
}

void TRCN_CORE_NAMESPACE::Renderer::attribPointer(uint32_t location, uint32_t nParameters, uint32_t parameterType,
    size_t stride, void* offsetFromOrigin)
{
    glVertexAttribPointer(location, nParameters, parameterType, GL_FALSE, stride, offsetFromOrigin);
    glEnableVertexAttribArray(location);
}

bool TRCN_CORE_NAMESPACE::Renderer::findTextureIndex(uint32_t& out, const Texture* textureToFind)
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

void TRCN_CORE_NAMESPACE::Renderer::ClearColorBuffer(const glm::vec3& color)
{
    glClearColor(color.r, color.g, color.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Trinacria::DSL::Renderer::CreateQuad(const QuadData& quadData)
{
    if (quadData.Texture != nullptr)
    {
        CreateQuad(quadData.Transform, quadData.Texture, quadData.Color, quadData.TexCoords);
    }
    else if (quadData.Sprite != nullptr)
    {
        CreateQuad(quadData.Transform, quadData.Sprite, quadData.Color);
    }
    // using plain color
    else
    {
        CreateQuad(quadData.Transform, Texture::NO_TEXTURE, quadData.Color);
    }
}

void TRCN_CORE_NAMESPACE::QuadTexCoords::Normalize(uint32_t texWidth, uint32_t texHeight)
{
    Coord0.x = Coord0.x / texWidth;
    Coord0.y = Coord0.y / texHeight;

    Coord1.x = Coord1.x / texWidth;
    Coord1.y = Coord1.y / texHeight;

    Coord2.x = Coord2.x / texWidth;
    Coord2.y = Coord2.y / texHeight;

    Coord3.x = Coord3.x / texWidth;
    Coord3.y = Coord3.y / texHeight;
}

void Trinacria::DSL::TriangleTexCoords::Normalize(uint32_t texWidth, uint32_t texHeight)
{
    Coord0.x = Coord0.x / texWidth;
    Coord0.y = Coord0.y / texHeight;

    Coord1.x = Coord1.x / texWidth;
    Coord1.y = Coord1.y / texHeight;

    Coord2.x = Coord2.x / texWidth;
    Coord2.y = Coord2.y / texHeight;
}
