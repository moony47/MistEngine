#include "Texture.h"

#include "stb_image/stb_image.h"
#include "ShaderController.h"


namespace Mist {

Texture::Texture(ShaderController& shaderController, const std::string& path) :
    m_RendererID(0),
    m_Filepath(path),
    m_LocalBuffer(nullptr),
    m_Width(0),
    m_Height(0),
    m_BPP(0),
    m_Slot(0),
    m_ShaderController(shaderController) {

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    MS_GLCALL(glGenTextures(1, &m_RendererID));
    MS_GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    MS_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    MS_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    MS_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    MS_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    MS_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    MS_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
    delete[] m_LocalBuffer;
    MS_GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) {
    if (m_ShaderController.BindTexture(slot, m_RendererID))
        m_Slot = slot;
}

void Texture::Unbind() {
    if (m_Slot != -1) {
        m_ShaderController.UnbindTexture((unsigned int)m_Slot);
        m_Slot = -1;
    }
}

} // namespace Mist