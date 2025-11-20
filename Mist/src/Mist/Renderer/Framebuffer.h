#pragma once

namespace Mist {

enum class FramebufferTextureFormat {
    None = 0,

    // Colour
    RGBA8,
    RED_INTEGER,

    // Depth/Stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format) :
        TextureFormat(format) {};
};

struct FramebufferAttachmentSpecification {
    std::vector<FramebufferTextureSpecification> Attachments;

    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> list) :
        Attachments(list) {};
};

struct FramebufferSpecification {
    uint32_t Width, Height;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SwapChainTarget = false;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    // virtual FramebufferSpecification& GetSpecification() = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual uint32_t GetColourAttachment(size_t i = 0) const = 0;
    virtual uint32_t GetDepthAttachment() const = 0;

    virtual void ClearColourAttachment(size_t index, int clearValue) = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

} // namespace Mist