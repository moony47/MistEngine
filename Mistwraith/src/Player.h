#pragma once

#include <Mist.h>

using namespace Mist;

class Player : public Entity2D {
public:
    Player(Scene2D* scene,
           Node* parent = nullptr,
           glm::vec2 position = {0.0f, 0.0f},
           float rotation = 0.0f,
           glm::vec2 scale = {1.0f, 1.0f}) :
        Entity2D(scene, parent, position, rotation, scale) {
    }

    void OnUpdate(DeltaTime deltaTime) override {
        glm::vec2 translation(0.0f);
        if (Input::IsKeyPressed(KeyCode::W))
            translation.y += 1;
        if (Input::IsKeyPressed(KeyCode::A))
            translation.x -= 1;
        if (Input::IsKeyPressed(KeyCode::S))
            translation.y -= 1;
        if (Input::IsKeyPressed(KeyCode::D))
            translation.x += 1;

        if (translation != glm::vec2(0.0))
            translation = deltaTime * m_Speed * glm::normalize(translation);
        m_Transform.Position += translation;

        if (Input::IsKeyPressed(KeyCode::Q))
            m_Transform.Rotation += deltaTime * m_Speed;
        if (Input::IsKeyPressed(KeyCode::E))
            m_Transform.Rotation -= deltaTime * m_Speed;
    }

    // void OnEvent(Event& e) override {
    //     EventDispatcher dispatcher(e);
    // }

private:
    float m_Speed = 2.0f;
};