#include "mistpch.h"
#include "OpenGLFramebuffer.h"

#include <GLAD/glad.h>
#include <glm/common.hpp>

namespace Mist {

static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils {

static GLenum TextureTarget(bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void AttachColourTexture(uint32_t attachmentID,
                                uint32_t samples,
                                GLint internalFormat,
                                GLenum format,
                                uint32_t width,
                                uint32_t height,
                                int index) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), attachmentID, 0);
}

static void AttachDepthTexture(
    uint32_t attachmentID, uint32_t samples, GLint format, GLenum attachmentType, uint32_t width, uint32_t height) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), attachmentID, 0);
}

static void CreateTextures(bool multisampled, uint32_t* outAttachmentIDs, size_t count) {
    glCreateTextures(TextureTarget(multisampled), (GLsizei) count, outAttachmentIDs);
}

static void BindTexture(bool multisampled, uint32_t attachmentID) {
    glBindTexture(TextureTarget(multisampled), attachmentID);
}

static bool IsDepthFormat(FramebufferTextureFormat format) {
    switch (format) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return true;
    }

    return false;
}

static GLenum GLFormat(FramebufferTextureFormat fbFormat) {
    switch (fbFormat) {
        case FramebufferTextureFormat::RGBA8:
            return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER:
            return GL_RED_INTEGER;
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return GL_DEPTH24_STENCIL8;
    }

    MIST_CORE_ASSERT(false, "[TextureFormatToGL] Unknown FramebufferTextureFormat");
    return 0;
}

} // namespace Utils

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) :
    m_Spec(spec) {

    for (auto texSpec : m_Spec.Attachments.Attachments) {
        if (!Utils::IsDepthFormat(texSpec.TextureFormat))
            m_ColourAttachmentSpecs.emplace_back(texSpec);
        else
            m_DepthAttachmentSpec = texSpec;
    }

    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    DeleteTextures();
}

void OpenGLFramebuffer::DeleteTextures() {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures((GLsizei)m_ColourAttachments.size(), m_ColourAttachments.data());
    m_ColourAttachments.clear();
    glDeleteTextures(1, &m_DepthAttachment);
    m_DepthAttachment = 0;
}

void OpenGLFramebuffer::Invalidate() {
    if (m_RendererID)
        DeleteTextures();

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    bool multisampled = m_Spec.Samples > 1;
    size_t colourCount = m_ColourAttachmentSpecs.size();
    if (colourCount > 0) {
        m_ColourAttachments.resize(colourCount);
        Utils::CreateTextures(multisampled, m_ColourAttachments.data(), colourCount);

        // Attachments
        for (size_t i = 0; i < colourCount; i++) {
            Utils::BindTexture(multisampled, m_ColourAttachments[i]);
            switch (m_ColourAttachmentSpecs[i].TextureFormat) {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColourTexture(m_ColourAttachments[i], m_Spec.Samples, GL_RGBA8, GL_RGBA, m_Spec.Width,
                                               m_Spec.Height, (int)i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColourTexture(m_ColourAttachments[i], m_Spec.Samples, GL_R32I, GL_RED_INTEGER,
                                               m_Spec.Width, m_Spec.Height, (int)i);
                    break;
            }
        }
    }

    if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None) {
        Utils::CreateTextures(multisampled, &m_DepthAttachment, 1);
        Utils::BindTexture(multisampled, m_DepthAttachment);
        switch (m_DepthAttachmentSpec.TextureFormat) {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8,
                                          GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
                break;
        }
    }

    if (colourCount > 1) {
        MIST_CORE_ASSERT(colourCount <= 4, "[OpenGLFramebuffer::Invalidate] Maximum of 4 colour attachments supported");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers((GLsizei)colourCount, buffers);
    } else if (!colourCount) {
        glDrawBuffer(GL_NONE);
    }

    MIST_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                     "[OpenGLFramebuffer::Invalidate] Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    width = std::clamp<uint32_t>(width, 1, s_MaxFramebufferSize);
    height = std::clamp<uint32_t>(height, 1, s_MaxFramebufferSize);

    m_Spec.Width = width;
    m_Spec.Height = height;
    Invalidate();
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
    MIST_CORE_ASSERT(attachmentIndex < m_ColourAttachments.size(),
                     "[OpenGLFramebuffer::ReadPixel] Attachment index is out of range")

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLFramebuffer::ClearColourAttachment(size_t index, int clearValue) {
    MIST_CORE_ASSERT(index < m_ColourAttachments.size(), "[OpenGLFramebuffer::ClearColourAttachment] Attachment index out of range");

    auto& spec = m_ColourAttachmentSpecs[index];
    glClearTexImage(m_ColourAttachments[index], 0, Utils::GLFormat(spec.TextureFormat), GL_INT, &clearValue);
}

void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Spec.Width, m_Spec.Height);
}

void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Mist