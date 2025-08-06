#include "ShaderController.h"

#include <iostream>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

ShaderController::ShaderController() :
    m_CurrentShaderID(0),
    m_MaxTextureSlots(8),
    m_CurrentTextureIDs(nullptr) {

    // GLCall(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_MaxTextureSlots));

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
        GLCall(glUseProgram(shaderID));
        m_CurrentShaderID = shaderID;
    }
}

void ShaderController::UnbindShader() {
    if (m_CurrentShaderID != 0) {
        GLCall(glUseProgram(0));
        m_CurrentShaderID = 0;
    }
}

bool ShaderController::BindTexture(unsigned int textureSlot, unsigned int textureID) {
    if (textureSlot >= (unsigned int) m_MaxTextureSlots) {
        std::cout << "[BindTexture] Texture Slot " << textureSlot << " not supported." << std::endl;
        return false;
    }

    if (m_CurrentTextureIDs[textureSlot] != textureID) {
        GLCall(glActiveTexture(GL_TEXTURE0 + textureSlot));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
        m_CurrentTextureIDs[textureSlot] = textureID;
    }
    return true;
}

void ShaderController::UnbindTexture(unsigned int textureSlot) {
    if (textureSlot >= (unsigned int) m_MaxTextureSlots) {
        std::cout << "[UnbindTexture] Texture Slot " << textureSlot << " not supported." << std::endl;
        return;
    }

    if (m_CurrentTextureIDs[textureSlot] != 0) {
        GLCall(glActiveTexture(GL_TEXTURE0 + textureSlot));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        m_CurrentTextureIDs[textureSlot] = 0;
    }
}

Shader* ShaderController::CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    return new Shader(*this, vertShaderPath, fragShaderPath);
}

Texture* ShaderController::CreateTexture(const std::string& texturePath) {
    return new Texture(*this, texturePath);
}