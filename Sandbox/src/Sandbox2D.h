#pragma once

#include <Mist.h>
using namespace Mist;

class Sandbox2D : public Layer {
public:
    Sandbox2D();
    ~Sandbox2D() override;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(DeltaTime deltaTime) override;

    void OnFrameStart(DeltaTime deltaTime) override;
    void OnFrameEnd(DeltaTime deltaTime) override;

    void OnImGuiRender(DeltaTime deltaTime) override;

    void OnEvent(Event& e) override;

private:
    OrthographicCameraController m_CameraController;

    Ref<Shader> m_Shader;

    Ref<VertexArray> m_VertexArray;

    glm::vec4 m_SpriteColour = {0.2f, 0.0f, 0.8f, 1.0f};
};
