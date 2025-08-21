#include "mistpch.h"

#include "OpenGLShaderController.h"

#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

namespace Mist {

OpenGLShaderController::OpenGLShaderController() :
    m_CurrentShaderID(0),
    m_MaxTextureSlots(8),
    m_CurrentTextureIDs(nullptr) {

    // MS_GLCALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_MaxTextureSlots));

    m_CurrentTextureIDs = new unsigned int[m_MaxTextureSlots];
    for (int slot = 0; slot < m_MaxTextureSlots; slot++)
        m_CurrentTextureIDs[slot] = 0;
}

void OpenGLShaderController::BindShader(uint32_t shaderID) {
    if (m_CurrentShaderID != shaderID) {
        MIST_GLCALL(glUseProgram(shaderID));
        m_CurrentShaderID = shaderID;
    }
}

void OpenGLShaderController::UnbindShader() {
    if (m_CurrentShaderID != 0) {
        MIST_GLCALL(glUseProgram(0));
        m_CurrentShaderID = 0;
    }
}

bool OpenGLShaderController::BindTexture(uint32_t textureSlot, uint32_t textureID) {
    if (textureSlot >= (uint32_t)m_MaxTextureSlots) {
        MIST_CORE_ERROR("[BindTexture] Texture Slot {0} not supported.", textureSlot);
        return false;
    }

    if (m_CurrentTextureIDs[textureSlot] != textureID) {
        MIST_GLCALL(glActiveTexture(GL_TEXTURE0 + textureSlot));
        MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
        m_CurrentTextureIDs[textureSlot] = textureID;
    }
    return true;
}

void OpenGLShaderController::UnbindTexture(uint32_t textureSlot) {
    if (textureSlot >= (uint32_t)m_MaxTextureSlots) {
        MIST_CORE_ERROR("[UnbindTexture] Texture Slot {0} not supported.", textureSlot);
        return;
    }

    if (m_CurrentTextureIDs[textureSlot] != 0) {
        MIST_GLCALL(glActiveTexture(GL_TEXTURE0 + textureSlot));
        MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        m_CurrentTextureIDs[textureSlot] = 0;
    }
}

Ref<Shader> OpenGLShaderController::CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    return std::make_shared<OpenGLShader>(vertShaderPath, fragShaderPath);
}

Ref<Texture2D> OpenGLShaderController::CreateTexture(const std::string& texturePath) {
    return std::make_shared<OpenGLTexture2D>(texturePath);
}

void OpenGLShaderController::DeregisterShader(uint32_t shaderID) {
    if (m_CurrentShaderID == shaderID)
        UnbindShader();
}

void OpenGLShaderController::DeregisterTexture(uint32_t textureID) {
    for (int textureSlot = 0; textureSlot < m_MaxTextureSlots; textureSlot++)
        if (m_CurrentTextureIDs[textureSlot] == textureID)
            UnbindTexture(textureSlot);
}

} // namespace Mist