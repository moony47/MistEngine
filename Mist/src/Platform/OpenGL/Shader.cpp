#include "mistpch.h"

#include "Shader.h"

#include "OpenGL/Renderer.h"
#include "ShaderController.h"

namespace Mist {

Shader::Shader(ShaderController& shaderController,
               const std::string& vertShaderPath,
               const std::string& fragShaderPath) :
    m_ShaderController(shaderController) {
    ShaderProgramSource source = ParseShader(vertShaderPath, fragShaderPath);
    auto result = CreateShader(source.VertexSource, source.FragmentSource);
    if (result)
        m_RendererID = *result;
    else
        MIST_CORE_ERROR(result.error());
}

Shader::~Shader() {
    m_ShaderController.DeregisterShader(m_RendererID);
    MIST_GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    m_ShaderController.BindShader(m_RendererID);
}

void Shader::Unbind() const {
    m_ShaderController.UnbindShader();
}

void Shader::SetUniformMat4f(int loc, const glm::mat4& mat) const {
    Bind();
    MIST_GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniform1i(int loc, int v0) const {
    Bind();
    MIST_GLCALL(glUniform1i(loc, v0));
}

void Shader::SetUniform1iv(int loc, unsigned int count, int* v) const {
    Bind();
    MIST_GLCALL(glUniform1iv(loc, count, v));
}

void Shader::SetUniform1f(int loc, float v0) const {
    Bind();
    MIST_GLCALL(glUniform1f(loc, v0));
}

void Shader::SetUniform4f(int loc, float v0, float v1, float v2, float v3) const {
    Bind();
    MIST_GLCALL(glUniform4f(loc, v0, v1, v2, v3));
}

Shader::ShaderProgramSource Shader::ParseShader(const std::string& vertShaderPath,
                                                const std::string& fragShaderPath) const {
    std::stringstream ss[2];

    std::ifstream vertStream(vertShaderPath);
    ss[0] << vertStream.rdbuf();
    std::ifstream fragStream(fragShaderPath);
    ss[1] << fragStream.rdbuf();

    return {ss[0].str(), ss[1].str()};
}

std::expected<unsigned int, std::string> Shader::CompileShader(unsigned int type, const std::string& source) const {
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

std::expected<unsigned int, std::string> Shader::CreateShader(const std::string& vertexShader,
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

int Shader::GetUniformLocation(const std::string& name) const {
    MIST_GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        MIST_CORE_WARN("Uniform '", name, "' doesn't exist");
    return location;
}

} // namespace Mist