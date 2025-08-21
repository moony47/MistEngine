#pragma once

#include "OpenGL/OpenGLRenderer.h"
#include "Mist/Renderer/Texture.h"
#include <string>

namespace Mist {

class OpenGLTexture2D : public Texture2D{
private:
    std::string m_Filepath;
    unsigned char* m_LocalBuffer;
    uint32_t m_Width, m_Height;

    uint32_t m_RendererID;
    uint32_t m_Slot;

public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) override;
    void Unbind() override;

    inline uint32_t GetWidth() const override {
        return m_Width;
    }
    inline uint32_t GetHeight() const override {
        return m_Height;
    }
};

} // namespace Mist