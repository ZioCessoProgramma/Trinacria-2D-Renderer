#include "Trinacria/FrameBuffer.h"
#include <glad/glad.h>

void Trinacria::DSL::FrameBuffer::Cleanup()
{
    glDeleteFramebuffers(1, &_frameBufferId);
}

void Trinacria::DSL::FrameBuffer::GenFrameBuffer()
{
    glGenFramebuffers(1, &_frameBufferId);
}

void Trinacria::DSL::FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
}

void Trinacria::DSL::FrameBuffer::BindAttachTexture(uint32_t attachment, uint32_t textureType, uint32_t textureId) const
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureType, textureId, 0);
}
