#pragma once

#include <Mist.h>
using namespace Mist;

class ExampleLayer : public Layer {
public:
    ExampleLayer() :
        Layer("Example"),
        m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
        m_CameraController.SetZoomLevel(3.0f);
    }

    void OnAttach() override {
        m_VertexArray = VertexArray::Create();

        float vertices[4 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
            +0.5f, -0.5f, 1.0f, 0.0f, // 1
            -0.5f, +0.5f, 0.0f, 1.0f, // 2
            +0.5f, +0.5f, 1.0f, 1.0f, // 3
        };

        uint32_t indices[] = {0, 1, 2, 1, 2, 3};

        auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->SetLayout({
            {ShaderDataType::Float2,  "a_Position"},
            {ShaderDataType::Float2, "a_TexCoords"}
        });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        MIST_SHADERLIB->Create("BasicTexture", "res/shaders/Basic.vert", "res/shaders/Basic.frag");
        MIST_SHADERLIB->Bind("BasicTexture");

        MIST_TEXTURE2DLIB->Create("Diamond", "res/textures/diamond.png");
        MIST_TEXTURE2DLIB->Bind("Diamond", 1);
        MIST_SHADER("BasicTexture")->SetUniform1i("u_Texture", 1);

        RenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    }

    void OnDetach() {
        MIST_SHADERLIB->Remove("BasicTexture");
        MIST_TEXTURE2DLIB->Remove("Diamond");
    }

    void OnUpdate(DeltaTime deltaTime) override {
        m_CameraController.OnUpdate(deltaTime);
    }

    void OnFrameStart(DeltaTime deltaTime) override {
        Renderer::BeginScene(m_CameraController.GetCamera() /*lights, environment*/);
    }

    void OnFrameEnd(DeltaTime deltaTime) override {
        glm::mat4 spriteTransform =
            glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), m_SpritePosition), m_SpriteRotation, {0, 0, 1}),
                       {2.0f, 2.0f, 1.0f});

        // Material* material = new Material(m_Shader);

        for (int i = 0; i < 10; i++) {
            glm::vec4 colour = (((9.0f - (float)i) * m_SpriteColourStart) + ((float)i * m_SpriteColourEnd)) / 4.0f;
            MIST_SHADERLIB->Bind("BasicTexture");
            MIST_SHADER("BasicTexture")->SetUniform4f("u_Colour", colour.r, colour.g, colour.b, colour.a);

            for (int j = 0; j < 10; j++) {
                Renderer::Submit(MIST_SHADER("BasicTexture"), m_VertexArray,
                                 glm::scale(glm::translate(spriteTransform, {1.0f * j, -1.0f * i, 0.0f}),
                                            {glm::pow(0.98f, j + i * 10), glm::pow(0.98f, j + i * 10), 1.0f}));
            }
        }

        Renderer::EndScene();

        // Renderer::Flush();
    }

    void OnImGuiRender(DeltaTime deltaTime) override {
        ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColourStart), 0);
        ImGui::ColorEdit4("End Colour", glm::value_ptr(m_SpriteColourEnd), 0);
    }

    void OnEvent(Event& e) override {
        m_CameraController.OnEvent(e);
    }

private:
    OrthographicCameraController m_CameraController;
    Ref<VertexArray> m_VertexArray;

    float m_CameraMoveSpeed = 300.0f;
    float m_CameraRotation = 0.0f;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};

    float m_SpriteMoveSpeed = 300.0f;
    float m_SpriteRotation = 0.0f;
    glm::vec3 m_SpritePosition = {0.0f, 0.0f, 0.0f};

    glm::vec4 m_SpriteColourStart = {0.2f, 0.8f, 0.8f, 1.0f};
    glm::vec4 m_SpriteColourEnd = {0.8f, 0.0f, 0.6f, 1.0f};
};