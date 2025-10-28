#include "mistpch.h"
#include "Entity.h"

namespace Mist {

Entity::Entity(Scene* scene, entt::entity entityID) :
    m_Scene(scene),
    m_EntityID(entityID) {};

} // namespace Mist