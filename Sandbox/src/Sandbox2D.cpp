#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
}

Sandbox2D::~Sandbox2D() {
}

void Sandbox2D::OnAttach() {
    MIST_PROFILE_FUNCTION();

    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    MIST_TEXLIB->Create("Diamond", "res/textures/diamond.png");
    MIST_TEXLIB->Create("Star", "res/textures/star.png");

    MIST_TEXLIB->Create("SpriteSheet", "res/textures/RPGpack_sheet_2X.png");
    MIST_TEXLIB->CreateSub("GrassN", "SpriteSheet", {1, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassNE", "SpriteSheet", {2, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassE", "SpriteSheet", {2, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassSE", "SpriteSheet", {2, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassS", "SpriteSheet", {1, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassSW", "SpriteSheet", {0, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassW", "SpriteSheet", {0, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("GrassNW", "SpriteSheet", {0, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("Grass", "SpriteSheet", {1, 11}, {128, 128});

    MIST_TEXLIB->CreateSub("StairE", "SpriteSheet", {7, 6}, {128, 128});
    MIST_TEXLIB->CreateSub("StairW", "SpriteSheet", {8, 6}, {128, 128});
    MIST_TEXLIB->CreateSub("Tree", "SpriteSheet", {3, 1}, {128, 128}, {1, 2});
}

void Sandbox2D::OnDetach() {
    MIST_PROFILE_FUNCTION();

    MIST_TEXLIB->Remove("Diamond");
    MIST_TEXLIB->Remove("Star");
    MIST_TEXLIB->Remove("SpriteSheet", true);
}

void Sandbox2D::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(deltaTime);
}

void Sandbox2D::OnFrameStart(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Mist::Renderer2D::ResetStats();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
}

void Sandbox2D::OnFrameEnd(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    //Renderer2D::DrawQuad({0.0f, 0.0f, 1.0f}, glm::radians(0.0f), {10.0f, 10.0f}, glm::vec4{0.4f, 0.4f, 0.4f, 1.0f});

    Renderer2D::DrawQuad({0.0f, 1.0f}, glm::radians(90.0f), {1.0f, 1.0f}, glm::vec4{0.3f, 1.0f, 0.6f, 1.0f}, "Diamond");
    Renderer2D::DrawQuad({1.0f, 1.5f}, glm::radians(0.0f), {1.0f, 2.0f}, m_SpriteColour);
    Renderer2D::DrawQuad({0.0f, 0.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "StairE");
    Renderer2D::DrawQuad({1.0f, 0.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "StairW");

    Renderer2D::DrawQuad({3.0f, 1.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "Grass");
    Renderer2D::DrawQuad({3.0f, 2.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassN");
    Renderer2D::DrawQuad({4.0f, 2.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassNE");
    Renderer2D::DrawQuad({4.0f, 1.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassE");
    Renderer2D::DrawQuad({4.0f, 0.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassSE");
    Renderer2D::DrawQuad({3.0f, 0.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassS");
    Renderer2D::DrawQuad({2.0f, 0.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassSW");
    Renderer2D::DrawQuad({2.0f, 1.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassW");
    Renderer2D::DrawQuad({2.0f, 2.0f}, glm::radians(0.0f), {1.0f, 1.0f}, "GrassNW");

    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    ImGui::Begin("Sandbox2D");
    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour), ImGuiColorEditFlags_Float);
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnEvent(e);
}
