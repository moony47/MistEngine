#include "mistpch.h"
#include "RenderCommand.h"

#include "OpenGL/OpenGLRendererAPI.h"

namespace Mist {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}