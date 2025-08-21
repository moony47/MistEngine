#include "mistpch.h"

#include "Mist/Renderer/Shader.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Mist {

OpenGLTexture2D::OpenGLTexture2D(const std::string& name,  const std::string& path) :
    m_Name(name),
    m_Filepath(path) {

    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    m_LocalBuffer = stbi_load(path.c_str(), &width, &height, &channels, 0);

    MIST_CORE_ASSERT(m_LocalBuffer, "[Texture2D] Failed to load image.");

    m_Width = width;
    m_Height = height;

    // Identify the pixel type flags from the number of channels
    GLenum internalFormat, dataFormat;
    switch (channels) {
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        default:
            MIST_ERROR("[Texture2D] Failed to identify pixel type from num. image channels.");
    }

    MIST_GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
    MIST_GLCALL(glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height));

    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    MIST_GLCALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
}

OpenGLTexture2D::~OpenGLTexture2D() {
    stbi_image_free(m_LocalBuffer);
    MIST_GLCALL(glDeleteTextures(1, &m_RendererID));
}

void OpenGLTexture2D::Bind(uint32_t slot) {
    m_Slot = slot;
    MIST_GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void OpenGLTexture2D::Unbind() {
    MIST_GLCALL(glActiveTexture(GL_TEXTURE0 + m_Slot));
    MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    m_Slot = 0xFFFFFFFF;
}

} // namespace Mist