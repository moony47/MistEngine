#include "TestSpritesBatch.h"

#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"



test::TestSpritesBatch::TestSpritesBatch(float winWidth, float winHeight)
	: m_Width(winWidth), m_Height(winHeight) {

	const int numSprites = 4096;
	m_Sprites.reserve(numSprites);

	float tex, Vx, Vy, Cr, Cg, Cb;
	for (int i = 0; i < numSprites; i++) {
		// Define sprites with random spacial, chomatic velocities and texture
		Vx = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 500.0f - 250.0f;
		Vy = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 500.0f - 250.0f;

		Cr = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2.5f + 0.5f;
		Cg = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2.5f + 0.5f;
		Cb = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2.5f + 0.5f;

		tex = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);

		m_Sprites.emplace_back(
			tex > 0.5f,
			glm::vec4(0.3f, 0.3f, 0.8f, 1.0f),
			glm::vec4(Cr, Cg, Cb, 0.0f),
			glm::vec3(500.0f, 500.0f, 0.0f),
			glm::vec3(Vx, Vy, 0.0f)
		);
	}

	// Define VertexBuffer layout
	m_VBL = std::make_unique<VertexBufferLayout>();
	m_VBL->Push<float>(2);
	m_VBL->Push<float>(2);
	m_VBL->Push<float>(4);
	m_VBL->Push<float>(1);

	// Compile shader and push onto device
	m_Shader = std::make_unique<Shader>("res/shaders/Batch.shader");

	// Cache uniform locations
	m_uMVPLoc = m_Shader->GetUniformLocation("u_MVP");
	m_uTexLoc = m_Shader->GetUniformLocation("u_Texture");

	// Push textures onto device
	m_TexDiamond = std::make_unique<Texture>("res/textures/diamond.png");
	m_TexStar = std::make_unique<Texture>("res/textures/star.png");
	m_TexDiamond->Bind(0);
	m_TexStar->Bind(1);

	// Attatch textures to shader
	int samplers[] = { 0, 1 };
	m_Shader->SetUniform1iv(m_uTexLoc, 2, samplers);

	// Define a fixed MVP
	glm::mat4 proj = glm::ortho(0.0f, m_Width, 0.0f, m_Height);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = proj * view * model;
	m_Shader->SetUniformMat4f(m_uMVPLoc, mvp);

	m_Shader->Unbind();
}

void test::TestSpritesBatch::OnUpdate(float deltaTime) {
	int i = 0;
	int numSprites = m_Sprites.size();

	// Create mem buffers to be pushed onto device
	float* vertexData = new float[numSprites * 4 * 9];
	unsigned int* indexData = new unsigned int[numSprites * 6];

	// Update location and colour of sprites.
	// Push their vertices and indicies into buffers
	for (auto& sprite : m_Sprites) {
		sprite.Update(deltaTime, m_Width, m_Height, i, &vertexData[i * 4 * 9], &indexData[i * 6]);
		i++;
	}

	// Create new VertexArray
	m_VA = std::make_unique<VertexArray>();

	// Create and attach VertexBuffer and IndexBuffer
	VertexBuffer vb(vertexData, numSprites * 4 * 9 * sizeof(float));
	IndexBuffer ib(indexData, numSprites * 6);
	m_VA->AddBuffer(vb, *m_VBL);

	// Unbind everything before vb and ib are deleted
	m_VA->Unbind();
	vb.Unbind();
	ib.Unbind();

	// Free the CPU side contiguous buffers
	delete[] vertexData;
	delete[] indexData;
}

void test::TestSpritesBatch::OnRender(const Renderer& renderer) {
	// Make single call to draw all sprites
	renderer.Draw(*m_VA, *m_Shader, m_Sprites.size() * 6);

	// Delete VertexArray ready for next frame
	m_VA.reset();
}
