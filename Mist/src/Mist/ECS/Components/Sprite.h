#pragma once
#include "../Component.h"
#include "../Node.h"
#include "Mist/Renderer/Texture.h"

namespace Mist {

class Sprite : public Component {
public:
    Sprite(Entity2D* node, std::string textureName);
    //Sprite(Ref<Entity2D> node, Ref<Texture2D> texture);

    //void SetTexture(Ref<Texture2D> texture);
    void SetTextureName(std::string textureName);

    inline const std::string& GetTextureName() const {
        return m_TextureName;
    }
    inline const Transform2D& GetTransform() const {
        return m_Transform;
    }

private:
    std::string m_TextureName;
    Transform2D m_Transform;
};

} // namespace Mist