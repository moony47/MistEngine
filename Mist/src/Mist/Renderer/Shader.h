#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace Mist {

class Shader {
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
    virtual void SetFloat(const std::string& name, float value) = 0;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

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
    Ref<Shader> Create(const std::string& name, const std::string& shaderDirectory = "res/shaders");
    void Remove(const std::string& name);

    inline static ShaderLibrary* GetInstance() {
        return s_Instance;
    }

protected:
    virtual void Unbind_Impl() = 0;
    virtual Ref<Shader> Create_Impl(const std::string& name, const std::string& shaderDirectory) = 0;

private:
    inline bool ShaderExists(const std::string& name) const {
        return m_Shaders.contains(name);
    }

private:
    static ShaderLibrary* s_Instance;

    std::string m_CurrentShader;
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

#define MIST_SHADERLIB ShaderLibrary::GetInstance()
#define MIST_SHADER(x) ShaderLibrary::GetInstance()->Get(x)

} // namespace Mist
