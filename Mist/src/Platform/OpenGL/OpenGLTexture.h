#pragma once

#include "OpenGL/OpenGLRenderer.h"
#include "Mist/Renderer/Texture.h"

#include <string>

namespace Mist {

class OpenGLTexture2D : public Texture2D{

public:
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(uint32_t width, uint32_t height);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) override;

    inline uint32_t GetWidth() const override {
        return m_Width;
    }
    inline uint32_t GetHeight() const override {
        return m_Height;
    }

    void SetData(void* data, uint32_t size) override;

private:
    std::string m_Filepath;

    void* m_LocalBuffer = nullptr;
    uint32_t m_Width = 0, m_Height = 0;

    uint32_t m_RendererID = 0;

    GLenum m_InternalFormat = GL_RGBA8;
    GLenum m_DataFormat = GL_RGBA;
};

} // namespace Mist