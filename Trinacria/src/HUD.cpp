#include "Trinacria/HUD.h"
#include <cstring>
#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "Trinacria/Assert.h"
#include "Trinacria/Macros.h"
#include "Trinacria/Renderer.h"

void TRCN_CORE_NAMESPACE::HUD::Init(const std::string& progressBarVertPath, const std::string& progressBarFragPath, const std::string& buttonVertPath, const std::string& buttonFragPath)
{
    setupProgressBars();
    setupButtons();

    _shader.LoadCoreShader(progressBarVertPath, progressBarFragPath);
    _shader.LoadCoreShader(buttonVertPath, buttonFragPath);


    _progressBarVertices.reserve(MaxProgressBars);
    _progressBarIndices.reserve(MaxProgressBarsIndices);

    _buttonVertices.reserve(MaxHUDVertices);
    _buttonIndices.reserve(MaxHUDIndices);
}

void TRCN_CORE_NAMESPACE::HUD::Cleanup()
{
    glDeleteVertexArrays(1, &_progressBarsVao);
    glDeleteBuffers(1, &_progressBarsVbo);
    glDeleteBuffers(1, &_progressBarsEbo);
}

void TRCN_CORE_NAMESPACE::HUD::EndHUD()
{
    glBindBuffer(GL_ARRAY_BUFFER, _progressBarsVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ProgressBarVertex) * _progressBarVertices.size(), _progressBarVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _progressBarsEbo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * _progressBarIndices.size(), _progressBarIndices.data());
}

void TRCN_CORE_NAMESPACE::HUD::CreateHUDQuad(const HUDQuadData& HUDQuad)
{
    TRCN_DEPEND_START("Create HUD Quad");

    TRCN_DEPEND_RETURN_ASSERT_VOID(_progressBarVertices.size() < MaxHUDVertices);
    TRCN_DEPEND_RETURN_ASSERT_VOID(_progressBarIndices.size() < MaxHUDIndices);

    uint32_t index = setupTexture(HUDQuad.texture);

    createProgressBar(-HUDQuad.transform.Pivot, HUDQuad.Color, index, glm::vec2(1),
                  HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, 0, -1.f, glm::vec4(0.f));
}

void Trinacria::DSL::HUD::AddOnClickFunction(const OnClickType& onClick, const Transform& transformOfTheQuad)
{
    _onClickAndPos.emplace_back(onClick, transformOfTheQuad);
}

void TRCN_CORE_NAMESPACE::HUD::CreateProgressBar(const HUDQuadData& HUDQuad, Texture* fillTexture, float progress, const glm::vec4& fillColor)
{
    TRCN_DEPEND_START("Create Progress Bar");

    TRCN_DEPEND_RETURN_ASSERT_VOID(_progressBarVertices.size() < MaxHUDVertices);
    TRCN_DEPEND_RETURN_ASSERT_VOID(_progressBarIndices.size() < MaxHUDIndices);

     uint32_t index = setupTexture(HUDQuad.texture);
    uint32_t fillTexIndex = setupTexture(fillTexture);

    createProgressBar(-HUDQuad.transform.Pivot, HUDQuad.Color, index, glm::vec2(1),
                  HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, fillTexIndex, progress, fillColor);
}

void TRCN_CORE_NAMESPACE::HUD::FlushBuffers()
{
    _progressBarVertices.clear();
    _progressBarIndices.clear();
}

void TRCN_CORE_NAMESPACE::HUD::draw()
{
    _shader.Bind();

    for (auto& tex : _textures)
    {
        std::string format = std::format("u_Textures[{}]", tex.second);

        _shader.SetUniformInt(format.c_str(), tex.second - 1);

        tex.first->Bind(tex.second + GL_TEXTURE0 - 1);
    }

    glBindVertexArray(_progressBarsVao);

    glDrawElements(GL_TRIANGLES, _progressBarIndices.size(), GL_UNSIGNED_INT, nullptr);

    Texture::ClearTextureSlots();
}

