#include "mistpch.h"

#include "OpenGLRenderer.h"

#include <GLAD/glad.h>

#include "OpenGLVertexArray.h"
#include "OpenGLShader.h"

namespace Mist {

void ClearGLErrors() {
    while (glGetError());
}

bool LogGLCall(const char* function, const char* file, int line) {
    bool noError = true;
    while (GLenum error = glGetError()) {
        MIST_CORE_ERROR("[OpenGL Error] ({0}): {1} {2}:{3}", error, function, file, line);
        noError = false;
    }
    return noError;
}

void OpenGLRenderer::Clear() {
    MIST_GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void OpenGLRenderer::Draw(const VertexArray& va, const Shader& shader, unsigned int count) const {
    shader.Bind();
    va.Bind();
    MIST_GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

} // namespace Mist
