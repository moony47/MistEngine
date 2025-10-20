#pragma once
#include "Mist/Core/Core.h"
#include "Mist/ECS/Cameras/OrthographicCameraController.h"
#include "Mist/ECS/Components/Sprite.h"
#include "Mist/ECS/Node.h"

namespace Mist {

class Scene2D : public Node {
public:
    Scene2D() :
        Node(),
        m_Nodes(),
        m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true){
    }
    ~Scene2D() override;

    void OnUpdate(DeltaTime deltaTime);
    void OnRender();

    void OnEvent(Event& e);

    void AddNode(const std::string& name, Node* node);
    //Sprite* CreateSprite(Ref<Entity2D> node, const std::string& textureName);
    //Ref<Sprite> CreateSprite(Ref<Entity2D> node, Ref<Texture2D> texture);

private:
    std::unordered_map<std::string, Node*> m_Nodes;

    OrthographicCameraController m_CameraController;
};

} // namespace Mist
