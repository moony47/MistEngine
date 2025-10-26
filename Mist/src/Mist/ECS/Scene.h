#pragma once

#include "Mist/ECS/Cameras/OrthographicCameraController.h"
#include "Mist/ECS/Components/Sprite.h"
#include <any>

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

    template <typename T>
    void AddComponent(Node* targetNode, T&& component) {
        std::vector<T>& vector = GetComponents<T>();
        vector.emplace_back(component);
        targetNode->AddComponent(&vector.back());
    }

    template <typename T>
    void HintComponentCount(size_t count) {
        std::vector<T>& vector = GetComponents<T>();
        vector.reserve(vector.size() + count);
    }

private:
    template <typename T>
    std::vector<T>& GetComponents() {
        static const std::type_index type(typeid(T));

        auto it = m_Components.find(type);
        if (it == m_Components.end()) {
            auto p = m_Components.insert(std::make_pair(type, std::make_any<std::vector<T>>()));
            return std::any_cast<std::vector<T>&>(p.second);
        }

        return std::any_cast<std::vector<T>&>(*it);
    }

private:
    std::unordered_map<std::string, Scope<Node>> m_Nodes;

    std::unordered_map<std::type_index, std::any> m_Components;

    OrthographicCameraController m_CameraController;
};

} // namespace Mist