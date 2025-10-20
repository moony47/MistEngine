#include "mistpch.h"
#include "Scene.h"

#include "Mist/Renderer/RenderCommand.h";
#include "Mist/Renderer/Renderer2D.h";

namespace Mist {

Scene2D::~Scene2D() {
    for (auto it = m_Nodes.begin(); it != m_Nodes.end(); it++)
        delete it->second;
}

void Scene2D::OnUpdate(DeltaTime deltaTime) {
    m_CameraController.OnUpdate(deltaTime);
}

void Scene2D::OnRender() {
    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (auto it1 = m_Nodes.begin(); it1 != m_Nodes.end(); it1++) {
        auto sprites = it1->second->FindComponents<Sprite>();
        for (auto it2 = sprites.begin(); it2 != sprites.end(); it2++) {
            const Transform2D& transform = (*it2)->GetTransform();
            Renderer2D::DrawQuad(transform.Position, transform.Rotation, transform.Scale, (*it2)->GetTextureName());
        }
    }

    Renderer2D::EndScene();
}

void Scene2D::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);
}

void Scene2D::AddNode(const std::string& name, Node* node) {
    m_Nodes[name] = node;
    // m_Nodes[name] = node;
}

// Sprite* Scene2D::CreateSprite(Ref<Entity2D> node, const std::string& textureName) {
//     m_Sprites.push_back(std::move(Sprite(node, textureName)));
//     return &m_Sprites.back();
// }

// Ref<Sprite> Scene2D::CreateSprite(Ref<Entity2D> node, Ref<Texture2D> texture) {
//     return CreateRef<Sprite>(m_Sprites.emplace_back(node, texture));
// }

} // namespace Mist
