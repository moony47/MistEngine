#include "mistpch.h"
#include "Node.h"

#include "Scene.h"

namespace Mist {

Node::~Node() {
    for (auto it1 = m_Components.begin(); it1 != m_Components.end(); it1++)
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
            delete *it2;
}

} // namespace Mist