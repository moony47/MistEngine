#pragma once

#include "Renderer.h"

namespace Mist {

class ShaderController;

class Texture {
private:
    std::string m_Filepath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

    unsigned int m_RendererID;
    int m_Slot;

    ShaderController& m_ShaderController;

public:
    Texture(ShaderController& shaderController, const std::string& path);
    ~Texture();

    void Bind(unsigned int slot);
    void Unbind();

    inline int GetWidth() const {
        return m_Width;
    }
    inline int GetHeight() const {
        return m_Height;
    }
};

} // namespace Mist