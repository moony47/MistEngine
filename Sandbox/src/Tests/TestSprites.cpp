#include "TestSprites.h"

TestSprites::TestSprites() :
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
    const int numSprites = 2048;

    m_StarSprites.reserve(numSprites / 2);
    m_DiamondSprites.reserve(numSprites / 2);

    float tex, Vx, Vy, Cr, Cg, Cb;
    for (int i = 0; i < numSprites; i++) {
        tex = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Vx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 4.0f - 2.0f;
        Vy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 4.0f - 2.0f;
        Cr = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
        Cg = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
        Cb = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
        if (tex > 0.5f)
            m_StarSprites.emplace_back(true, glm::vec4(0.3f, 0.3f, 0.8f, 1.0f), glm::vec4(Cr, Cg, Cb, 0.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(Vx, Vy, 0.0f));
        else
            m_DiamondSprites.emplace_back(false, glm::vec4(0.3f, 0.3f, 0.8f, 1.0f), glm::vec4(Cr, Cg, Cb, 0.0f),
                                          glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(Vx, Vy, 0.0f));
    }

    m_VA = VertexArray::Create();

    Ref<VertexBuffer> vb;
    vb = VertexBuffer::Create(singleQuadVertices, 16 * sizeof(float));
    BufferLayout vbl = {
        {ShaderDataType::Float2,  "a_Position"},
        {ShaderDataType::Float2, "a_TexCoords"}
    };
    vb->SetLayout(vbl);
    m_VA->AddVertexBuffer(vb);

    Ref<IndexBuffer> ib;
    ib = IndexBuffer::Create(singleQuadIndices, 6);
    m_VA->SetIndexBuffer(ib);

    MIST_SHADERLIB->Create("Basic", "res/shaders/Basic.vert", "res/shaders/Basic.frag");
    MIST_SHADERLIB->Bind("Basic");

    // Add textures
    MIST_TEXTURE2DLIB->Create("Diamond", "res/textures/diamond.png");
    MIST_TEXTURE2DLIB->Create("Star", "res/textures/star.png");
    MIST_TEXTURE2D("Diamond")->Bind(0);
    MIST_TEXTURE2D("Star")->Bind(1);

    vb->Unbind();
    m_VA->Unbind();
    MIST_SHADERLIB->Unbind();
}

void TestSprites::OnUpdate(DeltaTime deltaTime) {
    m_CameraController.OnUpdate(deltaTime);

    for (auto& sprite : m_StarSprites)
        sprite.Update(deltaTime, -5.0f, -5.0f, 5.0f, 5.0f);
    for (auto& sprite : m_DiamondSprites)
        sprite.Update(deltaTime, -5.0f, -5.0f, 5.0f, 5.0f);
}

void TestSprites::OnFrameStart(DeltaTime deltaTime) {
    Renderer::BeginScene(m_CameraController.GetCamera() /*lights, environment*/);
}

void TestSprites::OnFrameEnd(DeltaTime deltaTime) {
    glm::mat4 model(1.0f);

    MIST_SHADERLIB->Bind("Basic");

    MIST_SHADER("Basic")->SetUniform1i("u_Texture", 0);
    for (auto& sprite : m_DiamondSprites) {
        model[3][0] = sprite.Position.x;
        model[3][1] = sprite.Position.y;

        MIST_SHADER("Basic")->SetUniform4f("u_Colour", sprite.Colour.r, sprite.Colour.g, sprite.Colour.b,
                                           sprite.Colour.a);

        Renderer::Submit(MIST_SHADER("Basic"), m_VA, model);
    }

    MIST_SHADER("Basic")->SetUniform1i("u_Texture", 1);
    for (auto& sprite : m_StarSprites) {
        model[3][0] = sprite.Position.x;
        model[3][1] = sprite.Position.y;

        MIST_SHADER("Basic")->SetUniform4f("u_Colour", sprite.Colour.r, sprite.Colour.g, sprite.Colour.b,
                                           sprite.Colour.a);

        Renderer::Submit(MIST_SHADER("Basic"), m_VA, model);
    }
}

void TestSprites::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);
}