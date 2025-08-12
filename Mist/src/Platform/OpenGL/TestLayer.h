#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGL/ShaderController.h"

#include <imgui.h>

#include "OpenGL/Renderer.h"
#include "Mist/Tests/TestMenu.h"

namespace Mist {

class ShaderControler;

class TestLayer {
private:
    Testing::Test* m_CurrentTest;
    Testing::TestMenu* m_TestMenu;

    ShaderController m_ShaderController;

public:
    TestLayer(unsigned int width, unsigned int height);
    ~TestLayer();

    void Resize(unsigned int width, unsigned int height);

    void Update(float deltaTime, std::unique_ptr<ImGuiIO>& io, Renderer& renderer);
};

} // namespace Mist