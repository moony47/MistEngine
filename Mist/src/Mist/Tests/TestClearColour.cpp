#include "mistpch.h"

#include "TestClearColour.h"

#include "OpenGL/ShaderController.h"

#include <imgui.h>

namespace Mist::Testing {

TestClearColour::TestClearColour(float width, float height) :
    m_ClearColour{0.2f, 0.3f, 0.8f, 1.0f} {
}

void TestClearColour::OnRender(const OpenGLRenderer& renderer) {
    MIST_GLCALL(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
    MIST_GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColour::OnImGuiRender() {
    ImGui::ColorEdit4("Clear Colour", m_ClearColour);
}

} // namespace Testing
