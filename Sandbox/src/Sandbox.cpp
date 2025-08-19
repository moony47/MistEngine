#include <Mist.h>

#include "ImGui/imgui.h"

using namespace Mist;

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
        m_VertexArray.reset(VertexArray::Create());

        float vertices[4 * 4] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            +50.0f, -50.0f, 1.0f, 0.0f, // 1
            -50.0f, +50.0f, 0.0f, 1.0f, // 2
            +50.0f, +50.0f, 1.0f, 1.0f, // 3
        };

        uint32_t indices[] = {0, 1, 2, 1, 2, 3};

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout({
            {ShaderDataType::Float2,  "a_Position"},
            {ShaderDataType::Float2, "a_TexCoords"}
        });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_Shader.reset(ShaderController::GetInstance()->CreateShader("../Mist/res/shaders/Basic.vert",
                                                                     "../Mist/res/shaders/Basic.frag"));
        m_Shader->Bind();

        m_Texture.reset(ShaderController::GetInstance()->CreateTexture("../Mist/res/textures/diamond.png"));
        m_Texture->Bind(0);
        m_Shader->SetUniform1i(m_Shader->GetUniformLocation("u_Texture"), 0);

        m_Camera = std::make_shared<OrthographicCamera>(0.0f, 0.0f, 0.0f, -640.0f, 640.0f, -360.0f, 360.0f);

        RenderCommand::SetClearColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void OnUpdateStart(DeltaTime deltaTime) override {
        if (Input::IsKeyPressed(MIST_KEY_D))
            m_CameraPosition.x += deltaTime * m_CameraMoveSpeed;
        if (Input::IsKeyPressed(MIST_KEY_A))
            m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
        if (Input::IsKeyPressed(MIST_KEY_W))
            m_CameraPosition.y += deltaTime * m_CameraMoveSpeed;
        if (Input::IsKeyPressed(MIST_KEY_S))
            m_CameraPosition.y -= deltaTime * m_CameraMoveSpeed;

        //if (Input::IsKeyPressed(MIST_KEY_A))
        //    m_SpritePosition.x -= deltaTime * m_SpriteMoveSpeed;
        //if (Input::IsKeyPressed(MIST_KEY_D))
        //    m_SpritePosition.x += deltaTime * m_SpriteMoveSpeed;
        //if (Input::IsKeyPressed(MIST_KEY_W))
        //    m_SpritePosition.y += deltaTime * m_SpriteMoveSpeed;
        //if (Input::IsKeyPressed(MIST_KEY_S))
        //    m_SpritePosition.y -= deltaTime * m_SpriteMoveSpeed;

        if (Input::IsKeyPressed(MIST_KEY_Q))
            m_SpriteRotation += deltaTime * 3.0f;
        if (Input::IsKeyPressed(MIST_KEY_E))
            m_SpriteRotation -= deltaTime * 3.0f;

        m_Camera->SetRotation(m_CameraRotation);
        m_Camera->SetPosition(m_CameraPosition);

        Renderer::BeginScene(*m_Camera /*lights, environment*/);
    }

    void OnUpdateEnd(DeltaTime deltaTime) override {
        glm::mat4 spriteTransform =
            glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), m_SpritePosition), m_SpriteRotation, {0, 0, 1}),
                       {2.0f, 2.0f, 1.0f});

        Material* material = new Material(m_Shader);

        for (int i = 0; i < 5; i++) {
            glm::vec4 colour = (((4.0f - (float)i) * m_SpriteColourStart) + ((float)i * m_SpriteColourEnd)) / 4.0f;
            m_Shader->SetUniform4f(m_Shader->GetUniformLocation("u_Colour"), colour.r, colour.g, colour.b, colour.a);
            for (int j = 0; j < 5; j++) {
                Renderer::Submit(m_Shader, m_VertexArray,
                                 glm::scale(glm::translate(spriteTransform, {90.0f * j, -90.0f * i, 0.0f}),
                                            {glm::pow(0.95f, j + i * 5), glm::pow(0.95f, j + i * 5), 1.0f}));
            }
        }

        Renderer::EndScene();

        // Renderer::Flush();
    }

    void OnImGuiRender(DeltaTime deltaTime) override {
        ImGui::ColorEdit4("Start Colour", &m_SpriteColourStart[0], 0);
        ImGui::ColorEdit4("End Colour", &m_SpriteColourEnd[0], 0);
    }

    void OnEvent(Mist::Event& e) override {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(MIST_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

private:
    bool OnKeyPressedEvent(KeyPressedEvent& e) {
        //switch (e.GetKeyCode()) {
        //    case MIST_KEY_1:
        //        m_CameraRotation = 0.0f;
        //        break;
        //    case MIST_KEY_2:
        //        m_CameraRotation = 90.0f;
        //        break;
        //    case MIST_KEY_3:
        //        m_CameraRotation = 180.0f;
        //        break;
        //    case MIST_KEY_4:
        //        m_CameraRotation = 270.0f;
        //        break;
        //}
        return false;
    }

private:
    std::shared_ptr<OrthographicCamera> m_Camera;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture2D> m_Texture;
    std::shared_ptr<VertexArray> m_VertexArray;

    float m_CameraMoveSpeed = 300.0f;
    float m_CameraRotation = 0.0f;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};

    float m_SpriteMoveSpeed = 300.0f;
    float m_SpriteRotation = 0.0f;
    glm::vec3 m_SpritePosition = {0.0f, 0.0f, 0.0f};

    glm::vec4 m_SpriteColourStart = {0.2f, 0.8f, 0.8f, 1.0f};
    glm::vec4 m_SpriteColourEnd = {0.8f, 0.0f, 0.6f, 1.0f};
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
