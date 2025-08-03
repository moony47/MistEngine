#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath) {
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniformMat4f(int loc, const glm::mat4& mat) {
    Bind();
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniform1i(int loc, int v0) {
    Bind();
    GLCall(glUniform1i(loc, v0));
}

void Shader::SetUniform1iv(int loc, unsigned int count, int* v) {
    Bind();
    GLCall(glUniform1iv(loc, count, v));
}

void Shader::SetUniform1f(int loc, float v0) {
    Bind();
    GLCall(glUniform1f(loc, v0));
}

void Shader::SetUniform4f(int loc, float v0, float v1, float v2, float v3) {
    Bind();
    GLCall(glUniform4f(loc, v0, v1, v2, v3));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];

    std::ifstream stream(filepath);
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            ss[(int)type] << line << '\n';
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Check compilation was successfull
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) const {
    //if (auto locationIter = m_UniformLocationCache.find(name); locationIter != m_UniformLocationCache.end())
    //    return locationIter->second;

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    //m_UniformLocationCache[name] = location;
    return location;
}
