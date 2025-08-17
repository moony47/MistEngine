#pragma once

#include "OpenGL/OpenGLRenderer.h"
#include "Mist/Renderer/Texture.h"
#include <string>

namespace Mist {

class OpenGLTexture2D : public Texture2D{
private:
    std::string m_Filepath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

    uint32_t m_RendererID;
    int m_Slot;

public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) override;
    void Unbind() override;

    inline int GetWidth() const override {
        return m_Width;
    }
    inline int GetHeight() const override {
        return m_Height;
    }
};

} // namespace Mist