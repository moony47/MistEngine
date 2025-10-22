#pragma once

#include "Mist/ECS/Cameras/OrthographicCameraController.h"
#include "Mist/ECS/Components/Sprite.h"

namespace Mist {

class Node;
class Entity2D;

class Scene2D {
public:
    Scene2D() :
        m_Nodes(),
        m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
    }
    ~Scene2D();

    void OnUpdate(DeltaTime deltaTime);
    void OnRender();

    void OnEvent(Event& e);

    void AddNode(const std::string& name, Node* node);

    void AddSpriteToEntity(Entity2D* node, const std::string& textureName);

    void HintSpriteCount(size_t count);

private:
    std::unordered_map<std::string, Scope<Node>> m_Nodes;

    std::vector<Sprite> m_Sprites;
    OrthographicCameraController m_CameraController;
};

} // namespace Mist