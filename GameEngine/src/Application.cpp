#include "Application.h"
#include "OpenGL.h"

namespace GameEngine {

OpenGL* opengl;

Application::Application() {
    opengl = new OpenGL(1600, 900);
}

Application::~Application() {
    delete opengl;
}

void Application::Run() {
    // while (true);
    //  Loop until the user closes the window
    while (!opengl->WindowShouldClose())
        opengl->Update();
}

} // namespace GameEngine