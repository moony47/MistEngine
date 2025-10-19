#include "mistpch.h"
#include "OpenGLFramebuffer.h"

#include <GLAD/glad.h>
#include <glm/common.hpp>

namespace Mist {

static const uint32_t s_MaxFramebufferSize = 8192;

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) :
    m_Spec(spec) {
    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_ColourAttachment);
    glDeleteTextures(1, &m_DepthAttachment);
}

void OpenGLFramebuffer::Invalidate() {
    if (m_RendererID) {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColourAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColourAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Spec.Width, m_Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Spec.Width, m_Spec.Height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

    MIST_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,  "[OpenGLFramebuffer::Invalidate] Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    width = std::clamp<uint32_t>(width, 1, s_MaxFramebufferSize);
    height = std::clamp<uint32_t>(height, 1, s_MaxFramebufferSize);

    //MIST_CORE_INFO("[OpenGLFramebuffer::Resize] Framebuffer resized to {0}, {1}", width, height);

    m_Spec.Width = width;
    m_Spec.Height = height;
    Invalidate();
}

void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Spec.Width, m_Spec.Height);
}

void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Mist