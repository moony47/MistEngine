#include <Mist.h>

#include "ImGui/imgui.h"

using namespace Mist;

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example"),
        m_Camera(0.0f, 0.0f, 0.0f, -640.0f, 640.0f, -360.0f, 360.0f) {
        m_VertexArray = VertexArray::Create();

        float vertices[4 * 4] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            +50.0f, -50.0f, 1.0f, 0.0f, // 1
            -50.0f, +50.0f, 0.0f, 1.0f, // 2
            +50.0f, +50.0f, 1.0f, 1.0f, // 3
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

        SHADERLIB->Create("BasicTexture", "../Mist/res/shaders/Basic.vert", "../Mist/res/shaders/Basic.frag");
        SHADERLIB->Bind("BasicTexture");

        TEXTURE2DLIB->Create("Diamond", "../Mist/res/textures/diamond.png");
        TEXTURE2DLIB->Bind("Diamond", 0);
        SHADERLIB->Get("BasicTexture")->SetUniformTexture2D("u_Texture", TEXTURE2DLIB->Get("Diamond"));

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

        if (Input::IsKeyPressed(MIST_KEY_Q))
            m_SpriteRotation += deltaTime * 3.0f;
        if (Input::IsKeyPressed(MIST_KEY_E))
            m_SpriteRotation -= deltaTime * 3.0f;

        m_Camera.SetRotation(m_CameraRotation);
        m_Camera.SetPosition(m_CameraPosition);

        Renderer::BeginScene(m_Camera /*lights, environment*/);
    }

    void OnUpdateEnd(DeltaTime deltaTime) override {
        glm::mat4 spriteTransform =
            glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), m_SpritePosition), m_SpriteRotation, {0, 0, 1}),
                       {2.0f, 2.0f, 1.0f});

        // Material* material = new Material(m_Shader);

        for (int i = 0; i < 10; i++) {
            glm::vec4 colour = (((9.0f - (float)i) * m_SpriteColourStart) + ((float)i * m_SpriteColourEnd)) / 4.0f;
            SHADERLIB->Get("BasicTexture")->SetUniform4f("u_Colour", colour.r, colour.g, colour.b, colour.a);

            for (int j = 0; j < 10; j++) {
                Renderer::Submit(SHADERLIB->Get("BasicTexture"), m_VertexArray,
                                 glm::scale(glm::translate(spriteTransform, {90.0f * j, -90.0f * i, 0.0f}),
                                            {glm::pow(0.98f, j + i * 10), glm::pow(0.98f, j + i * 10), 1.0f}));
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
        // switch (e.GetKeyCode()) {
        //     case MIST_KEY_1:
        //         m_CameraRotation = 0.0f;
        //         break;
        //     case MIST_KEY_2:
        //         m_CameraRotation = 90.0f;
        //         break;
        //     case MIST_KEY_3:
        //         m_CameraRotation = 180.0f;
        //         break;
        //     case MIST_KEY_4:
        //         m_CameraRotation = 270.0f;
        //         break;
        // }
        return false;
    }

private:
    OrthographicCamera m_Camera;
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
