#include "mistpch.h"

#include "ShaderController.h"
#include "Renderer.h"

#include "OpenGL/OpenGLShaderController.h"

namespace Mist {

	ShaderController* ShaderController::s_Instance = new OpenGLShaderController;

} // namespace Mist