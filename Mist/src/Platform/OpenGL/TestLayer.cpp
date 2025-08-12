#include "mistpch.h"

#include "OpenGL/TestLayer.h"
#include "Mist/Tests/TestClearColour.h"
#include "Mist/Tests/TestSprites.h"
#include "Mist/Tests/TestSpritesBatch.h"

#include <GLAD/glad.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Mist {

TestLayer::TestLayer(unsigned int width, unsigned int height) :
    m_CurrentTest(nullptr),
    m_TestMenu(new Testing::TestMenu(m_ShaderController, m_CurrentTest, (float)width, (float)height)) {

    m_TestMenu->RegisterTest<Testing::TestClearColour>("Clear Colour");
    m_TestMenu->RegisterTest<Testing::TestSprites>("Sprites (Single)");
    m_TestMenu->RegisterTest<Testing::TestSpritesBatch>("Sprites (Batch)");

    m_CurrentTest = m_TestMenu;
}

TestLayer::~TestLayer() {
    delete m_CurrentTest;
    if (m_CurrentTest != m_TestMenu)
        delete m_TestMenu;
}

void TestLayer::Resize(unsigned int width, unsigned int height) {
    m_CurrentTest->Resize(width, height);
}

void TestLayer::Update(float deltaTime, std::unique_ptr<ImGuiIO>& io, Renderer& renderer) {
    if (m_CurrentTest) {
        m_CurrentTest->OnUpdate(deltaTime);
        m_CurrentTest->OnRender(renderer);

        ImGui::Begin("Test");

        if (m_CurrentTest != m_TestMenu && ImGui::Button("Return to Menu")) {
            delete m_CurrentTest;
            m_CurrentTest = m_TestMenu;
        }
        m_CurrentTest->OnImGuiRender();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }
}

} // namespace Mist