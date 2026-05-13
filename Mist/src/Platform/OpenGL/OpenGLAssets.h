#pragma once

#include "Mist/Renderer/Shader.h"
#include "Mist/Renderer/Texture.h"

namespace Mist {

class OpenGLShaderLibrary : public ShaderLibrary {
public:
    OpenGLShaderLibrary() = default;

protected:
    void Unbind_Impl() override;

    Ref<Shader> Create_Impl(const std::string& name, const std::string& shaderDirectory) override;
};

class OpenGLTexture2DLibrary : public Texture2DLibrary {
public:
    OpenGLTexture2DLibrary() = default;

protected:
    void Unbind_Impl(uint32_t slot) override;
    Ref<Texture2D> Create_Impl(const std::string& path) override;
    Ref<Texture2D> Create_Impl(uint32_t width, uint32_t height) override;
};

} // namespace Mist