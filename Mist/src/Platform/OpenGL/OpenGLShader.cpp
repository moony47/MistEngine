#include "mistpch.h"

#include "OpenGLRenderer.h"
#include "OpenGLShader.h"

#include "Mist/Renderer/ShaderController.h"
#include "glm/gtc/type_ptr.hpp"

namespace Mist {

OpenGLShader::OpenGLShader(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    ShaderProgramSource source = ParseShader(vertShaderPath, fragShaderPath);
    auto result = CreateShader(source.VertexSource, source.FragmentSource);
    if (result)
        m_RendererID = *result;
    else
        MIST_CORE_ERROR(result.error());
}

OpenGLShader::~OpenGLShader() {
    ShaderController::GetInstance()->DeregisterShader(m_RendererID);
    MIST_GLCALL(glDeleteProgram(m_RendererID));
}

void OpenGLShader::Bind() const {
    ShaderController::GetInstance()->BindShader(m_RendererID);
}

void OpenGLShader::Unbind() const {
    ShaderController::GetInstance()->UnbindShader();
}

void OpenGLShader::SetUniformMat4f(int loc, const glm::mat4& mat) const {
    Bind();
    MIST_GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat)));
}

void OpenGLShader::SetUniform1i(int loc, int v0) const {
    Bind();
    MIST_GLCALL(glUniform1i(loc, v0));
}

void OpenGLShader::SetUniform1iv(int loc, unsigned int count, int* v) const {
    Bind();
    MIST_GLCALL(glUniform1iv(loc, count, v));
}

void OpenGLShader::SetUniform1f(int loc, float v0) const {
    Bind();
    MIST_GLCALL(glUniform1f(loc, v0));
}

void OpenGLShader::SetUniform4f(int loc, float v0, float v1, float v2, float v3) const {
    Bind();
    MIST_GLCALL(glUniform4f(loc, v0, v1, v2, v3));
}

//void OpenGLShader::SetUniform4fv(int loc, const glm::vec4& v) const {
//    Bind();
//    MIST_GLCALL(glUniform4fv(loc, 1, glm::value_ptr(v)));
//}

OpenGLShader::ShaderProgramSource OpenGLShader::ParseShader(const std::string& vertShaderPath,
                                                            const std::string& fragShaderPath) const {
    std::stringstream ss[2];

    std::ifstream vertStream(vertShaderPath);
    ss[0] << vertStream.rdbuf();
    std::ifstream fragStream(fragShaderPath);
    ss[1] << fragStream.rdbuf();

    return {ss[0].str(), ss[1].str()};
}

std::expected<unsigned int, std::string> OpenGLShader::CompileShader(unsigned int type,
                                                                     const std::string& source) const {
    MIST_GLCALL(unsigned int id = glCreateShader(type));
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
        MIST_CORE_ERROR("Failed to compile ", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader");
        MIST_CORE_ERROR(message);
        MIST_GLCALL(glDeleteShader(id));
        return std::unexpected(message);
    }

    return id;
}

std::expected<unsigned int, std::string> OpenGLShader::CreateShader(const std::string& vertexShader,
                                                                    const std::string& fragmentShader) const {
    unsigned int program = glCreateProgram();

    auto vertexResult = CompileShader(GL_VERTEX_SHADER, vertexShader);
    if (!vertexResult)
        return vertexResult;

    auto fragmentResult = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (!fragmentResult)
        return fragmentResult;

    MIST_GLCALL(glAttachShader(program, *vertexResult));
    MIST_GLCALL(glAttachShader(program, *fragmentResult));
    MIST_GLCALL(glLinkProgram(program));
    MIST_GLCALL(glValidateProgram(program));

    MIST_GLCALL(glDeleteShader(*vertexResult));
    MIST_GLCALL(glDeleteShader(*fragmentResult));

    return program;
}

int OpenGLShader::GetUniformLocation(const std::string& name) const {
    MIST_GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        MIST_CORE_WARN("Uniform '{0}' doesn't exist", name);
    return location;
}

} // namespace Mist