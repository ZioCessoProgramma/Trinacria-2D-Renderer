#include "Trinacria/FrameBuffer.h"
#include <glad/glad.h>

Trinacria::DSL::FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &_frameBufferId);

    Bind();
}

Trinacria::DSL::FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &_frameBufferId);
}

void Trinacria::DSL::FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
}

void Trinacria::DSL::FrameBuffer::AttachTexture(uint32_t attachment, uint32_t textureType, uint32_t textureId)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureType, textureId, 0);
}
