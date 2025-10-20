#pragma once

#include "Mist/Core/Core.h"

namespace Mist {

class Node;

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

} // namespace Mist
