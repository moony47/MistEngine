#include "mistpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "OpenGL/OpenGLIndexBuffer.h"
#include "OpenGL/OpenGLVertexBuffer.h"

namespace Mist {

IndexBuffer* IndexBuffer::Create(const uint32_t* indices, size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, size);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

VertexBuffer* VertexBuffer::Create(const float* vertices, size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Mist