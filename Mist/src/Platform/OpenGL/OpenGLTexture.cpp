#include "mistpch.h"

#include "Mist/Renderer/ShaderController.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Mist {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
    m_RendererID(0),
    m_Filepath(path),
    m_LocalBuffer(nullptr),
    m_Width(0),
    m_Height(0),
    m_Slot(0) {

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
    ShaderController::GetInstance()->DeregisterTexture(m_RendererID);
    MIST_GLCALL(glDeleteTextures(1, &m_RendererID));
}

void OpenGLTexture2D::Bind(uint32_t slot) {
    if (ShaderController::GetInstance()->BindTexture(slot, m_RendererID))
        m_Slot = slot;
}

void OpenGLTexture2D::Unbind() {
    if (m_Slot != -1) {
        ShaderController::GetInstance()->UnbindTexture(m_Slot);
        m_Slot = -1;
    }
}

} // namespace Mist