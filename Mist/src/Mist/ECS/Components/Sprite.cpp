#include "mistpch.h"
#include "Sprite.h"


namespace Mist {

Sprite::Sprite(const Transform2D& transform, const std::string& textureName) :
    m_TextureName(textureName),
    m_Transform(transform) {
}

void Sprite::SetTextureName(const std::string& textureName) {
    m_TextureName = textureName;
}

} // namespace Mist