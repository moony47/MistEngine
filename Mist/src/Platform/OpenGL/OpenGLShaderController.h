#pragma once

#include "Mist/Renderer/ShaderController.h"

namespace Mist {

class OpenGLShaderController : public ShaderController {
private:
    int m_MaxTextureSlots;
    uint32_t* m_CurrentTextureIDs;
    uint32_t m_CurrentShaderID;

public:
    OpenGLShaderController();

    void BindShader(uint32_t shaderID) override;
    void UnbindShader() override;

    bool BindTexture(uint32_t textureSlot, uint32_t textureID) override;
    void UnbindTexture(uint32_t textureSlot) override;

    Ref<Shader> CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) override;
    Ref<Texture2D> CreateTexture(const std::string& texturePath) override;

    void DeregisterShader(uint32_t shaderID) override;
    void DeregisterTexture(uint32_t textureID) override;
};

} // namespace Mist