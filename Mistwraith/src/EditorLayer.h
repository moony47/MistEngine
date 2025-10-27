#pragma once

#include <Mist.h>

using namespace Mist;

class EditorLayer : public Layer {
public:
    EditorLayer();
    ~EditorLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(DeltaTime deltaTime) override;
    void OnRender(DeltaTime deltaTime) override;
    void OnImGuiRender(DeltaTime deltaTime) override;

    void OnEvent(Event& e) override;

private:
    Ref<Scene> m_ActiveScene;
    Entity m_CameraEntity;
    Entity m_SpriteEntity;

    OrthographicCameraController m_CameraController;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize{0.0f, 0.0f};
    bool m_ViewportFocussed = false, m_ViewportHovered = false;
};