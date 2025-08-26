#include "Sandbox2D.h"

void Sandbox2D::OnAttach() {
    RenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    MIST_TEXTURE2DLIB->Create("Diamond", "res/textures/Diamond.png");
}

void Sandbox2D::OnDetach() {
    MIST_TEXTURE2DLIB->Remove("Diamond");
}

void Sandbox2D::OnUpdate(DeltaTime deltaTime) {
    m_CameraController.OnUpdate(deltaTime);
}

void Sandbox2D::OnFrameStart(DeltaTime deltaTime) {
    Renderer2D::BeginScene(m_CameraController.GetCamera());
}

void Sandbox2D::OnFrameEnd(DeltaTime deltaTime) {

    // Renderer2D::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
    Renderer2D::DrawQuad({0.0f, 0.0f}, 0.0f, {100.0f, 100.0f}, "Diamond");
    Renderer2D::DrawQuad({-0.5f, -0.5f}, 30.0f, {0.5f, 0.5f}, m_SpriteColour, "Diamond");
    Renderer2D::DrawQuad({0.5f, 0.5f}, 0.0f, {1.5f, 0.5f}, glm::vec4{1.0f, 1.0f, 1.0f, 2.0f} - m_SpriteColour);
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender(DeltaTime deltaTime) {
    ImGui::Begin("Sandbox2D");
    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour),
                                                ImGuiColorEditFlags_Float);
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);
}
