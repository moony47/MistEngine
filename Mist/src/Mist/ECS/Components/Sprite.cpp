#include "mistpch.h"
#include "Sprite.h"

namespace Mist {

Sprite::Sprite(Entity2D* node, std::string textureName) :
    Component(node),  
    m_TextureName(textureName),
    m_Transform(node->GetTransform()) {
}
//Sprite::Sprite(Ref<Entity2D> node, Ref<Texture2D> texture) :
//    Component(node),
//    m_Texture(texture),
//    m_Transform(node->GetTransform()) {
//}

//void Sprite::SetTexture(Ref<Texture2D> texture) {
//    m_Texture = texture;
//}
void Sprite::SetTextureName(std::string textureName) {
    m_TextureName = textureName;
}
} // namespace Mist