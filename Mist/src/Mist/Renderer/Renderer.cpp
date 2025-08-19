#include "mistpch.h"
#include "Renderer.h"

namespace Mist {

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& camera) {
    s_SceneData->VP = camera.GetVP();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,
                      const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->Bind();
    shader->SetUniformMat4f(shader->GetUniformLocation("u_VP"), s_SceneData->VP);
    shader->SetUniformMat4f(shader->GetUniformLocation("u_Transform"), transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Mist
