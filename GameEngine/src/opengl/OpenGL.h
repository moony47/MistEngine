#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"

#include "Renderer.h"
#include "tests/TestMenu.h"
#include "shading/ShaderController.h"

class ShaderController;

class OpenGL {
private:
    GLFWwindow* m_Window;
    Renderer m_Renderer;
    std::unique_ptr<ImGuiIO> m_IO;

    double m_LastTime = 0.0f;

    Testing::Test* m_CurrentTest; 
    Testing::TestMenu* m_TestMenu;

    ShaderController m_ShaderController;
public:
    OpenGL(unsigned int width, unsigned int height);
    ~OpenGL();

    void Update();
    bool WindowShouldClose();
};