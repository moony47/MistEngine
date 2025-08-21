#pragma once

#include "Mist/Renderer/Shader.h"
#include "Mist/Renderer/Texture.h"

namespace Mist {

class OpenGLShaderLibrary : public ShaderLibrary {
public:
    OpenGLShaderLibrary() = default;

protected:
    void Unbind_Impl() override;

    Ref<Shader> Create_Impl(const std::string& name,
                            const std::string& vertShaderPath,
                            const std::string& fragShaderPath) override;
};

class OpenGLTexture2DLibrary : public Texture2DLibrary {
public:
    OpenGLTexture2DLibrary() = default;

protected:
    void Unbind_Impl(uint32_t slot) override;

    Ref<Texture2D> Create_Impl(const std::string& name, const std::string& path) override;
};

} // namespace Mist