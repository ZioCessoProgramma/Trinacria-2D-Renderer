#pragma once

#include <cstdint>

#include "Macros.h"
#include "glm/glm.hpp"

namespace TRCN_CORE_NAMESPACE
{
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
         * @note to call once at the start of the application
         */

        static void Init();

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
         * @brief the maximum number of HUD vertices in the screen
         * @warning to change this always use a multiplier of 4
         */

        static constexpr size_t MaxHUDQuads    =            100;
        static constexpr size_t MaxHUDVertices = MaxHUDQuads * 4;
        static constexpr size_t MaxHUDIndices  = MaxHUDQuads * 6;

    private:

        friend class Renderer;

        static void draw() {}

        inline static uint32_t vao;
        inline static uint32_t vbo;
        inline static uint32_t ebo;
    };
}
