#include "TestMenu.h"

#include "imgui/imgui.h"

test::TestMenu::TestMenu(Test*& currentTestPointer, float winWidth, float winHeight)
	: m_CurrentTest(currentTestPointer), m_Width (winWidth), m_Height (winHeight) {
}

test::TestMenu::~TestMenu() {

}

void test::TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests) 
		if (ImGui::Button(test.first.c_str())) 
			m_CurrentTest = test.second(m_Width, m_Height);
}
