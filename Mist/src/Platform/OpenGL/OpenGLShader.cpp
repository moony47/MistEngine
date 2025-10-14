#include "mistpch.h"

#include "OpenGLRenderer.h"
#include "OpenGLShader.h"

#include "glm/gtc/type_ptr.hpp"

namespace Mist {

OpenGLShader::OpenGLShader(const std::string& name,
                           const std::string& vertShaderPath,
                           const std::string& fragShaderPath) :
    m_Name(name) {
    MIST_PROFILE_FUNCTION();

    ShaderProgramSource source = {ReadFile(vertShaderPath), ReadFile(fragShaderPath)};
    auto result = CreateShader(source);

    if (result) {
        m_RendererID = *result;
        FindUniforms(source.VertexSource);
        FindUniforms(source.FragmentSource);
    } else
        MIST_CORE_ERROR("[OpenGLShader::OpenGLShader] {0}", result.error());
}

OpenGLShader::~OpenGLShader() {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glDeleteProgram(m_RendererID));
}

std::string OpenGLShader::ReadFile(const std::string& path) const {
    MIST_PROFILE_FUNCTION();

    std::string result;
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    } else
        MIST_CORE_ERROR("[OpenGLShader::ReadFile] Could not open file {0}", path);
    return result;
}

void OpenGLShader::Bind() const {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glUseProgram(m_RendererID));
}

void OpenGLShader::Unbind() const {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glUseProgram(0));
}

void OpenGLShader::SetUniformMat4f(const std::string& uniformName, const glm::mat4& mat) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat)));
}

void OpenGLShader::SetUniform1i(const std::string& uniformName, int v0) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniform1i(loc, v0));
}

void OpenGLShader::SetUniform1iv(const std::string& uniformName, uint32_t count, int* v) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniform1iv(loc, count, v));
}

void OpenGLShader::SetUniform1f(const std::string& uniformName, float v0) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniform1f(loc, v0));
}

void OpenGLShader::SetUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniform4f(loc, v0, v1, v2, v3));
}

void OpenGLShader::SetUniform1b(const std::string& uniformName, bool val) const {
    MIST_PROFILE_FUNCTION();

    int loc = m_UniformLocations.at(uniformName);
    MIST_GLCALL(glUniform1i(loc, val));
}

// void OpenGLShader::SetUniform4fv(int loc, const glm::vec4& v) const {
//     Bind();
//     MIST_GLCALL(glUniform4fv(loc, 1, glm::value_ptr(v)));
// }

std::expected<uint32_t, std::string> OpenGLShader::CompileShader(uint32_t type, const std::string& source) const {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(uint32_t id = glCreateShader(type));
    const char* src = source.c_str();
    MIST_GLCALL(glShaderSource(id, 1, &src, nullptr));
    MIST_GLCALL(glCompileShader(id));

    // Check compilation was successful
    int result;
    MIST_GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result) {
        int length;
        MIST_GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        MIST_GLCALL(glGetShaderInfoLog(id, length, &length, message));

        MIST_CORE_ERROR("[OpenGLShader::CompileShader] Failed to compile '{0}' shader:",
                        type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        MIST_GLCALL(glDeleteShader(id));

        return std::unexpected(message);
    }

    return id;
}

std::expected<uint32_t, std::string> OpenGLShader::CreateShader(const ShaderProgramSource& source) const {
    MIST_PROFILE_FUNCTION();

    uint32_t program = glCreateProgram();

    auto vertexResult = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    if (!vertexResult) {
        MIST_GLCALL(glDeleteProgram(program));
        return vertexResult;
    }

    auto fragmentResult = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);
    if (!fragmentResult) {
        MIST_GLCALL(glDeleteShader(*vertexResult));
        MIST_GLCALL(glDeleteProgram(program));
        return fragmentResult;
    }

    MIST_GLCALL(glAttachShader(program, *vertexResult));
    MIST_GLCALL(glAttachShader(program, *fragmentResult));
    MIST_GLCALL(glLinkProgram(program));
    MIST_GLCALL(glValidateProgram(program));

    MIST_GLCALL(glDeleteShader(*vertexResult));
    MIST_GLCALL(glDeleteShader(*fragmentResult));

    return program;
}

void OpenGLShader::FindUniforms(const std::string& source) {
    MIST_PROFILE_FUNCTION();

    size_t offset = 0;
    size_t start = source.find("uniform ", 0);
    size_t end;
    while (start != -1) {
        start = source.find(' ', start + 8) + 1;

        size_t bracket = source.find('[', start);
        size_t semicolon = source.find(';', start);
        if (bracket < semicolon && bracket != 0) {
            end = bracket;
        } else {
            end = semicolon;
        }

        std::string sub = source.substr(start, end - start);
        MIST_GLCALL(m_UniformLocations[sub] = glGetUniformLocation(m_RendererID, sub.c_str()));
        offset = end;
        start = source.find("uniform ", offset);
    }
}

} // namespace Mist