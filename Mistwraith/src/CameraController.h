#pragma once

#include <Mist.h>
//using namespace Mist;
namespace Mist {

class CameraController : public ScriptableEntity {
public:
    void OnCreate() override {};

    void OnDestroy() override {};

    void OnUpdate(DeltaTime deltaTime) override {
        // WASD Camera Controls
        glm::vec3 cameraStep(0.0f);
        if (Input::IsKeyPressed(KeyCode::W))
            cameraStep.y += 1;
        if (Input::IsKeyPressed(KeyCode::A))
            cameraStep.x -= 1;
        if (Input::IsKeyPressed(KeyCode::S))
            cameraStep.y -= 1;
        if (Input::IsKeyPressed(KeyCode::D))
            cameraStep.x += 1;
        if (cameraStep != glm::vec3{0.0f, 0.0f, 0.0f})
            cameraStep = m_PanSpeed * deltaTime * glm::normalize(cameraStep);
        GetComponent<TransformComponent>().TranslateBy(cameraStep);
    }

    void OnEvent(Event& e) override {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(MIST_BIND_EVENT_FN(CameraController::OnMouseScrolled));
    }

private:
    bool OnMouseScrolled(MouseScrolledEvent& e) {
        auto& camera = GetComponent<CameraComponent>().Camera;
        camera.SetOrthographicSize(
            glm::clamp((1 - e.GetYOffset() * m_ScrollSpeed) * camera.GetOrthographicSize(), 0.5f, 100.0f));
        return true;
    }

private:
    TransformComponent* m_Transform = nullptr;

    const float m_PanSpeed = 5.0f, m_ScrollSpeed = 0.25f;
};

} // namespace Mist