#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_Filepath;
	unsigned int m_RendererID;
	//mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniformMat4f(int loc, const glm::mat4& v0);
	void SetUniform1i(int loc, int v0);
	void SetUniform1iv(int loc, unsigned int count, int* v0);
	void SetUniform1f(int loc, float v0);
	void SetUniform4f(int loc, float v0, float v1, float v2, float v3);

	int GetUniformLocation(const std::string& name) const;
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

