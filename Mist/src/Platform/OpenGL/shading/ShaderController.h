#pragma once

#include <string>

namespace Mist {

class Shader;
class Texture;

class ShaderController {
private:
    int m_MaxTextureSlots;
    unsigned int* m_CurrentTextureIDs;
    unsigned int m_CurrentShaderID;

public:
    ShaderController();
    ~ShaderController();

    void BindShader(unsigned int shaderID);
    void UnbindShader();

    bool BindTexture(unsigned int textureSlot, unsigned int textureID);
    void UnbindTexture(unsigned int textureSlot);

    Shader* CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath);
    Texture* CreateTexture(const std::string& texturePath);
};

} // namespace Mist