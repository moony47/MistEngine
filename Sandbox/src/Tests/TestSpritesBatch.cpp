#include "TestSpritesBatch.h"

#include <ppl.h>

const size_t numSprites = 2048;

TestSpritesBatch::TestSpritesBatch() :
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.SetZoomLevel(3.0f);

    m_VertexBuffer = std::make_unique<float[]>(numSprites * 4 * 9);
    m_IndexBuffer = std::make_unique<unsigned int[]>(numSprites * 6);

    m_Sprites.reserve(numSprites);

    float tex, Vx, Vy, Cr, Cg, Cb;
    for (size_t i = 0; i < numSprites; i++) {
        // Define sprites with random spacial and chromatic velocities and texture index
        tex = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Vx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 4.0f - 2.0f;
        Vy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 4.0f - 2.0f;
        Cr = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
        Cg = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;
        Cb = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.5f + 0.5f;

        m_Sprites.emplace_back(tex > 0.5f, glm::vec4(Cr, Cg, Cb, 0.0f), glm::vec3(Vx, Vy, 0.0f),
                               &m_VertexBuffer[i * 4 * 9], &m_IndexBuffer[i * 6]);
    }

    // Define VertexBuffer layout
    m_VBL.reset(new BufferLayout({
        {ShaderDataType::Float2,     "a_Position"},
        {ShaderDataType::Float2,    "a_TexCoords"},
        {ShaderDataType::Float4,       "a_Colour"},
        { ShaderDataType::Float, "a_SamplerIndex"},
    }));

    // Compile shader and push onto device
    MIST_SHADERLIB->Create("Batch", "assets/shaders/Batch.vert", "assets/shaders/Batch.frag");
    MIST_SHADERLIB->Bind("Batch");

    // Push textures onto device
    MIST_TEXLIB->Create("Diamond", "assets/textures/diamond.png");
    MIST_TEXLIB->Create("Star", "assets/textures/star.png");
    MIST_TEX("Diamond")->Bind(0);
    MIST_TEX("Star")->Bind(1);

    // Attach textures to shader
    int samplers[] = {0, 1};
    MIST_SHADER("Batch")->SetUniform1iv("u_Texture", 2, samplers);

    MIST_SHADERLIB->Unbind();

    RenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void TestSpritesBatch::OnDetach() {
    MIST_PROFILE_FUNCTION();

    MIST_SHADERLIB->Remove("Batch");
    MIST_TEXLIB->Remove("Diamond");
    MIST_TEXLIB->Remove("Star");
}

void TestSpritesBatch::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(deltaTime);

    // Update location and colour of sprites.
    // Push their vertices and indices into buffers
    //concurrency::parallel_for(size_t(0), m_Sprites.size(), [&](size_t i) {
    //    m_Sprites[i].Update(deltaTime, -5.0f, -5.0f, 5.0f, 5.0f, (unsigned int)i);
    //});
     for (size_t i = 0; i < m_Sprites.size(); i++)
         m_Sprites[i].Update(deltaTime, -5.0f, -5.0f, 5.0f, 5.0f, (unsigned int)i);

    // Create new VertexArray
    m_VA = VertexArray::Create();

    // Create and attach VertexBuffer and IndexBuffer
    Ref<VertexBuffer> vb;
    vb = VertexBuffer::Create(m_VertexBuffer.get(), numSprites * 4 * 9 * sizeof(float));
    Ref<IndexBuffer> ib;
    ib = IndexBuffer::Create(m_IndexBuffer.get(), numSprites * 6);

    vb->SetLayout(*m_VBL);
    m_VA->AddVertexBuffer(vb);
    m_VA->SetIndexBuffer(ib);

    // Unbind everything before vb and ib are deleted
    m_VA->Unbind();
    vb->Unbind();
    ib->Unbind();
}

void TestSpritesBatch::OnFrameStart(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Renderer::BeginScene(m_CameraController.GetCamera() /*lights, environment*/);
}

void TestSpritesBatch::OnFrameEnd(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Renderer::Submit(MIST_SHADER("Batch"), m_VA, glm::mat4(1.0));

    // Delete VertexArray ready for next frame
    m_VA.reset();
}

void TestSpritesBatch::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);
}
