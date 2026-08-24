#pragma once

#include <cstdint>

#include "Macros.h"
#include "Shader.h"
#include "Transform.h"
#include "glm/glm.hpp"

namespace TRCN_CORE_NAMESPACE
{
    /**
     * @brief struct containing all data a hud quad needs
     */
    struct HUDQuad
    {
        /**
         * @brief the transform of the quad
         */

        Transform transform;

        /**
        * @brief texture index in texture array
        * @note the texture array for the HUD is different from the one of the Renderer
        */

        uint32_t TextureIndex;

        /**
         * @brief the color of the hud quad
         */

        glm::vec4 Color;
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

        static void CreateHUDQuad(const HUDQuad& HUDQuad);

        /**
         * @brief flushes all the buffers
         * @note call it at the end of the frame after Renderer::Draw
         */

        // TODO: add overload that makes you flush only a part of the buffer
        static void FlushBuffers();

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

        inline static Shader _shader;

        static void draw();

        static void createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex, const glm::vec2& scale, const glm::mat4& matrix);
    };
}
