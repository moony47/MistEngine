#include "mistpch.h"
#include "Scene.h"

#include "Mist/ECS/Node.h"
#include "Mist/Renderer/RenderCommand.h"
#include "Mist/Renderer/Renderer2D.h"

namespace Mist {

Scene2D::~Scene2D() {
}

void Scene2D::OnUpdate(DeltaTime deltaTime) {
    m_CameraController.OnUpdate(deltaTime);

    for (auto it = m_Nodes.begin(); it != m_Nodes.end(); it++)
        it->second->OnUpdate(deltaTime);
}

void Scene2D::OnRender() {
    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (auto it = m_Sprites.begin(); it != m_Sprites.end(); it++) {
        const Transform2D& transform = it->GetTransform();
        Renderer2D::DrawQuad(transform.Position, transform.Rotation, transform.Scale, it->GetTextureName());
    }

    Renderer2D::EndScene();
}

void Scene2D::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);

    for (auto it = m_Nodes.begin(); it != m_Nodes.end(); it++)
        it->second->OnEvent(e);
}

void Scene2D::AddNode(const std::string& name, Node* node) {
    m_Nodes[name] = Scope<Node>(node);
}

void Scene2D::AddSpriteToEntity(Entity2D* node, const std::string& textureName) {
    m_Sprites.emplace_back(node, textureName);
    node->AddComponent(&m_Sprites.back());
}

void Scene2D::HintSpriteCount(size_t count) {
    m_Sprites.reserve(m_Sprites.size() + count);
}

} // namespace Mist
