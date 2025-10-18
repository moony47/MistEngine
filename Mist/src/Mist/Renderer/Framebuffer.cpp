#include "mistpch.h"
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h";
#include "Renderer.h"

namespace Mist {

Ref<Framebuffer> Mist::Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFramebuffer>(spec);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Mist