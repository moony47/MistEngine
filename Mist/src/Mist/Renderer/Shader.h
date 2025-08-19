#pragma once

#include "glm/glm.hpp"

namespace Mist {

	class Shader {
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Set Uniforms
		virtual void SetUniformMat4f(int loc, const glm::mat4& v0) const = 0;
		virtual void SetUniform1i(int loc, int v0) const = 0;
		virtual void SetUniform1iv(int loc, unsigned int count, int* v0) const = 0;
		virtual void SetUniform1f(int loc, float v0) const = 0;
        virtual void SetUniform4f(int loc, float v0, float v1, float v2, float v3) const = 0;
        //virtual void SetUniform4fv(int loc, const glm::vec4& v) const;

		virtual int GetUniformLocation(const std::string& name) const = 0;

	protected:
		struct ShaderProgramSource {
			std::string VertexSource;
			std::string FragmentSource;
		};

		virtual ShaderProgramSource ParseShader(const std::string& vertShaderPath, const std::string& fragShaderPath) const = 0;
		virtual std::expected<unsigned int, std::string> CompileShader(unsigned int type, const std::string& source) const = 0;
		virtual std::expected<unsigned int, std::string> CreateShader(const std::string& vertexShader,
			const std::string& fragmentShader) const = 0;
	};

}

