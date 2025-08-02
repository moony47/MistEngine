#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColour.h"
#include "tests/TestSprites.h"

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    int windowWidth = 1600, windowHeight = 900;
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

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

    {

        Renderer renderer;
        glm::mat4 proj = (glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight));

        ImGuiIO& io = ImGui::GetIO();

        test::Test* test = nullptr;

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (test != nullptr) {
                test->OnUpdate(1.0f);
                test->OnRender(renderer, proj);  
                if (test->OnImGuiRender())
                    test = nullptr;
            } else {
                ImGui::Begin("Select a Test");

                if (ImGui::Button("Clear Colour"))
                    test = new test::TestClearColour();
                else if (ImGui::Button("Sprites"))
                    test = new test::TestSprites();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            //{
            //    model = glm::translate(glm::mat4(1.0f), translationA);
            //    mvp = proj * view * model;

            //    shader.Bind();
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    shader.SetUniform4f("u_Colour", r, 0.3f, 0.8f, 1.0f);

            //    renderer.Draw(va, ib, shader);
            //}

            //{
            //    model = glm::translate(glm::mat4(1.0f), translationB);
            //    mvp = proj * view * model;

            //    shader.Bind();
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    shader.SetUniform4f("u_Colour", r, 0.3f, 0.8f, 1.0f);

            //    renderer.Draw(va, ib, shader);
            //}

            //// Animate the colour
            //if (r < 0.0f || r > 1.0f)
            //    rIncrement = -rIncrement;
            //r += rIncrement;

            //{
            //    ImGui::Begin("Object Controls");

            //    ImGui::SliderFloat("Translation1X", &translationA.x, 50.0f, windowWidth - 50.0f);
            //    ImGui::SliderFloat("Translation1Y", &translationA.y, 50.0f, windowHeight - 50.0f);

            //    ImGui::SliderFloat("Translation2X", &translationB.x, 50.0f, windowWidth - 50.0f);
            //    ImGui::SliderFloat("Translation2Y", &translationB.y, 50.0f, windowHeight - 50.0f);
            //    
            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //    ImGui::End();
            //}



            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}