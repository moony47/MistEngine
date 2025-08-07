#include "mistpch.h"
#include "WindowsWindow.h"

#include "ImGuiGLFWRenderer.h"
#include "ImGuiOpenGLRenderer.h"
#include <imgui.h>

#include "Mist/Events/ApplicationEvent.h"
#include "Mist/Events/KeyEvent.h"
#include "Mist/Events/MouseEvent.h"

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

    MIST_CORE_INFO("Creating Window: {0} ({1} {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        int sucess = glfwInit();
        MIST_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

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

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    MIST_CORE_INFO("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));

    MS_GLCALL(glEnable(GL_BLEND));
    MS_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();

    m_IO.reset(&ImGui::GetIO());
    m_IO->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    m_IO->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    TEMP_layer = new TestLayer(1600, 900);
}

void WindowsWindow::Shutdown() {
    delete TEMP_layer;
    glfwDestroyWindow(m_Window);
}

void WindowsWindow::OnUpdate() {
    TEMP_layer->Update(m_IO);

    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return m_Data.VSync;
}

} // namespace Mist