#include "mistpch.h"
#include "UniformBuffer.h"

#include "Mist/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include "RendererAPI.h"
#include <Mist/Core/Core.h>
#include <cstdint>
namespace Mist {

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLUniformBuffer>(size, binding);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Mist