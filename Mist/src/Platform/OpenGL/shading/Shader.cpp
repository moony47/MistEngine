#include "mistpch.h"

#include "Shader.h"

#include "Renderer.h"
#include "ShaderController.h"

namespace Mist {

Shader::Shader(ShaderController& shaderController,
               const std::string& vertShaderPath,
               const std::string& fragShaderPath) :
    m_ShaderController(shaderController) {
    ShaderProgramSource source = ParseShader(vertShaderPath, fragShaderPath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    m_ShaderController.DeregisterShader(m_RendererID);
    MS_GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    m_ShaderController.BindShader(m_RendererID);
}

void Shader::Unbind() const {
    m_ShaderController.UnbindShader();
}

void Shader::SetUniformMat4f(int loc, const glm::mat4& mat) const {
    Bind();
    MS_GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniform1i(int loc, int v0) const {
    Bind();
    MS_GLCALL(glUniform1i(loc, v0));
}

void Shader::SetUniform1iv(int loc, unsigned int count, int* v) const {
    Bind();
    MS_GLCALL(glUniform1iv(loc, count, v));
}

void Shader::SetUniform1f(int loc, float v0) const {
    Bind();
    MS_GLCALL(glUniform1f(loc, v0));
}

void Shader::SetUniform4f(int loc, float v0, float v1, float v2, float v3) const {
    Bind();
    MS_GLCALL(glUniform4f(loc, v0, v1, v2, v3));
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const {
    MS_GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    MS_GLCALL(glShaderSource(id, 1, &src, nullptr));
    MS_GLCALL(glCompileShader(id));

    // Check compilation was successful
    int result;
    MS_GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result) {
        int length;
        MS_GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        MS_GLCALL(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
                  << std::endl;
        std::cout << message << std::endl;
        MS_GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    MS_GLCALL(glAttachShader(program, vs));
    MS_GLCALL(glAttachShader(program, fs));
    MS_GLCALL(glLinkProgram(program));
    MS_GLCALL(glValidateProgram(program));

    MS_GLCALL(glDeleteShader(vs));
    MS_GLCALL(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) const {
    MS_GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    return location;
}

} // namespace Mist