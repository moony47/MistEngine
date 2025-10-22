#include "mistpch.h"
#include "Sprite.h"


namespace Mist {

Sprite::Sprite(Entity2D* node, const std::string& textureName) :
    Component(node),  
    m_TextureName(textureName),
    m_Transform(node->GetTransform()) {
}

void Sprite::SetTextureName(const std::string& textureName) {
    m_TextureName = textureName;
}

} // namespace Mist