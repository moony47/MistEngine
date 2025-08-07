#include "mistpch.h"

#include <TestLayer.h>

#include "TestClearColour.h"
#include "TestSprites.h"
#include "TestSpritesBatch.h"

#include <GLAD/glad.h>

#include "ImGuiOpenGLRenderer.h"
#include "ImGuiGLFWRenderer.h"

namespace Mist {

TestLayer::TestLayer(unsigned int width, unsigned int height) :
    m_CurrentTest(nullptr),
    m_TestMenu(new Testing::TestMenu(m_ShaderController, m_CurrentTest, (float)width, (float)height)) {

    //// Initialize the library
    //if (!glfwInit())
    //    throw "GLFW Init Failure";

    //// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //// Create a windowed mode window and its OpenGL context
    //m_Window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
    //if (!m_Window) {
    //    glfwTerminate();
    //    throw "GLFW Window Init Failure";
    //}

    //// Make the window's context current
    //glfwMakeContextCurrent(m_Window);

    //int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    //glfwSwapInterval(1);

    //std::cout << glGetString(GL_VERSION) << std::endl;

    //MS_GLCALL(glEnable(GL_BLEND));
    //MS_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    //ImGui_ImplOpenGL3_Init("#version 460");
    //ImGui::StyleColorsDark();

    //m_IO.reset(&ImGui::GetIO());
    //m_IO->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    //m_IO->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    m_LastTime = (float)glfwGetTime();

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

void TestLayer::Update(std::unique_ptr<ImGuiIO>& io) {
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;

    // Render here
    MS_GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    m_Renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_CurrentTest) {
        m_CurrentTest->OnUpdate(deltaTime);
        m_CurrentTest->OnRender(m_Renderer);

        ImGui::Begin("Test");

        if (m_CurrentTest != m_TestMenu && ImGui::Button("Return to Menu")) {
            delete m_CurrentTest;
            m_CurrentTest = m_TestMenu;
        }
        m_CurrentTest->OnImGuiRender();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Mist