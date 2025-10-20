#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <typeindex>

#include "Mist/Core/DeltaTime.h"
#include "Mist/Events/Event.h"
#include "Mist/ECS/Cameras/OrthographicCameraController.h"

namespace Mist {

class Node;

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
    
    void AddNode(const std::string& name, Node& node);

private:
    std::unordered_map<std::string, Node> m_Nodes;

    OrthographicCameraController m_CameraController;
};

class Component {
public:
    Component(Node* node) :
        m_Node(node) {
    }
    virtual ~Component() = default;

    Node* GetNode() {
        return m_Node;
    }

protected:
    Node* m_Node;
};

class Node {
public:
    Node(Scene2D* scene) :
        m_Scene(scene) {
    }
    Node(Scene2D* scene, Node* parent) :
        m_Scene(scene),
        m_Parent(parent) {
    }

    virtual ~Node() {
        // for (auto it1 = m_Components.begin(); it1 != m_Components.end(); it1++)
        //     for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        //         delete *it2;
    }

    inline const Node& GetParent() const {
        return *m_Parent;
    }
    inline void SetParent(Node* parent) {
        m_Parent = parent;
    }

    // template <typename T>
    //     requires std::is_base_of<Component, T>::value
    // void AddComponent(T* comp) {
    //     static const std::type_index type(typeid(T));
    //     m_Components[type].push_back(comp);
    // }

    // template <typename T>
    //     requires std::is_base_of<Component, T>::value
    // auto FindComponents() {
    //     static const std::type_index type(typeid(T));
    //     auto it = m_Components.find(type);

    //    std::vector<Component*>* baseVector;
    //    if (it == m_Components.end()) {
    //        static std::vector<Component*> empty;
    //        baseVector = &empty;
    //    } else
    //        baseVector = &it->second;

    //    return *baseVector |
    //           std::views::transform([](const Component* base) { return (T*)base; });
    //}

protected:
    Scene2D* m_Scene;
    Node* m_Parent = nullptr;
    // std::unordered_map<std::type_index, std::vector<Component*>> m_Components;
};

struct Transform2D {
    glm::vec2 Position;
    float Rotation;
    glm::vec2 Scale;
};

class Entity2D : public Node {
public:
    Entity2D(Scene2D* scene,
             Node* parent,
             glm::vec2 position = {0.0f, 0.0f},
             float rotation = 0.0f,
             glm::vec2 scale = {1.0f, 1.0f}) :
        Node(scene, parent),
        m_Transform(position, rotation, scale) {
    }

    inline Transform2D& GetTransform() {
        return m_Transform;
    }

private:
    Transform2D m_Transform;
};

} // namespace Mist