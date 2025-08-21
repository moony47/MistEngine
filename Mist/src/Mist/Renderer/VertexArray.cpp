#include "mistpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "OpenGL/OpenGLVertexArray.h"

namespace Mist {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		MIST_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}