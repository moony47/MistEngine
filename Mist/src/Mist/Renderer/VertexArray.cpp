#include "mistpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "OpenGL/OpenGLVertexArray.h"

namespace Mist {

	Mist::VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			MIST_CORE_ASSERT(false, "RenderAPI::None is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		MIST_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}