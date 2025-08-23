#include "TestClearColour.h"

TestClearColour::TestClearColour() :
    m_ClearColour{0.2f, 0.3f, 0.8f, 1.0f} {
}

void TestClearColour::OnUpdate(DeltaTime deltaTime) {
    RenderCommand::SetClearColour(m_ClearColour);
}

void TestClearColour::OnImGuiRender(DeltaTime deltaTime) {
    ImGui::ColorEdit4("Clear Colour", glm::value_ptr(m_ClearColour));
}

