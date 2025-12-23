#include "mistpch.h"
#include "OpenGLShader.h"

#include "Mist/Core/Timer.h"

#include <GLAD/glad.h>
#include <filesystem>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Mist {

namespace Utils {

static shaderc_shader_kind GlShaderStageToShaderC(GLenum stage) {
    switch (stage) {
        case GL_VERTEX_SHADER:
            return shaderc_glsl_vertex_shader;
        case GL_FRAGMENT_SHADER:
            return shaderc_glsl_fragment_shader;
    }
    MIST_CORE_ASSERT(false, "[GlShaderStageToShaderC] Unknown stage");
    return (shaderc_shader_kind)0;
}

static const char* GLShaderStageToString(GLenum stage) {
    switch (stage) {
        case GL_VERTEX_SHADER:
            return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER:
            return "GL_FRAGMENT_SHADER";
    }
    MIST_CORE_ASSERT(false, "[GlShaderStageToShaderC] Unknown stage");
    return nullptr;
}

static const char* GetCacheDirectory() {
    return "assets/cache/shaders/opengl";
}

static void CreateCacheDirectory() {
    std::string cacheDirectory = GetCacheDirectory();
    if (!std::filesystem::exists(cacheDirectory))
        std::filesystem::create_directories(cacheDirectory);
}

static const char* GLShaderStageCachedOpenGLFileExtension(GLenum stage) {
    switch (stage) {
        case GL_VERTEX_SHADER:
            return ".cached_opengl.vert";
        case GL_FRAGMENT_SHADER:
            return ".cached_opengl.frag";
    }
    MIST_CORE_ASSERT(false, "[GlShaderStageToShaderC] Unknown stage");
    return nullptr;
}

static const char* GLShaderStageCachedVulkanFileExtension(GLenum stage) {
    switch (stage) {
        case GL_VERTEX_SHADER:
            return ".cached_vulkan.vert";
        case GL_FRAGMENT_SHADER:
            return ".cached_vulkan.frag";
    }
    MIST_CORE_ASSERT(false, "[GlShaderStageToShaderC] Unknown stage");
    return nullptr;
}

} // namespace Utils

// OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) :
//     m_Name(name) {
//     MIST_PROFILE_FUNCTION();
//
//     std::unordered_map<GLenum, std::string> sources;
//     sources[GL_VERTEX_SHADER] = vertSrc;
//     sources[GL_FRAGMENT_SHADER] = fragSrc;
//
//     {
//         Timer timer;
//         CompileOrGetVulkanBinaries(sources);
//         CompileOrGetOpenGLBinaries();
//         CreateProgram();
//         MIST_CORE_WARN("OpenGL Shader creation took {0} ms", timer.ElapsedMilliseconds());
//     }
// }

OpenGLShader::OpenGLShader(const std::string& name, const std::string& directory) :
    m_Name(name) {
    MIST_PROFILE_FUNCTION();

    std::filesystem::path shaderPath = directory;

    std::filesystem::path vertPath = shaderPath / (name + ".vert");
    std::filesystem::path fragPath = shaderPath / (name + ".frag");

    MIST_CORE_ASSERT(std::filesystem::exists(vertPath),
                     "[OpenGLShader::OpenGLShader] Vertex shader source file does not exist");
    MIST_CORE_ASSERT(std::filesystem::exists(fragPath),
                     "[OpenGLShader::OpenGLShader] Fragment shader source file does not exist");

    m_Filepaths[GL_VERTEX_SHADER] = vertPath.string();
    m_Filepaths[GL_FRAGMENT_SHADER] = fragPath.string();

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = ReadFile(vertPath.string());
    sources[GL_FRAGMENT_SHADER] = ReadFile(fragPath.string());

    {
        Timer timer;
        CompileOrGetVulkanBinaries(sources);
        CompileOrGetOpenGLBinaries();
        CreateProgram();
        MIST_CORE_WARN("OpenGL Shader creation took {0} ms", timer.ElapsedMilliseconds());
    }
}

OpenGLShader::~OpenGLShader() {
    MIST_PROFILE_FUNCTION();

    glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    MIST_PROFILE_FUNCTION();

    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {

        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        } else
            MIST_CORE_ERROR("[OpenGLShader::ReadFile] Could not read from file '{0}'", filepath);

    } else
        MIST_CORE_ERROR("[OpenGLShader::ReadFile] Could not open file '{0}'", filepath);

    return result;
}

