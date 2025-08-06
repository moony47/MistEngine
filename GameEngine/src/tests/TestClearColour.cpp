#include "TestClearColour.h"

#include "shading/ShaderController.h"

#include "imgui/imgui.h"
namespace Testing {

Testing::TestClearColour::TestClearColour(ShaderController& shaderController, float width, float height) :
    m_ClearColour{0.2f, 0.3f, 0.8f, 1.0f} {
}

void Testing::TestClearColour::OnRender(const Renderer& renderer) {
    GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Testing::TestClearColour::OnImGuiRender() {
    ImGui::ColorEdit4("Clear Colour", m_ClearColour);
}

} // namespace Testing
