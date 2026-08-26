#pragma once

#include <cstdint>

#include "Macros.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "glm/glm.hpp"

namespace TRCN_CORE_NAMESPACE
{
    class Sprite;
    /**
     * @brief struct containing all data a hud quad needs
     */
    struct HUDQuadData
    {
        /**
         * @brief constructor for quad with textures
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param texture the texture of the quad
         * @param color the color of the quad, it'll be multiplied with the texture color e.j. if the color is set to red the texture will appear red-ish
         * @note use this only when dealing with textured quads. If it is plain color omit the texture parameter, it has a special constructor
         */

        HUDQuadData(const Transform& transform, Texture* texture, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), texture(texture), Color(color) { }

        /**
         * @brief constructor for quad with sprites
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param sprite the sprite of the quad
         * @param color the color of the quad, it'll be multiplied with the sprite color e.j. if the color is set to red the sprite will appear red-ish
         * @note use this only when dealing with sprite quads. If it is plain color omit the sprite parameter, it has a special constructor
         */

        HUDQuadData(const Transform& transform, Sprite* sprite, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), sprite(sprite), Color(color) { }

        /**
         * @brief the constructor for plain color quads
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param color the color of the quad
         */

        HUDQuadData(const Transform& transform, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), Color(color) { }

        /**
         * @brief the transform of the quad, it has position, scale, rotation etc...
         */

        Transform transform;

        /**
         * @brief the texture of the quad
         * @note if set to nullptr it'll use plain color or sprite
         */

        Texture* texture = nullptr;

        /**
         * @brief the sprite of the quad
         * @note if set to nullptr it'll use plain color or texture
         */

        Sprite* sprite = nullptr;

        /**
         * @brief the color of the quad, it'll be multiplied with the sprite/texture color e.j. if the color is set to red the sprite/texture will appear red-ish
         */

        glm::vec4 Color;

        /**
         * @brief texture coordinates for quad
         * @note change it only if you know what you are doing
         */

        QuadTexCoords TexCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
                                                glm::vec2(1.f), glm::vec2(0.f, 1.f));
    };

    /**
     * @brief HUDVertex is the vertex that is used by the HUD
     */

    struct HUDVertex
    {
        /**
         * @brief position in screen in range [0, 1] [0, 1], pivot is at bottom left
         */

        glm::vec2 Position;

        /**
         * @brief the color the vertex should have
         */

        glm::vec4 Color;

        /**
         * @brief texture index in texture array
         * @note the texture array for the HUD is different from the one of the Renderer
         */

        uint32_t TextureIndex;

        /**
         * @brief the tex coordinate (range [0; 1] [0; 1]) associated with this vertex
         */

        glm::vec2 TexCoord;
    };

    class HUD
    {
    public:

        /**
         * @brief initializes the HUD
         * @param vertPath the path to the vertex shader that draws the HUD
         * @param fragPath the path to the fragment shader that draws the HUD
         * @note to call once at the start of the application
         */

        static void Init(const std::string& vertPath, const std::string& fragPath);

        /**
        * @brief cleans up the HUD
        * @note to call once at the end of the application
        */

        static void Cleanup();

        /**
         * @brief deals with openGL buffers
         * @note call when finished the HUD
         */

        static void EndHUD();

        /**
         * @brief creates a quad that will be later drawn
         * @param HUDQuad the quad to create
         */

        static void CreateHUDQuad(const HUDQuadData& HUDQuad);

        /**
         * @brief flushes all the buffers
         * @note call it at the end of the frame after Renderer::Draw
         */

        // TODO: add overload that makes you flush only a part of the buffer
        static void FlushBuffers();

        inline static Shader _shader;

        static constexpr size_t MaxHUDQuads    =            100;
        static constexpr size_t MaxHUDVertices = MaxHUDQuads * 4;
        static constexpr size_t MaxHUDIndices  = MaxHUDQuads * 6;

    private:

        friend class Renderer;

        inline static uint32_t _vao;
        inline static uint32_t _vbo;
        inline static uint32_t _ebo;

        inline static std::vector<HUDVertex> _vertices;
        inline static std::vector<uint32_t> _indices;

        inline static std::vector<std::pair<Texture*, uint32_t>> _textures;


        static void draw();

        static void createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex, const glm::vec2& scale, const glm::mat4& matrix, const
                                  QuadTexCoords& coord);

        static bool findTextureIndex(uint32_t& outIndex, const Texture* texToFind);
    };
}
