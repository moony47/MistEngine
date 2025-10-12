#include "mistpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "OpenGL/OpenGLIndexBuffer.h"
#include "OpenGL/OpenGLVertexBuffer.h"

namespace Mist {

Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, size_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Mist