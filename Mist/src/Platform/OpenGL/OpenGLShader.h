#pragma once

#include "Mist/Renderer/Shader.h"

#include <unordered_map>

namespace Mist {

typedef unsigned int GLenum;

class OpenGLShader : public Shader {
public:
    // OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
    OpenGLShader(const std::string& name, const std::string& directory);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void SetInt(const std::string& name, int value) override;
    void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    void SetFloat(const std::string& name, float value) override;
    void SetFloat2(const std::string& name, const glm::vec2& value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat3(const std::string& name, const glm::mat3& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    const std::string& GetName() const override {
        return m_Name;
    }

private:
    std::string ReadFile(const std::string& filepath);

    void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
    void CompileOrGetOpenGLBinaries();
    void CreateProgram();
    void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

private:
    uint32_t m_RendererID;
    std::string m_Name;

    std::unordered_map<GLenum, std::string> m_Filepaths;

    std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
    std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

    std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
};

} // namespace Mist