void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources) {
    GLuint program = glCreateProgram();

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
    const bool optimise = true;
    if (optimise)
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

    auto& shaderData = m_VulkanSPIRV;
    shaderData.clear();

    for (auto&& [stage, source] : shaderSources) {
        std::filesystem::path cachedPath =
            cacheDirectory / (m_Name + Utils::GLShaderStageCachedVulkanFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                source, Utils::GlShaderStageToShaderC(stage), m_Filepaths[stage].c_str(), options);
            MIST_CORE_ASSERT(module.GetCompilationStatus() == shaderc_compilation_status_success,
                             module.GetErrorMessage());

            shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }

    for (auto&& [stage, data] : shaderData)
        Reflect(stage, data);
}

void OpenGLShader::CompileOrGetOpenGLBinaries() {

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
    const bool optimise = false;
    if (optimise)
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

    auto& shaderData = m_OpenGLSPIRV;
    shaderData.clear();
    m_OpenGLSourceCode.clear();
    for (auto&& [stage, spirv] : m_VulkanSPIRV) {
        std::filesystem::path cachedPath =
            cacheDirectory / (m_Name + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            spirv_cross::CompilerGLSL glslCompiler(spirv);
            m_OpenGLSourceCode[stage] = glslCompiler.compile();
            auto& source = m_OpenGLSourceCode[stage];

            shaderc::SpvCompilationResult module =
                compiler.CompileGlslToSpv(source, Utils::GlShaderStageToShaderC(stage), m_Filepaths[stage].c_str());
            MIST_CORE_ASSERT(module.GetCompilationStatus() == shaderc_compilation_status_success,
                             module.GetErrorMessage());

            shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }
}

void OpenGLShader::CreateProgram() {
    GLuint program = glCreateProgram();

    std::vector<GLuint> shaderIDs;
    for (auto&& [stage, spirv] : m_OpenGLSPIRV) {
        GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
        glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(),
                       (GLsizei)spirv.size() * sizeof(uint32_t));
        glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
        glAttachShader(program, shaderID);
    }

    glLinkProgram(program);

    GLint isLinked;

    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
        MIST_CORE_ERROR("[OpenGLShader::CreateProgram] Shader linking failed ({0}):\n{1}", m_Name, infoLog.data());

        glDeleteProgram(program);

        for (auto id : shaderIDs)
            glDeleteShader(id);
    }

    for (auto id : shaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    m_RendererID = program;
}

void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData) {
    spirv_cross::Compiler compiler(shaderData);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    MIST_CORE_TRACE("[OpenGLShader::Reflect] {0} {1}", Utils::GLShaderStageToString(stage), m_Filepaths[stage]);
    MIST_CORE_TRACE("   {0} uniform buffers", resources.uniform_buffers.size());
    MIST_CORE_TRACE("   {0} resources", resources.sampled_images.size());

    MIST_CORE_TRACE("Uniform Buffers:");
    for (const auto& resource : resources.uniform_buffers) {
        const auto& bufferType = compiler.get_type(resource.base_type_id);
        uint32_t bufferSize = (uint32_t)compiler.get_declared_struct_size(bufferType);
        uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        int memberCount = (int)bufferType.member_types.size();

        MIST_CORE_TRACE("   {0}", resource.name);
        MIST_CORE_TRACE("   Size = {0}", bufferSize);
        MIST_CORE_TRACE("   Binding = {0}", binding);
        MIST_CORE_TRACE("   Members = {0}", memberCount);
    }
}

void OpenGLShader::Bind() const {
    MIST_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    MIST_PROFILE_FUNCTION();

    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetInt] Uniform name not found");

    glUniform1i(location, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetIntArray] Uniform name not found");

    glUniform1iv(location, count, values);
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetFloat] Uniform name not found");

    glUniform1f(location, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetFloat2] Uniform name not found");

    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetFloat3] Uniform name not found");

    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetFloat4] Uniform name not found");

    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetMat3] Uniform name not found");

    glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(value));
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
    MIST_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    MIST_CORE_ASSERT(location > -1, "[OpenGLShader::SetMat4] Uniform name not found");

    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

} // namespace Mist