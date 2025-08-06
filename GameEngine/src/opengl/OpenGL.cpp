#include <OpenGL.h>

#include "tests/TestClearColour.h"
#include "tests/TestSprites.h"
#include "tests/TestSpritesBatch.h"

#include <GL/glew.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

OpenGL::OpenGL(unsigned int width, unsigned int height) :
    m_CurrentTest(nullptr), 
    m_TestMenu(new Testing::TestMenu(m_ShaderController, m_CurrentTest, (float)width, (float)height)) {

    // Initialize the library
    if (!glfwInit())
        throw "GLFW Init Failure";

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    m_Window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        throw "GLFW Window Init Failure";
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Init failed" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(330));
    ImGui::StyleColorsDark();

    m_IO.reset(&ImGui::GetIO());

    m_TestMenu->RegisterTest<Testing::TestClearColour>("Clear Colour");
    m_TestMenu->RegisterTest<Testing::TestSprites>("Sprites (Single)");
    m_TestMenu->RegisterTest<Testing::TestSpritesBatch>("Sprites (Batch)");

    m_CurrentTest = m_TestMenu;
}

OpenGL::~OpenGL() {
    delete m_CurrentTest;
    if (m_CurrentTest != m_TestMenu)
        delete m_TestMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void OpenGL::Update() {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;

    // Render here
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
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

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO->Framerate, m_IO->Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool OpenGL::WindowShouldClose() {
    return glfwWindowShouldClose(m_Window);
}