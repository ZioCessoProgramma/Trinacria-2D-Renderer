#pragma once

#include <cstdint>

#include "Macros.h"

namespace TRCN_CORE_NAMESPACE
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind();

        void AttachTexture(uint32_t attachment, uint32_t textureType, uint32_t textureId);

    private:
        uint32_t _frameBufferId;
    };
}