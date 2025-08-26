#include "TestClearColour.h"

TestClearColour::TestClearColour() :
    m_ClearColour{0.1f, 0.1f, 0.1f, 1.0f} {
}

void TestClearColour::OnUpdate(DeltaTime deltaTime) {
    RenderCommand::SetClearColour(m_ClearColour);
}

void TestClearColour::OnImGuiRender(DeltaTime deltaTime) {
    ImGui::Begin("Clear Colour");
    ImGui::ColorEdit4("Clear Colour", glm::value_ptr(m_ClearColour));
    ImGui::End();
}

