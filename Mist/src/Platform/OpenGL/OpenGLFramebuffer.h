#pragma once

#include "Mist/Renderer/Framebuffer.h"

namespace Mist {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer();

    void Invalidate();
    void Resize(uint32_t width, uint32_t height) override;

    inline uint32_t GetColourAttachment() const override {
        return m_ColourAttachment;
    }

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
    uint32_t m_ColourAttachment = 0, m_DepthAttachment = 0;
    FramebufferSpecification m_Spec;
};

} // namespace Mist