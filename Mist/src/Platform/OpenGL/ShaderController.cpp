#include "mistpch.h"

#include "ShaderController.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"

namespace Mist {

ShaderController::ShaderController() :
    m_CurrentShaderID(0),
    m_MaxTextureSlots(8),
    m_CurrentTextureIDs(nullptr) {

    // MS_GLCALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_MaxTextureSlots));

    m_CurrentTextureIDs = new unsigned int[m_MaxTextureSlots];
    for (int slot = 0; slot < m_MaxTextureSlots; slot++)
        m_CurrentTextureIDs[slot] = 0;
}

ShaderController::~ShaderController() {
    delete[] m_CurrentTextureIDs;
    UnbindShader();
    for (int slot = 0; slot < m_MaxTextureSlots; slot++)
        UnbindTexture(slot);
}

void ShaderController::BindShader(unsigned int shaderID) {
    if (m_CurrentShaderID != shaderID) {
        MS_GLCALL(glUseProgram(shaderID));
        m_CurrentShaderID = shaderID;
    }
}

void ShaderController::UnbindShader() {
    if (m_CurrentShaderID != 0) {
        MS_GLCALL(glUseProgram(0));
        m_CurrentShaderID = 0;
    }
}

bool ShaderController::BindTexture(unsigned int textureSlot, unsigned int textureID) {
    if (textureSlot >= (unsigned int)m_MaxTextureSlots) {
        MIST_CORE_ERROR("[BindTexture] Texture Slot {0} not supported.", textureSlot);
        return false;
    }

    if (m_CurrentTextureIDs[textureSlot] != textureID) {
        MS_GLCALL(glActiveTexture(GL_TEXTURE0 + textureSlot));
        MS_GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
        m_CurrentTextureIDs[textureSlot] = textureID;
    }
    return true;
}

void ShaderController::UnbindTexture(unsigned int textureSlot) {
    if (textureSlot >= (unsigned int)m_MaxTextureSlots) {
        MIST_CORE_ERROR("[UnbindTexture] Texture Slot {0} not supported.", textureSlot);
        return;
    }

    if (m_CurrentTextureIDs[textureSlot] != 0) {
        MS_GLCALL(glActiveTexture(GL_TEXTURE0 + textureSlot));
        MS_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        m_CurrentTextureIDs[textureSlot] = 0;
    }
}

Shader* ShaderController::CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    return new Shader(*this, vertShaderPath, fragShaderPath);
}

Texture* ShaderController::CreateTexture(const std::string& texturePath) {
    return new Texture(*this, texturePath);
}

void ShaderController::DeregisterShader(unsigned int shaderID) {
    if (m_CurrentShaderID == shaderID)
        UnbindShader();
}

void ShaderController::DeregisterTexture(unsigned int textureID) {
    for (int textureSlot = 0; textureSlot < m_MaxTextureSlots; textureSlot++)
        if (m_CurrentTextureIDs[textureSlot] == textureID)
            UnbindTexture(textureSlot);
}

} // namespace Mist