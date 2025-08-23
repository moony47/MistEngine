#include "TestMenu.h"

TestMenu::TestMenu() {
}

TestMenu::~TestMenu() {
}

void TestMenu::OnImGuiRender(DeltaTime deltaTime) {
    for (auto& test : m_Tests)
        if (ImGui::Button(test.first.c_str()))
            test.second(m_CurrentTest);
}
