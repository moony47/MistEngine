#include "mistpch.h"

#include "Shader.h"

#include "OpenGL/OpenGLAssets.h"

namespace Mist {

ShaderLibrary* ShaderLibrary::s_Instance = new OpenGLShaderLibrary;

void ShaderLibrary::Bind(const std::string& name) {
    MIST_CORE_ASSERT(Exists(name), "[ShaderLibrary::Bind] Shader not found");

    m_Shaders[name]->Bind();
    m_CurrentShader = name;
}

void ShaderLibrary::Unbind() {
    Unbind_Impl();
    m_CurrentShader = "";
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
    MIST_CORE_ASSERT(Exists(name), "[ShaderLibrary::Get] Shader not found");

    return m_Shaders[name];
}

Ref<Shader> ShaderLibrary::Create(const std::string& name,
                                  const std::string& vertShaderPath,
                                  const std::string& fragShaderPath) {
    MIST_CORE_ASSERT(!Exists(name), "[ShaderLibrary::Create] Shader already exists");

    Ref<Shader> shader = Create_Impl(name, vertShaderPath, fragShaderPath);
    m_Shaders[name] = shader;
    return shader;
}

void ShaderLibrary::Remove(const std::string& name) {
    MIST_CORE_ASSERT(!Exists(name), "[ShaderLibrary::Remove] Shader not found");

    if (name == m_CurrentShader)
        m_Shaders[name]->Unbind();
    m_Shaders.erase(name);
}

} // namespace Mist