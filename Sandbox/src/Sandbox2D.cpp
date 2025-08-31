#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
}

Sandbox2D::~Sandbox2D() {
}

void Sandbox2D::OnAttach() {
    PROFILE_FUNCTION();

    RenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    MIST_TEXTURE2DLIB->Create("Diamond", "res/textures/Diamond.png");
}

void Sandbox2D::OnDetach() {
    PROFILE_FUNCTION();

    MIST_TEXTURE2DLIB->Remove("Diamond");
}

void Sandbox2D::OnUpdate(DeltaTime deltaTime) {
    PROFILE_FUNCTION();

    m_CameraController.OnUpdate(deltaTime);
}

void Sandbox2D::OnFrameStart(DeltaTime deltaTime) {
    PROFILE_FUNCTION();

    Renderer2D::BeginScene(m_CameraController.GetCamera());
}

void Sandbox2D::OnFrameEnd(DeltaTime deltaTime) {
    PROFILE_FUNCTION();

    // Renderer2D::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
    Renderer2D::DrawQuad({0.0f, 0.0f}, 0.0f, {100.0f, 100.0f}, "Diamond");
    Renderer2D::DrawQuad({-0.5f, -0.5f}, 30.0f, {0.5f, 0.5f}, m_SpriteColour, "Diamond");
    Renderer2D::DrawQuad({0.5f, 0.5f}, 0.0f, {1.5f, 0.5f}, glm::vec4{1.0f, 1.0f, 1.0f, 2.0f} - m_SpriteColour);
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender(DeltaTime deltaTime) {
    PROFILE_FUNCTION();

    ImGui::Begin("Sandbox2D");
    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour), ImGuiColorEditFlags_Float);

    ImGui::End();
}

void Sandbox2D::OnEvent(Event& e) {
    PROFILE_FUNCTION();

    m_CameraController.OnEvent(e);
}
