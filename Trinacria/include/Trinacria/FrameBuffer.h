#pragma once

#include <cstdint>

#include "Macros.h"

namespace TRCN_CORE_NAMESPACE
{
    class FrameBuffer
    {
    public:
        void Cleanup();

        void GenFrameBuffer();
        void Bind() const;

        void BindAttachTexture(uint32_t attachment, uint32_t textureType, uint32_t textureId) const;

    private:
        uint32_t _frameBufferId = 0;
    };
}