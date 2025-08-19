#pragma once

#include "Mist/Renderer/Shader.h"

namespace Mist {

	class OpenGLShader : public Shader {
	private:
		uint32_t m_RendererID;

	public:
		OpenGLShader(const std::string& vertShaderPath, const std::string& fragShaderPath);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		// Set Uniforms
		void SetUniformMat4f(int loc, const glm::mat4& v0) const override;
		void SetUniform1i(int loc, int v0) const override;
		void SetUniform1iv(int loc, unsigned int count, int* v0) const override;
		void SetUniform1f(int loc, float v0) const override;
		void SetUniform4f(int loc, float v0, float v1, float v2, float v3) const override;
        //void SetUniform4fv(int loc, const glm::vec4& v) const override;

		int GetUniformLocation(const std::string& name) const override;

	protected:

		ShaderProgramSource ParseShader(const std::string& vertShaderPath, const std::string& fragShaderPath) const override;
		std::expected<unsigned int, std::string> CompileShader(unsigned int type, const std::string& source) const override;
		std::expected<unsigned int, std::string> CreateShader(const std::string& vertexShader,
			const std::string& fragmentShader) const override;
	};

} // namespace Mist