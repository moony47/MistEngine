#include "mistpch.h"
#include "OpenGLContext.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Mist {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
    m_WindowHandle(windowHandle) {
    MIST_ASSERT(windowHandle, "Window Handle is null!");
}

void OpenGLContext::Init() {
    MIST_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    MIST_ASSERT(status, "Failed to initialise GLAD!");
    MIST_CORE_INFO("Vendor:         {}", std::string((char*)glGetString(GL_VENDOR)));
    MIST_CORE_INFO("Renderer:       {}", std::string((char*)glGetString(GL_RENDERER)));
    MIST_CORE_INFO("OpenGL Version: {}", std::string((char*)glGetString(GL_VERSION)));
}

void OpenGLContext::SwapBuffers() {
    MIST_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
}

} // namespace Mist