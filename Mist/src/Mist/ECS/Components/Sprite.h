#pragma once

#include "../Node.h"
#include "Mist/Renderer/Texture.h"

namespace Mist {

class Sprite {
public:
    Sprite(const Transform2D& transform, const std::string& textureName);
    Sprite(const Sprite&) = default;

    void SetTextureName(const std::string& textureName);

    inline const std::string& GetTextureName() const {
        return m_TextureName;
    }
    inline const Transform2D& GetTransform() const {
        return m_Transform;
    }

private:
    std::string m_TextureName;
    const Transform2D& m_Transform;
};

} // namespace Mist