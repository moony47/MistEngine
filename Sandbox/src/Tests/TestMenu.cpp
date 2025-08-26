#include "TestMenu.h"

TestMenu::TestMenu() {
}

TestMenu::~TestMenu() {
}

void TestMenu::OnImGuiRender(DeltaTime deltaTime) {
    ImGui::Begin("Test Layers");
    for (auto& test : m_Tests)
        if (ImGui::Button(test.first.c_str()))
            m_CurrentTest = test.second(m_CurrentTest);
    ImGui::End();
}
