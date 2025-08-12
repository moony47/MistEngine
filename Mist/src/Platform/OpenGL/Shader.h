#pragma once

#include "glm/glm.hpp"

namespace Mist {

class ShaderController;

class Shader {
private:
    // std::string m_VertShaderPath;
    // std::string m_FragShaderPath;
    unsigned int m_RendererID;
    ShaderController& m_ShaderController;

public:
    Shader(ShaderController& shaderController, const std::string& vertShaderPath, const std::string& fragShaderPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set Uniforms
    void SetUniformMat4f(int loc, const glm::mat4& v0) const;
    void SetUniform1i(int loc, int v0) const;
    void SetUniform1iv(int loc, unsigned int count, int* v0) const;
    void SetUniform1f(int loc, float v0) const;
    void SetUniform4f(int loc, float v0, float v1, float v2, float v3) const;

    int GetUniformLocation(const std::string& name) const;

private:
    struct ShaderProgramSource {
        std::string VertexSource;
        std::string FragmentSource;
    };

    ShaderProgramSource ParseShader(const std::string& vertShaderPath, const std::string& fragShaderPath) const;
    std::expected<unsigned int, std::string> CompileShader(unsigned int type, const std::string& source) const;
    std::expected<unsigned int, std::string> CreateShader(const std::string& vertexShader,
                                                          const std::string& fragmentShader) const;
};

} // namespace Mist