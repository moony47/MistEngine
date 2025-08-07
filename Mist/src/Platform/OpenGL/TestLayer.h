#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "shading/ShaderController.h"

#include <imgui.h>

#include "Renderer.h"
#include "TestMenu.h"

namespace Mist {

class ShaderControler;

class TestLayer {
private:
    //GLFWwindow* m_Window;
    Renderer m_Renderer;
    //std::unique_ptr<ImGuiIO> m_IO;

    float m_LastTime = 0.0f;

    Testing::Test* m_CurrentTest;
    Testing::TestMenu* m_TestMenu;

    ShaderController m_ShaderController;

public:
    TestLayer(unsigned int width, unsigned int height);
    ~TestLayer();

    void Update(std::unique_ptr<ImGuiIO>& io);
    //bool WindowShouldClose();
};

} // namespace Mist