#include "Renderer.h"

#include <GLAD/glad.h>

#include "geometry/VertexArray.h"
#include "shading/Shader.h"

#include <iostream>

namespace Mist {

void ClearGLErrors() {
    while (glGetError());
}

bool LogGLCall(const char* function, const char* file, int line) {
    bool noError = true;
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        noError = false;
    }
    return noError;
}

void Renderer::Clear() {
    MS_GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, unsigned int count) const {
    shader.Bind();
    va.Bind();
    MS_GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

} // namespace Mist
