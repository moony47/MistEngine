#pragma once

#include "OpenGL/OpenGLRenderer.h"
#include "Mist/Renderer/Texture.h"

#include <string>

namespace Mist {

class OpenGLTexture2D : public Texture2D{

public:
    OpenGLTexture2D(const std::string& name, const std::string& path);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) override;
    void Unbind() override;

    inline uint32_t GetWidth() const override {
        return m_Width;
    }
    inline uint32_t GetHeight() const override {
        return m_Height;
    }
    inline uint32_t GetSlot() const override {
        return m_Slot;
    }

private:
    std::string m_Name;
    std::string m_Filepath;

    unsigned char* m_LocalBuffer = nullptr;
    uint32_t m_Width = 0, m_Height = 0;

    uint32_t m_RendererID = 0;
    uint32_t m_Slot = 0;
};

} // namespace Mist