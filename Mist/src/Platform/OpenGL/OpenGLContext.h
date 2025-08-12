#pragma once

#include "Mist/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Mist {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

} // namespace Mist
