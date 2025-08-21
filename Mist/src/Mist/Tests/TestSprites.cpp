#include "mistpch.h"

#include "TestSprites.h"

#include "Mist/Renderer/Shader.h"
#include "Mist/Renderer/ShaderController.h"
#include "Mist/Renderer/Texture.h"
#include "Mist/Renderer/Buffer.h"
#include "Mist/Renderer/VertexArray.h"
#include "Mist/Renderer/Renderer.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/glm.hpp"

namespace Mist::Testing {

	TestSprites::TestSprites(float winWidth, float winHeight) :
		m_Width(winWidth),
		m_Height(winHeight),
		m_Camera(0.0f, 0.0f, 0.0f, 0.0f, m_Width, 0.0f, m_Height) {
		const int numSprites = 4096;

		m_StarSprites.reserve(numSprites / 2);
		m_DiamondSprites.reserve(numSprites / 2);

		float tex, Vx, Vy, Cr, Cg, Cb;
		for (int i = 0; i < numSprites; i++) {
			tex = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			Vx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 500.0f - 250.0f;
			Vy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 500.0f - 250.0f;
			Cr = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
			Cg = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
			Cb = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
			if (tex > 0.5f)
				m_StarSprites.emplace_back(true, glm::vec4(0.3f, 0.3f, 0.8f, 1.0f), glm::vec4(Cr, Cg, Cb, 0.0f),
					glm::vec3(500.0f, 500.0f, 0.0f), glm::vec3(Vx, Vy, 0.0f));
			else
				m_DiamondSprites.emplace_back(false, glm::vec4(0.3f, 0.3f, 0.8f, 1.0f), glm::vec4(Cr, Cg, Cb, 0.0f),
					glm::vec3(500.0f, 500.0f, 0.0f), glm::vec3(Vx, Vy, 0.0f));
		}

		m_VA.reset(VertexArray::Create());

		Ref<VertexBuffer> vb;
		vb.reset(VertexBuffer::Create(singleQuadVertices, 16 * sizeof(float)));
		BufferLayout vbl = {
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoords"}
		};
		vb->SetLayout(vbl);
		m_VA->AddVertexBuffer(vb);

		Ref<IndexBuffer> ib;
		ib.reset(IndexBuffer::Create(singleQuadIndices, 6));
		m_VA->SetIndexBuffer(ib);

		m_Shader.reset(ShaderController::GetInstance()->CreateShader("../Mist/res/shaders/Basic.vert",
			"../Mist/res/shaders/Basic.frag"));

		m_uVPLoc = m_Shader->GetUniformLocation("u_VP");
		m_uTexLoc = m_Shader->GetUniformLocation("u_Texture");
		m_uMLoc = m_Shader->GetUniformLocation("u_M");
		m_uColourLoc = m_Shader->GetUniformLocation("u_Colour");

		// Add textures
		m_TexDiamond.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/diamond.png"));
		m_TexStar.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/star.png"));
		m_TexDiamond->Bind(0);
		m_TexStar->Bind(1);

		m_Shader->SetUniformMat4f(m_uVPLoc, m_Camera.GetVP());

		vb->Unbind();
		m_VA->Unbind();
		m_Shader->Unbind();
	}

	void TestSprites::OnUpdate(float deltaTime) {
		for (auto& sprite : m_StarSprites)
			sprite.Update(deltaTime, m_Width, m_Height);
		for (auto& sprite : m_DiamondSprites)
			sprite.Update(deltaTime, m_Width, m_Height);
	}

	void TestSprites::OnRender() {
		glm::mat4 model(1.0f);

		m_Shader->SetUniform1i(m_uTexLoc, 1);
		for (auto& sprite : m_StarSprites) {
			model[3].x = sprite.Position.x;
			model[3].y = sprite.Position.y;
			m_Shader->SetUniformMat4f(m_uMLoc, model);

			m_Shader->SetUniform4f(m_uColourLoc, sprite.Colour.r, sprite.Colour.g, sprite.Colour.b, sprite.Colour.a);

			Renderer::Submit(m_Shader, m_VA);
		}

		m_Shader->SetUniform1i(m_uTexLoc, 0);
		for (auto& sprite : m_DiamondSprites) {
			model[3].x = sprite.Position.x;
			model[3].y = sprite.Position.y;
			m_Shader->SetUniformMat4f(m_uMLoc, model);

			m_Shader->SetUniform4f(m_uColourLoc, sprite.Colour.r, sprite.Colour.g, sprite.Colour.b, sprite.Colour.a);

			Renderer::Submit(m_Shader, m_VA);
		}
	}

	void TestSprites::Resize(unsigned int width, unsigned int height) {
		m_Width = (float)width;
		m_Height = (float)height;

		glm::mat4 proj = glm::ortho(0.0f, m_Width, 0.0f, m_Height);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 mvp = proj * view;
		m_Shader->SetUniformMat4f(m_uVPLoc, mvp);
	}

} // namespace Testing