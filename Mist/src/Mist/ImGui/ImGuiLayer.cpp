#include "mistpch.h"

#include "ImGuiLayer.h"

#include "Mist/Core/Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <Mist/Renderer/Renderer2D.h>

namespace Mist {

ImGuiLayer::ImGuiLayer() :
    Layer("ImGuiLayer") {
}

ImGuiLayer::~ImGuiLayer() {
}

void ImGuiLayer::OnAttach() {
    MIST_PROFILE_FUNCTION();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // float fontSize = 18.0f; // *2.0f;
    // io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so 
    // platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // SetDarkThemeColors();

    GLFWwindow* window = static_cast<GLFWwindow*>(MIST_APP.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::OnDetach() {
    MIST_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& e) {
    if (!m_PassEvents) {
        ImGuiIO& io = ImGui::GetIO();
        e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();
}

void ImGuiLayer::Begin() {
    MIST_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    MIST_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();
    Window& window = MIST_APP.GetWindow();
    io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace Mist