void TRCN_CORE_NAMESPACE::HUD::createProgressBar(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex,
                                             const glm::vec2& scale, const glm::mat4& matrix, const QuadTexCoords& coord, uint32_t fillTextureIndex, float progress, const
                                             glm::vec4& fillColor)
{
    glm::vec2 p0 = glm::vec2(matrix * glm::vec4(position, 0.f, 1.f));
    glm::vec2 p1 = glm::vec2(matrix * glm::vec4(position + glm::vec2(scale.x, 0.f), 0.f, 1.f));
    glm::vec2 p2 = glm::vec2(matrix * glm::vec4(position + scale, 0.f, 1.f));
    glm::vec2 p3 = glm::vec2(matrix * glm::vec4(position + glm::vec2(0.f, scale.y), 0.f, 1.f));

    _progressBarVertices.emplace_back(p0, color, textureIndex, coord.Coord0, fillTextureIndex, progress, fillColor);
    _progressBarVertices.emplace_back(p1, color, textureIndex, coord.Coord1, fillTextureIndex, progress, fillColor);
    _progressBarVertices.emplace_back(p2, color, textureIndex, coord.Coord2, fillTextureIndex, progress, fillColor);
    _progressBarVertices.emplace_back(p3, color, textureIndex, coord.Coord3, fillTextureIndex, progress, fillColor);

    size_t offset = _progressBarVertices.size() - 4;

    _progressBarIndices.push_back(offset);
    _progressBarIndices.push_back(offset + 1);
    _progressBarIndices.push_back(offset + 2);
    _progressBarIndices.push_back(offset + 2);
    _progressBarIndices.push_back(offset + 3);
    _progressBarIndices.push_back(offset);
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

uint32_t TRCN_CORE_NAMESPACE::HUD::setupTexture(Texture* texture)
{
    uint32_t index = 0;

    if (_textures.empty())
    {
        index = 1;
        _textures.emplace_back(texture, index);
    }
    else if (!findTextureIndex(index, texture) && texture)
    {
        index = _textures[_textures.size() - 1].second + 1;
        _textures.emplace_back(texture, index);
    }

    return index;
}

void TRCN_CORE_NAMESPACE::HUD::UpdateEvents(GLFWwindow* window, const glm::vec2& windowDimensions)
{
    // cannot subscribe to input poller layer because it is a layer and it is logically incorrect

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _lastStateOfLeftMouseButton == GLFW_RELEASE)
    {
        // TODO: check if is in button range

        for (auto& el : _onClickAndPos)
        {
            if (isInRange(el.second, window, windowDimensions)) el.first();
        }

        _lastStateOfLeftMouseButton = GLFW_PRESS;
    }
    else
    {
        _lastStateOfLeftMouseButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }
}

bool Trinacria::DSL::HUD::isInRange(const Transform& transform, GLFWwindow* window, const glm::vec2& windowDimensions)
{
    double _xPos, _yPos;
    glfwGetCursorPos(window, &_xPos, &_yPos);

    auto xPos = ((float)_xPos / windowDimensions.x - 0.5f) * 2;
    auto yPos = -((float)_yPos / windowDimensions.y - 0.5f) * 2;

    // point to find
    glm::vec2 P(xPos, yPos);

    glm::mat4 matrix = transform.GetMatrix();

    glm::vec2 A = glm::vec2(matrix * glm::vec4(-transform.Pivot, 0.f, 1.f));
    glm::vec2 B = glm::vec2(matrix * glm::vec4(-transform.Pivot + glm::vec2(1.f, 0.f), 0.f, 1.f));
    glm::vec2 D = glm::vec2(matrix * glm::vec4(-transform.Pivot + glm::vec2(0.f, 1.f), 0.f, 1.f));

    glm::vec2 AB = B - A; // one length
    glm::vec2 AD = D - A; // another one

    glm::vec2 AP = P - A;

    float lengthSquaredAB = glm::dot(AB, AB);
    float lengthSquaredAD = glm::dot(AD, AD);

    float dotABAP = glm::dot(AB, AP);
    float dotADAP = glm::dot(AD, AP);

    bool b = (dotABAP >= 0 && dotABAP <= lengthSquaredAB);
    bool b1 = (dotADAP >= 0 && dotADAP <= lengthSquaredAD);

    return b && b1;
}

