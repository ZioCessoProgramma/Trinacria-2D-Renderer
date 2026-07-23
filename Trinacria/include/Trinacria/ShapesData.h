#pragma once

#include "Macros.h"
#include "Sprite.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "Materials.h"

namespace TRCN_CORE_NAMESPACE
{
    struct QuadData
    {
        QuadData(const Transform& transform, Texture* texture, int materialIndex = 0, const glm::vec3& color = glm::vec3(1.f)) :
            transform(transform), texture(texture), Color(color), MaterialIndex(materialIndex){ }

        QuadData(const Transform& transform, Sprite* sprite, int materialIndex = 0, const glm::vec3& color = glm::vec3(1.f)) :
            transform(transform), sprite(sprite), Color(color), MaterialIndex(materialIndex) { }

        QuadData(const Transform& transform, const glm::vec3& color = glm::vec3(1.f), int materialIndex = 0) :
            transform(transform), Color(color), MaterialIndex(materialIndex) { }

        Transform transform;
        Texture* texture = nullptr;
        Sprite* sprite = nullptr;
        glm::vec3 Color;

        int MaterialIndex;

        QuadTexCoords TexCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
            glm::vec2(1.f), glm::vec2(0.f, 1.f));
    };

    struct TriangleData
    {
        TriangleData(const Transform& transform, Texture* texture,
            int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec3& color = glm::vec3(1.f)) :
                transform(transform), texture(texture), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

        TriangleData(const Transform& transform,
            Sprite* sprite, int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec3& color = glm::vec3(1.f)) :
                transform(transform), sprite(sprite), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

        TriangleData(const Transform& transform,
            int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec3& color = glm::vec3(1.f)) :
            transform(transform), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

        Transform transform;
        Texture* texture = nullptr;
        Sprite* sprite = nullptr;
        glm::vec3 Color;
        int MaterialIndex = 0;

        TriangleOrientation Orientation;

        TriangleTexCoords TexCoords = TriangleTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
            glm::vec2(0.f, 1.f));
    };
}