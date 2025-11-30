#pragma once

#include "Mist/Renderer/Texture.h"
#include "OpenGL/OpenGLRenderer.h"

#include <string>

namespace Mist {

class OpenGLTexture2D : public Texture2D {

public:
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(uint32_t width, uint32_t height);
    ~OpenGLTexture2D() override;

    const glm::vec2& GetTexCoords(size_t index) const override {
        static constexpr glm::vec2 standardCoords[4] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };
        return standardCoords[index];
    }

    void Bind(uint32_t slot) override;

    inline uint32_t GetWidth() const override {
        return m_Width;
    }
    inline uint32_t GetHeight() const override {
        return m_Height;
    }

    inline uint32_t GetRendererID() const override {
        return m_RendererID;
    }

    void SetData(void* data, uint32_t size) override;

private:
    void* m_LocalBuffer = nullptr;
    uint32_t m_Width, m_Height;

    GLenum m_InternalFormat = GL_RGBA8;
    GLenum m_DataFormat = GL_RGBA;

    std::string m_Path;

    uint32_t m_RendererID = 0;
};

} // namespace Mist