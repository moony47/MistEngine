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

    auto& sprites = GetComponents<Sprite>();
    for (auto it = sprites.begin(); it != sprites.end(); it++) {
        const Transform2D& transform = (*it).GetTransform();
        Renderer2D::DrawQuad(transform.Position, transform.Rotation, transform.Scale, (*it).GetTextureName());
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


} // namespace Mist
