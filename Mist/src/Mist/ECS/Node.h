#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <typeindex>

#include "Component.h"
#include "Mist/Core/DeltaTime.h"
#include "Mist/Events/Event.h"

namespace Mist {

class Scene2D;

class Node {
public:
    Node(Scene2D* scene, Node* parent = nullptr) :
        m_Scene(scene),
        m_Parent(parent) {
    }

    virtual ~Node();

    inline const Node& GetParent() const {
        return *m_Parent;
    }
    inline void SetParent(Node* parent) {
        m_Parent = parent;
    }

    virtual void OnUpdate(DeltaTime deltaTime) {
    }
    virtual void OnEvent(Event& e) {
    }

    template <typename T>
        requires std::is_base_of<Component, T>::value
    void AddComponent(T* comp) {
        static const std::type_index type(typeid(T));
        m_Components[type].emplace_back(comp);
    }

    template <typename T>
        requires std::is_base_of<Component, T>::value
    auto FindComponents() {
        static const std::type_index type(typeid(T));
        auto it = m_Components.find(type);

        static std::vector<Component*> empty;
        std::vector<Component*>& baseVector = empty;
        if (it != m_Components.end())
            baseVector = it->second;

        return baseVector | std::views::transform([](const Component* base) { return (T*)base; });
    }

protected:
    Scene2D* m_Scene;
    Node* m_Parent = nullptr;
    std::unordered_map<std::type_index, std::vector<Component*>> m_Components;
};

struct Transform2D {
    glm::vec2 Position;
    float Rotation;
    glm::vec2 Scale;
};

class Entity2D : public Node {
public:
    Entity2D(Scene2D* scene,
             Node* parent = nullptr,
             glm::vec2 position = {0.0f, 0.0f},
             float rotation = 0.0f,
             glm::vec2 scale = {1.0f, 1.0f}) :
        Node(scene, parent),
        m_Transform(position, rotation, scale) {
    }

    inline Transform2D& GetTransform() {
        return m_Transform;
    }

protected:
    Transform2D m_Transform;
};

} // namespace Mist