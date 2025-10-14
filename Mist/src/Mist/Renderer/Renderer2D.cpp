#include "mistpch.h"
#include "Renderer2D.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Mist {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Colour;
    glm::vec2 TexCoord;
    int TexIndex;
    float TilingFactor;

    static BufferLayout Layout;
};

BufferLayout QuadVertex::Layout = {
    {ShaderDataType::Float3,     "a_Position"},
    {ShaderDataType::Float4,       "a_Colour"},
    {ShaderDataType::Float2,    "a_TexCoords"},
    {   ShaderDataType::Int,     "a_TexIndex"},
    { ShaderDataType::Float, "a_TilingFactor"}
};

struct Renderer2DData {
    const static uint32_t MaxQuads = 20000;
    const static uint32_t MaxVertices = MaxQuads * 4;
    const static uint32_t MaxIndices = MaxQuads * 6;

    const std::string ShaderName = "2D";
    const std::string WhiteTexName = "WHITE";

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    size_t TextureIndex = 0;
    std::array<std::string, 32> TextureSlots;

    Renderer2D::Statistics Stats;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
    MIST_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = VertexArray::Create();

    // Allocate device memory block for vertex data
    s_Data.QuadVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->SetLayout(QuadVertex::Layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    // Allocate and populate the index buffer on device
    uint32_t* indices = new uint32_t[Renderer2DData::MaxIndices];
    for (auto [i, offset] = std::pair<size_t, uint32_t>{0, 0}; i < Renderer2DData::MaxIndices; i += 6, offset += 4) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
    }
    auto indexBuffer = IndexBuffer::Create(indices, Renderer2DData::MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
    delete[] indices; // No need to keep the indices on the host

    // Allocate host memory block for vertex data
    s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

    // Create 2D batch shader
    MIST_SHADERLIB->Create(s_Data.ShaderName, "res/shaders/2D.vert", "res/shaders/2D.frag")->Bind();

    // Set the sampler uniforms for each texture slot
    int samplers[32];
    for (int i = 0; i < s_Data.TextureSlots.size(); i++)
        samplers[i] = i;
    MIST_SHADER(s_Data.ShaderName)->SetUniform1iv("u_Texture", s_Data.TextureSlots.size(), samplers);

    // Create the white pixel texture for solid colours
    MIST_TEXLIB->Create(s_Data.WhiteTexName, 1, 1)->SetData(new uint32_t(0xFFFFFFFF), sizeof(uint32_t));
}

void Renderer2D::Shutdown() {
    MIST_PROFILE_FUNCTION();

    MIST_SHADERLIB->Remove(s_Data.ShaderName);
    MIST_TEXLIB->Remove(s_Data.WhiteTexName);

    delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(OrthographicCamera& camera) {
    MIST_PROFILE_FUNCTION();

    // Set the camera transform for this scene
    MIST_SHADERLIB->Bind(s_Data.ShaderName);
    MIST_SHADER(s_Data.ShaderName)->SetUniformMat4f("u_VP", camera.GetVP());

    BeginBatch();
}

void Renderer2D::EndScene() {
    MIST_PROFILE_FUNCTION();

    FlushBatch();
}

void Renderer2D::BeginBatch() {
    // Reset indices count vertex data buffer
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    // Reset texture requirements
    for (uint32_t i = 0; i < s_Data.TextureSlots.size(); i++)
        s_Data.TextureSlots[i] = "";
    s_Data.TextureIndex = 0;
}

void Renderer2D::FlushBatch() {
    // Upload the vertex data block to device
    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->UpdateBuffer(s_Data.QuadVertexBufferBase, dataSize);

    // Bind 2D batch shader and all the required textures
    MIST_SHADERLIB->Bind(s_Data.ShaderName);
    for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        MIST_TEXLIB->Bind(s_Data.TextureSlots[i], i);

    // Draw call for entire batch
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec3& position,
                          const float angleRad,
                          const glm::vec2& size,
                          const glm::vec4& colour,
                          const std::string& textureName,
                          float tilingFactor) {
    MIST_PROFILE_FUNCTION();

    constexpr glm::vec4 QuadGeoCorners[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    };

    // If batch is already drawing maximum number of quads, flush and start new batch
    if (s_Data.QuadIndexCount == Renderer2DData::MaxIndices) {
        FlushBatch();
        BeginBatch();
    }

    std::string sourceTextureName = MIST_TEXLIB->GetSourceTexture(textureName);

    // If texture is already bound to a slot, reference the same slot again
    int textureIndex = -1;
    for (size_t i = 0; i < s_Data.TextureIndex; i++)
        if (s_Data.TextureSlots[i] == sourceTextureName) {
            textureIndex = i;
            break;
        }

    // Otherwise, add the texture to the next available slot and reference that slot
    if (textureIndex == -1) {
        // If batch is already using all available texture slots, flush and start new batch
        if (s_Data.TextureIndex == s_Data.TextureSlots.size()) {
            FlushBatch();
            BeginBatch();
        }
        textureIndex = s_Data.TextureIndex;
        s_Data.TextureSlots[s_Data.TextureIndex++] = sourceTextureName;
    }

    // Compute transform for the quad geometry
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::rotate(transform, angleRad, {0, 0, 1});
    transform = glm::scale(transform, glm::vec3(size, 1.0f));

    Ref<Texture2D> texture = MIST_TEX(textureName);

    // Populate the vertex data of the quad's 4 vertices
    for (size_t i = 0; i < 4; i++) {
        s_Data.QuadVertexBufferPtr->Position = transform * QuadGeoCorners[i];
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords(i);
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    // Count 6 indices per quad
    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

Renderer2D::Statistics Renderer2D::GetStats() {
    return s_Data.Stats;
}

void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(Statistics));
}

} // namespace Mist