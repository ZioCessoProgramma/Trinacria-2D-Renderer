#pragma once

#include "Macros.h"
#include "Sprite.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"

namespace TRCN_CORE_NAMESPACE
{
    struct QuadData
    {
        QuadData(const Transform& transform, Texture* texture, const glm::vec3& color = glm::vec3(1.f)) :
            Transform(transform), Texture(texture), Color(color) { }

        QuadData(const Transform& transform, Sprite* sprite, const glm::vec3& color = glm::vec3(1.f)) :
            Transform(transform), Sprite(sprite), Color(color) { }

        Transform Transform;
        Texture* Texture = nullptr;
        Sprite* Sprite = nullptr;
        glm::vec3 Color;

        QuadTexCoords TexCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
            glm::vec2(1.f), glm::vec2(0.f, 1.f));
    };

    struct TriangleData
    {
        TriangleData(const Transform& transform, Texture* texture, TriangleOrientation orientation,
            const glm::vec3& color = glm::vec3(1.f)) :
                Transform(transform), Texture(texture), Orientation(orientation), Color(color) { }

        TriangleData(const Transform& transform, Sprite* sprite, TriangleOrientation orientation,
            const glm::vec3& color = glm::vec3(1.f)) :
                Transform(transform), Sprite(sprite), Orientation(orientation), Color(color) { }

        Transform Transform;
        Texture* Texture = nullptr;
        Sprite* Sprite = nullptr;
        glm::vec3 Color;

        TriangleOrientation Orientation;

        TriangleTexCoords TexCoords = TriangleTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
            glm::vec2(0.f, 1.f));
    };
}