#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"

#include "tests/TestClearColour.h"
#include "tests/TestSprites.h"
#include "tests/TestSpritesBatch.h"
#include "tests/TestMenu.h"

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    int windowWidth = 1600, windowHeight = 900;
    window = glfwCreateWindow(windowWidth, windowHeight, "Test Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    //glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Init failed" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(330));
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();

    {
        test::Test* currentTest = nullptr;
        test::TestMenu* menu = new test::TestMenu(currentTest, (float)windowWidth, (float)windowHeight);
        currentTest = menu;

        menu->RegisterTest<test::TestClearColour>("Clear Colour");
        menu->RegisterTest<test::TestSprites>("Sprites (Single)");
        menu->RegisterTest<test::TestSpritesBatch>("Sprites (Batch)");

        Renderer renderer;

        double lastTime = 0.0f;

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            // Render here
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) {
                currentTest->OnUpdate(deltaTime);
                currentTest->OnRender(renderer);

                ImGui::Begin("Test");

                if (currentTest != menu && ImGui::Button("Return to Menu")) {
                    delete currentTest;
                    currentTest = menu;
                }
                currentTest->OnImGuiRender();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != menu)
            delete menu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}