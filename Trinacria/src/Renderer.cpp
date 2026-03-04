#include "Trinacria/Renderer.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <format>
#include "Trinacria/Sprite.h"
#include "Trinacria/Log.h"

std::vector<TRCN_CORE_NAMESPACE::Vertex> TRCN_CORE_NAMESPACE::Renderer::_buffer;
std::vector<uint32_t> TRCN_CORE_NAMESPACE::Renderer::_indexBuffer;

uint32_t TRCN_CORE_NAMESPACE::Renderer::_vao;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_vbo;
uint32_t TRCN_CORE_NAMESPACE::Renderer::_ebo;

TRCN_CORE_NAMESPACE::Shader TRCN_CORE_NAMESPACE::Renderer::ShaderProgram;

const size_t TRCN_CORE_NAMESPACE::Renderer::MaxQuads;
const size_t TRCN_CORE_NAMESPACE::Renderer::MaxVertices;
const size_t TRCN_CORE_NAMESPACE::Renderer::MaxIndices;

std::vector<std::pair<TRCN_CORE_NAMESPACE::Texture*, uint32_t>> TRCN_CORE_NAMESPACE::Renderer::_textures;

void TRCN_CORE_NAMESPACE::Renderer::Init()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndices * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    attribPointer(0, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Position));
 
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, sizeof(int), GL_INT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TextureIndex));

    attribPointer(2, 2, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, TexCoords));
    attribPointer(3, 3, GL_FLOAT, sizeof(TRCN_CORE_NAMESPACE::Vertex), (void*)offsetof(TRCN_CORE_NAMESPACE::Vertex, Color));

    _buffer.reserve(MaxVertices);
    _indexBuffer.reserve(MaxIndices);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateQuad(const glm::vec2& position, Texture* texture, float size, const glm::vec3& color, const QuadTexCoords& texCoords)
{
    if (_buffer.size() >= MaxVertices)
    {
        TRCN_LOG("Vertex buffer overflowed please start a new batch!");
        return;
    }

    if (_indexBuffer.size() >= MaxIndices)
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

    _buffer.emplace_back(position, textureIndex, texCoords.Coord0, color);
    _buffer.emplace_back(position + glm::vec2(size, 0.f), textureIndex, texCoords.Coord1, color);
    _buffer.emplace_back(position + glm::vec2 (size), textureIndex, texCoords.Coord2, color);
    _buffer.emplace_back(position + glm::vec2(0.f, size), textureIndex, texCoords.Coord3, color);

    const size_t offset = _buffer.size() - 4;

    _indexBuffer.push_back(offset);
    _indexBuffer.push_back(offset + 1);
    _indexBuffer.push_back(offset + 2);

    _indexBuffer.push_back(offset + 2);
    _indexBuffer.push_back(offset + 3);
    _indexBuffer.push_back(offset);
}

void TRCN_CORE_NAMESPACE::Renderer::CreateQuad(const glm::vec2& position, Sprite* sprite, float size, const glm::vec3& color)
{
    QuadTexCoords texCoords = sprite->GetTexCoords();
    texCoords.Normalize(sprite->GetParent()->GetWidth(), sprite->GetParent()->GetHeight());

    CreateQuad(position, sprite->GetParent(), size, color, texCoords);
}

void TRCN_CORE_NAMESPACE::Renderer::EndScene()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _buffer.size() * sizeof(Vertex), _buffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indexBuffer.size() * sizeof(uint32_t), _indexBuffer.data());
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

    glDrawElements(GL_TRIANGLES, _indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
}

void TRCN_CORE_NAMESPACE::Renderer::FlushBuffers()
{
    _buffer.clear();
    _indexBuffer.clear();
}

void TRCN_CORE_NAMESPACE::Renderer::CleanUp()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);

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
