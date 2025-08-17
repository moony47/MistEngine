#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "Mist/Renderer/ShaderController.h"
#include "OpenGL/OpenGLRenderer.h"
#include "Mist/Tests/TestMenu.h"

namespace Mist {

class ShaderController;

class TestLayer {
private:
    Testing::Test* m_CurrentTest;
    Testing::TestMenu* m_TestMenu;

public:
    TestLayer(unsigned int width, unsigned int height);
    ~TestLayer();

    void Resize(unsigned int width, unsigned int height);

    void Update(float deltaTime, ImGuiIO& io, OpenGLRenderer& renderer);
};

} // namespace Mist