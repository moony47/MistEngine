#pragma once

#include "Mist/Renderer/Framebuffer.h"

namespace Mist {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer();

    void DeleteTextures();

    void Invalidate();
    void Resize(uint32_t width, uint32_t height) override;
    int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    inline uint32_t GetColourAttachment(size_t i = 0) const override {
        MIST_CORE_ASSERT(i < m_ColourAttachments.size(), "[OpenGLFramebuffer::GetColourAttachment] Index out of range");
        return m_ColourAttachments[i];
    }
    inline uint32_t GetDepthAttachment() const override {
        return m_DepthAttachment;
    }

    void ClearColourAttachment(size_t index, int clearValue) override;

    void Bind() override;
    void Unbind() override;

    // inline FramebufferSpecification& GetSpecification() override {
    //     return m_Spec;
    // }
    inline const FramebufferSpecification& GetSpecification() const override {
        return m_Spec;
    }

private:
    uint32_t m_RendererID = 0;
    FramebufferSpecification m_Spec;

    std::vector<FramebufferTextureSpecification> m_ColourAttachmentSpecs;
    FramebufferTextureSpecification m_DepthAttachmentSpec;

    std::vector<uint32_t> m_ColourAttachments;
    uint32_t m_DepthAttachment = 0;
};

} // namespace Mist