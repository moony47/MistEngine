#include <Mist.h>

#include "ImGui/imgui.h"

using namespace Mist;

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
		Layer("Example") {
		m_VertexArray.reset(VertexArray::Create());

		float vertices[4 * 4] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			+50.0f, -50.0f, 1.0f, 0.0f,
			-50.0f, +50.0f, 0.0f, 1.0f,
			+50.0f, +50.0f, 1.0f, 1.0f,
		};

		uint32_t indices[] = {
			0,1,2,
			1,2,3
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"}
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(ShaderController::GetInstance()->CreateShader("../Mist/res/shaders/Basic.vert", "../Mist/res/shaders/Basic.frag"));
		m_Shader->Bind();

		m_Texture.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/diamond.png"));
		m_Texture->Bind(0);
		m_Shader->SetUniform1i(m_Shader->GetUniformLocation("u_Texture"), 0);

		m_Shader->SetUniform4f(m_Shader->GetUniformLocation("u_Colour"), 0.8f, 0.2f, 0.2f, 1.0f);

		m_Camera = std::make_shared<OrthographicCamera>(0.0f, 0.0f, 0.0f, -640.0f, 640.0f, -360.0f, 360.0f);
    }

	void OnUpdateStart() override {
		m_Camera->SetRotation(theta);
		theta += 1.0f;
		m_Camera->SetPosition(x, 0.0f, 0.0f);
		x += 1.0f;

		Renderer::BeginScene(*m_Camera /*lights, environment*/);

		glm::mat4 model(1.0f);
		m_Shader->SetUniformMat4f(m_Shader->GetUniformLocation("u_M"), model);

		Renderer::Submit(m_Shader, m_VertexArray);
    }

	void OnUpdateEnd() override {
		Renderer::EndScene();

		//Renderer::Flush();
    }

    void OnImGuiRender() override {
    }

    void OnEvent(Mist::Event& e) override {
    }

private:
	std::shared_ptr<OrthographicCamera> m_Camera;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<VertexArray> m_VertexArray;

	float theta = 0.0f;
	float x = 0.0f;
};

class Sandbox : public Mist::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }
    ~Sandbox() {
    }
};

Mist::Application* Mist::CreateApplication() {
    return new Sandbox();
}
