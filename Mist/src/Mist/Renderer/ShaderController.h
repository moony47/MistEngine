#pragma once

#include "Shader.h"
#include "Texture.h"

namespace Mist {

class ShaderController {
public:
    virtual void BindShader(uint32_t shaderID) = 0;
    virtual void UnbindShader() = 0;

    virtual bool BindTexture(uint32_t textureSlot, uint32_t textureID) = 0;
    virtual void UnbindTexture(uint32_t textureSlot) = 0;

    virtual Ref<Shader> CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) = 0;
    virtual Ref<Texture2D> CreateTexture(const std::string& texturePath) = 0;

    virtual void DeregisterShader(uint32_t shaderID) = 0;
    virtual void DeregisterTexture(uint32_t textureID) = 0;

    inline static ShaderController* GetInstance() {
        return s_Instance;
    }

private:
    static ShaderController* s_Instance;
};

} // namespace Mist