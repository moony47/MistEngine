#include "mistpch.h"

#include "OpenGLRenderer.h"
#include "OpenGLRendererAPI.h"

namespace Mist {

void OpenGLRendererAPI::Init() {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glEnable(GL_BLEND));
    MIST_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    MIST_GLCALL(glViewport(x, y, width, height));
}

void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour) {
    MIST_GLCALL(glClearColor(colour.r, colour.g, colour.b, colour.a));
}

void OpenGLRendererAPI::Clear() {
    MIST_GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    vertexArray->Bind();
    MIST_GLCALL(glDrawElements(GL_TRIANGLES, indexCount ? indexCount : (uint32_t)vertexArray->GetIndexBuffer()->GetCount(),
                               GL_UNSIGNED_INT, nullptr));
}

} // namespace Mist