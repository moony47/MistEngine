#include "TestMenu.h"

#include <imgui.h>

namespace Mist::Testing {

TestMenu::TestMenu(ShaderController& shaderController,
                         Test*& currentTestPointer,
                         float winWidth,
                         float winHeight) :
    m_ShaderController(shaderController),
    m_CurrentTest(currentTestPointer),
    m_Width(winWidth),
    m_Height(winHeight) {
}

TestMenu::~TestMenu() {
}

void TestMenu::OnImGuiRender() {
    for (auto& test : m_Tests)
        if (ImGui::Button(test.first.c_str()))
            m_CurrentTest = test.second(m_ShaderController, m_Width, m_Height);
}

} // namespace Mist::Testing
