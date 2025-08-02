#include "TestSprites.h"

#include <cstdlib>

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

float positions[] = {
	-50.0f, -50.0f, 0.0f, 0.0f,
	 50.0f, -50.0f, 1.0f, 0.0f,
	 50.0f,  50.0f, 1.0f, 1.0f,
	-50.0f,  50.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

test::TestSprites::TestSprites() : 
	m_Sprites{},
	vb(positions, sizeof(positions)),
	ib(indices, sizeof(indices) / sizeof(indices[0])),
	shader("res/shaders/Basic.shader"),
	texture("res/textures/diamond.png"),
	view(glm::mat4(1.0f)) {

	for (int i = 0; i < 1; i++)
		m_Sprites.push_back(Sprite(
			glm::vec4(0.3f, 0.3f, 0.8f, 1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 5.0f, 0.0f)
		));

	// Vertex Buffer Layout
	layout.Push<float>(2);
	layout.Push<float>(2);

	// Vertex Array
	va.AddBuffer(vb, layout);

	// Add texture to shader
	shader.Bind();
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
	texture.Unbind();
}

test::TestSprites::~TestSprites() {
	for (int i = 0; i < m_Sprites.size(); i++)
		delete &m_Sprites[i];
	delete &m_Sprites;
}

void test::TestSprites::OnUpdate(float deltaTime) {
	return;
	for (int i = 0; i < m_Sprites.size(); i++)
		m_Sprites[i] = m_Sprites[i].Update(deltaTime);
}

void test::TestSprites::OnRender(const Renderer& renderer, const glm::mat4& proj) {
	for (int i = 0; i < m_Sprites.size(); i++) {
		shader.Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Sprites[i].Position);
		glm::mat4 mvp = proj * view * model;
		shader.SetUniformMat4f("u_MVP", mvp);

		shader.SetUniform4f("u_Colour", m_Sprites[i].Colour.r, m_Sprites[i].Colour.g, m_Sprites[i].Colour.b, m_Sprites[i].Colour.a);

		renderer.Draw(va, ib, shader);
	}
}

bool test::TestSprites::OnImGuiRender() {
	return ImGui::Button("Return to Menu");
}
