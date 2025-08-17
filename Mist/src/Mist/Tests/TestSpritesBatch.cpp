#include "mistpch.h"

#include "TestSpritesBatch.h"

#include "Mist/Renderer/Shader.h"
#include "Mist/Renderer/ShaderController.h"
#include "Mist/Renderer/Texture.h"
#include "Mist/Renderer/Renderer.h"
#include "Mist/Renderer/Buffer.h"
#include "Mist/Renderer/VertexArray.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/glm.hpp"

namespace Mist::Testing {

	const size_t numSprites = 4096;

	TestSpritesBatch::TestSpritesBatch(float winWidth, float winHeight) :
		m_Width(winWidth),
		m_Height(winHeight) {

		m_VertexBuffer = std::make_unique<float[]>(numSprites * 4 * 9);
		m_IndexBuffer = std::make_unique<unsigned int[]>(numSprites * 6);

		m_Sprites.reserve(numSprites);

		float tex, Vx, Vy, Cr, Cg, Cb;
		for (size_t i = 0; i < numSprites; i++) {
			// Define sprites with random spacial and chromatic velocities and texture index
			Vx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 500.0f - 250.0f;
			Vy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 500.0f - 250.0f;

			Cr = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
			Cg = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
			Cb = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;

			tex = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

			m_Sprites.emplace_back(tex > 0.5f, glm::vec4(Cr, Cg, Cb, 0.0f), glm::vec3(Vx, Vy, 0.0f),
				&m_VertexBuffer[i * 4 * 9], &m_IndexBuffer[i * 6]);
		}

		// Define VertexBuffer layout
		m_VBL.reset(new BufferLayout({
				{ShaderDataType::Float2, "a_Position"},
				{ShaderDataType::Float2, "a_TexCoords"},
				{ShaderDataType::Float4, "a_Colour"},
				{ShaderDataType::Int, "a_SamplerIndex"},
			}));

		// Compile shader and push onto device
		m_Shader.reset(ShaderController::GetInstance()->CreateShader("../Mist/res/shaders/Batch.vert", "../Mist/res/shaders/Batch.frag"));

		// Cache uniform locations
		m_uMVPLoc = m_Shader->GetUniformLocation("u_MVP");
		m_uTexLoc = m_Shader->GetUniformLocation("u_Texture");

		// Push textures onto device
		m_TexDiamond.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/diamond.png"));
		m_TexStar.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/star.png"));
		m_TexDiamond->Bind(0);
		m_TexStar->Bind(1);

		// Attach textures to shader
		int samplers[] = { 0, 1 };
		m_Shader->SetUniform1iv(m_uTexLoc, 2, samplers);

		// Define a fixed MVP
		glm::mat4 proj = glm::ortho(0.0f, m_Width, 0.0f, m_Height);
		glm::mat4 view(1.0f);
		glm::mat4 model(1.0f);
		glm::mat4 mvp = proj * view * model;
		m_Shader->SetUniformMat4f(m_uMVPLoc, mvp);

		m_Shader->Unbind();
	}

	void TestSpritesBatch::OnUpdate(float deltaTime) {
		// Update location and colour of sprites.
		// Push their vertices and indices into buffers
		for (size_t i = 0; i < m_Sprites.size(); i++)
			m_Sprites[i].Update(deltaTime, m_Width, m_Height, (unsigned int)i);

		// Create new VertexArray
		m_VA.reset(VertexArray::Create());

		// Create and attach VertexBuffer and IndexBuffer
		std::shared_ptr<VertexBuffer> vb;
		vb.reset(VertexBuffer::Create(m_VertexBuffer.get(), numSprites * 4 * 9 * sizeof(float)));
		std::shared_ptr<IndexBuffer> ib;
		ib.reset(IndexBuffer::Create(m_IndexBuffer.get(), numSprites * 6));

		vb->SetLayout(*m_VBL);
		m_VA->AddVertexBuffer(vb);
		m_VA->SetIndexBuffer(ib);

		// Unbind everything before vb and ib are deleted
		m_VA->Unbind();
		vb->Unbind();
		ib->Unbind();
	}

	void TestSpritesBatch::OnRender() {
		// Make single call to draw all sprites
		Renderer::Submit(m_Shader, m_VA);

		// Delete VertexArray ready for next frame
		m_VA.reset();
	}

	void TestSpritesBatch::Resize(unsigned int width, unsigned int height) {
		m_Width = (float)width;
		m_Height = (float)height;

		glm::mat4 proj = glm::ortho(0.0f, m_Width, 0.0f, m_Height);
		glm::mat4 view(1.0f);
		glm::mat4 model(1.0f);
		glm::mat4 mvp = proj * view * model;
		m_Shader->SetUniformMat4f(m_uMVPLoc, mvp);
	}

} // namespace Mist::Testing