void Trinacria::DSL::HUD::setupProgressBars()
{
    glGenVertexArrays(1, &_progressBarsVao);
    glBindVertexArray(_progressBarsVao);

    glGenBuffers(1, &_progressBarsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _progressBarsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ProgressBarVertex) * MaxProgressBarsVertices, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_progressBarsEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _progressBarsEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxProgressBarsIndices, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ProgressBarVertex),
        (void*)offsetof(ProgressBarVertex, Position));

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ProgressBarVertex),
       (void*)offsetof(ProgressBarVertex, Color));

    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(ProgressBarVertex),
        (void*)offsetof(ProgressBarVertex, TextureIndex));

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(ProgressBarVertex), (void*)offsetof(ProgressBarVertex, TexCoord));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(ProgressBarVertex), (void*)(offsetof(ProgressBarVertex, FillTextureIndex)));
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(ProgressBarVertex), (void*)offsetof(ProgressBarVertex, Progress));
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(ProgressBarVertex), (void*)offsetof(ProgressBarVertex, FillColor));
    glEnableVertexAttribArray(6);
}

void Trinacria::DSL::HUD::setupButtons()
{
    glGenVertexArrays(1, &_buttonsVao);
    glBindVertexArray(_buttonsVao);

    glGenBuffers(1, &_buttonsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _buttonsVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(ButtonVertex) * MaxHUDVertices, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_buttonsEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buttonsEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxHUDIndices, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, Position));

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, Color));

    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(ButtonVertex),
        (void*)offsetof(ButtonVertex, TextureIndex));

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
        sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, TexCoord));

    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
        sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, HoveredColor));

    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
        sizeof(ButtonVertex), (void*)offsetof(ButtonVertex, PressedColor));

    glEnableVertexAttribArray(5);
}

void TRCN_CORE_NAMESPACE::HUD::createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex, const glm::vec2& scale, const glm::mat4& matrix, const QuadTexCoords& coords, const glm::vec4& hoveredColor, const glm::vec4& pressedColor)
{
    glm::vec2 p0 = glm::vec2(matrix * glm::vec4(position, 0.f, 1.f));
    glm::vec2 p1 = glm::vec2(matrix * glm::vec4(position + glm::vec2(scale.x, 0.f), 0.f, 1.f));
    glm::vec2 p2 = glm::vec2(matrix * glm::vec4(position + scale, 0.f, 1.f));
    glm::vec2 p3 = glm::vec2(matrix * glm::vec4(position + glm::vec2(0.f, scale.y), 0.f, 1.f));

    _buttonVertices.emplace_back(p0, color, textureIndex, coords.Coord0, hoveredColor, pressedColor);
    _buttonVertices.emplace_back(p1, color, textureIndex, coords.Coord1, hoveredColor, pressedColor);
    _buttonVertices.emplace_back(p2, color, textureIndex, coords.Coord2, hoveredColor, pressedColor);
    _buttonVertices.emplace_back(p3, color, textureIndex, coords.Coord3, hoveredColor, pressedColor);

    size_t offset = _progressBarVertices.size() - 4;

    _buttonIndices.push_back(offset);
    _buttonIndices.push_back(offset + 1);
    _buttonIndices.push_back(offset + 2);
    _buttonIndices.push_back(offset + 2);
    _buttonIndices.push_back(offset + 3);
    _buttonIndices.push_back(offset);

}

void TRCN_CORE_NAMESPACE::HUD::CreateButton(const HUDQuadData& HUDQuad, const glm::vec4& hoveredColor, const glm::vec4& pressedColor)
{
    TRCN_DEPEND_START("Create Button");
    TRCN_DEPEND_ASSERT(_buttonVertices.size() <= MaxHUDVertices);
    TRCN_DEPEND_ASSERT(_buttonIndices.size() <= MaxHUDIndices);

    uint32_t index = setupTexture(HUDQuad.texture);

    createHUDQuad(-HUDQuad.transform.Pivot, HUDQuad.Color, index, glm::vec2(1.f), HUDQuad.transform.GetMatrix(), HUDQuad.TexCoords, hoveredColor, pressedColor);
}
