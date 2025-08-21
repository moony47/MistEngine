#pragma once

#include <unordered_map>
#include "Mist/Renderer/Shader.h"

namespace Mist {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& name, const std::string& vertShaderPath, const std::string& fragShaderPath);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    // Set Uniforms
    void SetUniformMat4f(const std::string& uniformName, const glm::mat4& v0) const override;
    void SetUniform1i(const std::string& uniformName, int v0) const override;
    void SetUniform1iv(const std::string& uniformName, uint32_t count, int* v0) const override;
    void SetUniform1f(const std::string& uniformName, float v0) const override;
    void SetUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3) const override;
    // void SetUniform4fv(int loc, const glm::vec4& v) const override;

    void SetUniformTexture2D(const std::string& uniformName, Ref<Texture2D> texture) const override;

    //int GetUniformLocation(const std::string& name) const override;

    const std::string& GetName() const override {
        return m_Name;
    }

private:
    std::string ReadFile(const std::string& path) const;
    std::expected<uint32_t, std::string> CompileShader(uint32_t type, const std::string& source) const;
    std::expected<uint32_t, std::string> CreateShader(const ShaderProgramSource& source) const;
    void FindUniforms(const std::string& source);

private:
    uint32_t m_RendererID;
    std::string m_Name;

    std::unordered_map<std::string, int> m_UniformLocations;
};

} // namespace Mist