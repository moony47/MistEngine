#pragma once

#include "../Node.h"
#include "../Component.h"
#include "Mist/Renderer/Texture.h"

namespace Mist {

class Sprite final : public Component {
public:
    Sprite(Entity2D* node, const std::string& textureName);

    void SetTextureName(const std::string& textureName);

    inline const std::string& GetTextureName() const {
        return m_TextureName;
    }
    inline const Transform2D& GetTransform() const {
        return m_Transform;
    }

private:
    std::string m_TextureName;
    Transform2D& m_Transform;
};

} // namespace Mist