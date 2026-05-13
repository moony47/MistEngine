#include "VertexArray.h"
#include "mistpch.h"

#include "OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Mist {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexArray>();
    }

    MIST_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Mist