#include "mistpch.h"
#include "WindowsWindow.h"

#include "Mist/Events/EventSystem.h"
#include "Mist/Renderer/Renderer.h"

#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLContext.h"

namespace Mist {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
    MIST_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    MIST_PROFILE_FUNCTION();

    Init(props);
}

WindowsWindow::~WindowsWindow() {
    MIST_PROFILE_FUNCTION();

    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    MIST_PROFILE_FUNCTION();

    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    MIST_CORE_TRACE("Creating Window: {0} ({1} {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        MIST_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(false);

    InitEventCallbacks();
}

void WindowsWindow::Shutdown() {
    MIST_PROFILE_FUNCTION();

    //delete TEMP_layer;
    delete m_Context;
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

DeltaTime WindowsWindow::OnUpdate() {
    MIST_PROFILE_FUNCTION();

    float time = (float)glfwGetTime();
    DeltaTime deltaTime = time - m_LastFrameTime;
    m_LastFrameTime = time;

    glfwPollEvents();
    return deltaTime;
}

void WindowsWindow::OnFrameStart(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    RenderCommand::Clear();
}

void WindowsWindow::OnFrameEnd(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
    MIST_PROFILE_FUNCTION();

    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return m_Data.VSync;
}

void WindowsWindow::InitEventCallbacks() {
    MIST_PROFILE_FUNCTION();

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent e(width, height);
        data.EventCallback(e);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        WindowCloseEvent e;
        data.EventCallback(e);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent e((KeyCode)key, 0);
                data.EventCallback(e);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent e((KeyCode)key, 0);
                data.EventCallback(e);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent e((KeyCode)key, 1);
                data.EventCallback(e);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent e((MouseButtonCode)button);
                data.EventCallback(e);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent e((MouseButtonCode)button);
                data.EventCallback(e);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent e((float)xOffset, (float)yOffset);
        data.EventCallback(e);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent e((float)x, (float)y);
        data.EventCallback(e);
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t c) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent e((KeyCode)c);
        data.EventCallback(e);
    });
}

} // namespace Mist