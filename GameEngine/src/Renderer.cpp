#include "Renderer.h"

#include <iostream>
#include <GL/glew.h>

void GLClearError() {
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line) {
    bool noError = true;
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        noError = false;
    }
    return noError;
}

void Renderer::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
