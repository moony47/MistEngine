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

    void OnFrameStart(DeltaTime deltaTime) override;
    void OnFrameEnd(DeltaTime deltaTime) override;

    void OnImGuiRender(DeltaTime deltaTime) override;

    void OnEvent(Event& e) override;

private:
    OrthographicCameraController m_CameraController;

    Ref<VertexArray> m_VertexArray;
    Ref<Shader> m_Shader;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize{0.0f, 0.0f};

    glm::vec4 m_SpriteColour{0.2f, 0.0f, 0.8f, 1.0f};
};
