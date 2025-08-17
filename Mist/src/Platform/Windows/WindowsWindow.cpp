#include "mistpch.h"
#include "WindowsWindow.h"

#include "Mist/Events/EventSystem.h"

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
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
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
    SetVSync(true);

    InitEventCallbacks();

	MIST_GLCALL(glEnable(GL_BLEND));
	MIST_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_LastTime = (float)glfwGetTime();

    //TEMP_layer = new TestLayer(m_Data.Width, m_Data.Height);
}

void WindowsWindow::Shutdown() {
    //delete TEMP_layer;
    delete m_Context;
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowsWindow::OnUpdateStart() {
    // Render here
}

void WindowsWindow::OnUpdateEnd() {
	float currentTime = (float)glfwGetTime();
    float deltaTime = m_LastTime - currentTime;
    m_LastTime = currentTime;

    //TEMP_layer->Update(deltaTime, ImGui::GetIO(), m_Renderer);

    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return m_Data.VSync;
}

void WindowsWindow::Resize(unsigned int width, unsigned int height) {
    //TEMP_layer->Resize(width, height);
    glViewport(0, 0, width, height);
}

void WindowsWindow::InitEventCallbacks() {
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
                KeyPressedEvent e(key, 0);
                data.EventCallback(e);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent e(key, 0);
                data.EventCallback(e);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent e(key, 1);
                data.EventCallback(e);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent e(button);
                data.EventCallback(e);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent e(button);
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

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int c) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent e(c);
        data.EventCallback(e);
    });
}

} // namespace Mist