#include "mistpch.h"

#include "OpenGLTexture.h"

#include "Mist/Renderer/Shader.h"

#include "stb_image.h"

namespace Mist {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
    m_Path(path) {
    MIST_PROFILE_FUNCTION();

    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    {
        MIST_PROFILE_SCOPE("stbi_load");
        m_LocalBuffer = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    MIST_CORE_ASSERT(m_LocalBuffer, "[Texture2D] Failed to load image.");

    m_Width = width;
    m_Height = height;

    // Identify the pixel type flags from the number of channels
    switch (channels) {
        case 3:
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
            break;
        case 4:
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
            break;
        default:
            MIST_ERROR("[Texture2D] Failed to identify pixel type from channels.");
    }

    MIST_GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
    MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    MIST_GLCALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    MIST_GLCALL(
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
    m_Path(""),
    m_Width(width),
    m_Height(height),
    m_InternalFormat(GL_RGBA8),
    m_DataFormat(GL_RGBA) {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
    MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    MIST_GLCALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

OpenGLTexture2D::~OpenGLTexture2D() {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glDeleteTextures(1, &m_RendererID));
    stbi_image_free(m_LocalBuffer);
}

void OpenGLTexture2D::Bind(uint32_t slot) {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glBindTextureUnit(slot, m_RendererID));
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    MIST_PROFILE_FUNCTION();

    uint32_t channels = m_DataFormat == GL_RGBA ? 4 : 3;
    MIST_CORE_ASSERT(size == m_Width * m_Height * channels,
                     "[OpenGLTexture2D::SetData] Data must be the entire texture");
    m_LocalBuffer = data;
    MIST_GLCALL(
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
}

} // namespace Mist