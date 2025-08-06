#include "Renderer.h"

#include <GL/glew.h>

#include "geometry/VertexArray.h"
#include "geometry/IndexBuffer.h"
#include "shading/Shader.h"

#include <iostream>

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

void Renderer::Draw(const VertexArray& va, const Shader& shader, unsigned int count) const {
    shader.Bind();
    va.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}
