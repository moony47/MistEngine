#include "mistpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Mist {

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;


void Renderer::Init() {
    PROFILE_FUNCTION();

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::Shutdown() {
    PROFILE_FUNCTION();

    RenderCommand::Shutdown();
    Renderer2D::Shutdown();
}


void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    PROFILE_FUNCTION();

    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera) {
    PROFILE_FUNCTION();

    s_SceneData->VP = camera.GetVP();
}

void Renderer::EndScene() {
    PROFILE_FUNCTION();
}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
    PROFILE_FUNCTION();

    shader->Bind();
    shader->SetUniformMat4f("u_VP", s_SceneData->VP);
    shader->SetUniformMat4f("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Mist
