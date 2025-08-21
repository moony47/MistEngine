#pragma once

#include "glm/glm.hpp"
#include "Texture.h"

namespace Mist {

class Shader {
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // Set Uniforms
    virtual void SetUniformMat4f(const std::string& uniformName, const glm::mat4& v0) const = 0;
    virtual void SetUniform1i(const std::string& uniformName, int v0) const = 0;
    virtual void SetUniform1iv(const std::string& uniformName, unsigned int count, int* v0) const = 0;
    virtual void SetUniform1f(const std::string& uniformName, float v0) const = 0;
    virtual void SetUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3) const = 0;
    // virtual void SetUniform4fv(int loc, const glm::vec4& v) const;

    virtual void SetUniformTexture2D(const std::string& uniformName, Ref<Texture2D> texture) const = 0;

    //virtual int GetUniformLocation(const std::string& name) const = 0;

    virtual const std::string& GetName() const = 0;

protected:
    struct ShaderProgramSource {
        std::string VertexSource;
        std::string FragmentSource;
    };
};

class ShaderLibrary {
public:
    void Bind(const std::string& name);
    void Unbind();

    Ref<Shader> Get(const std::string& name);
    Ref<Shader> Create(const std::string& name, const std::string& vertShaderPath, const std::string& fragShaderPath);
    void Remove(const std::string& name);

    inline static ShaderLibrary* GetInstance() {
        return s_Instance;
    }

protected:
    virtual void Unbind_Impl() = 0;
    virtual Ref<Shader> Create_Impl(const std::string& name,
                                    const std::string& vertShaderPath,
                                    const std::string& fragShaderPath) = 0;

private:
    inline bool Exists(const std::string& name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }

private:
    static ShaderLibrary* s_Instance;

    std::string m_CurrentShader = "";
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

#define SHADERLIB ShaderLibrary::GetInstance()

} // namespace Mist
