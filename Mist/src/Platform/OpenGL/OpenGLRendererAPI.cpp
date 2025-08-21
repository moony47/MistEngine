#include "mistpch.h"
#include "OpenGLRendererAPI.h"

#include "OpenGLRenderer.h"

namespace Mist {

void OpenGLRendererAPI::Init() {
    MIST_GLCALL(glEnable(GL_BLEND));
    MIST_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour) {
    MIST_GLCALL(glClearColor(colour.r, colour.g, colour.b, colour.a));
}

void OpenGLRendererAPI::Clear() {
    MIST_GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
    MIST_GLCALL(glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
}

} // namespace Mist