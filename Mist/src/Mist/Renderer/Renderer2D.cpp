#include "mistpch.h"
#include "Renderer2D.h"

#include "Mist/Cameras/Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "VertexArray.h"

namespace Mist {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Colour;
    glm::vec2 TexCoord;
    float TilingFactor;
    int TexIndex;
    int EntityID = -1;

    static BufferLayout Layout;
};

BufferLayout QuadVertex::Layout = {
    {ShaderDataType::Float3,     "a_Position"},
    {ShaderDataType::Float4,       "a_Colour"},
    {ShaderDataType::Float2,    "a_TexCoords"},
    { ShaderDataType::Float, "a_TilingFactor"},
    {   ShaderDataType::Int,     "a_TexIndex"},
    {   ShaderDataType::Int,     "a_EntityID"}
};

struct CircleVertex {
    glm::vec3 WorldPosition;
    glm::vec3 LocalPosition;
    glm::vec4 Colour;
    float Thickness;
    float Fade;
    int EntityID = -1;

    static BufferLayout Layout;
};

BufferLayout CircleVertex::Layout = {
    {ShaderDataType::Float3, "a_WorldPosition"},
    {ShaderDataType::Float3, "a_LocalPosition"},
    {ShaderDataType::Float4,        "a_Colour"},
    { ShaderDataType::Float,     "a_Thickness"},
    { ShaderDataType::Float,          "a_Fade"},
    {   ShaderDataType::Int,      "a_EntityID"}
};

struct Renderer2DData {
    const static uint32_t MaxQuads = 20000;
    const static uint32_t MaxVertices = MaxQuads * 4;
    const static uint32_t MaxIndices = MaxQuads * 6;

    const std::string QuadShaderName = "Quad";
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    const std::string CircleShaderName = "Circle";
    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;

    uint32_t CircleIndexCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    const std::string WhiteTexName = "None";
    int TextureIndex = 0;
    std::array<std::string, 32> TextureSlots;

    Renderer2D::Statistics Stats;

    struct CameraData {
        glm::mat4 ViewProjection;
    };
    CameraData CameraBuffer;

    Ref<UniformBuffer> CameraUniformBuffer;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
    MIST_PROFILE_FUNCTION();

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

    // Allocate device memory block for vertex data
    s_Data.QuadVertexArray = VertexArray::Create();
    s_Data.QuadVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->SetLayout(QuadVertex::Layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
    s_Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, Renderer2DData::MaxIndices));

    s_Data.CircleVertexArray = VertexArray::Create();
    s_Data.CircleVertexBuffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(CircleVertex));
    s_Data.CircleVertexBuffer->SetLayout(CircleVertex::Layout);
    s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
    s_Data.CircleVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, Renderer2DData::MaxIndices));

    delete[] indices; // No need to keep the indices on the host

    // Allocate host memory block for vertex data
    s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];
    s_Data.CircleVertexBufferBase = new CircleVertex[Renderer2DData::MaxVertices];

    // Create 2D batch shader
    MIST_SHADERLIB->Create(s_Data.QuadShaderName);
    MIST_SHADERLIB->Create(s_Data.CircleShaderName);

    // Create the white pixel texture for solid colours
    MIST_TEXLIB->Create(s_Data.WhiteTexName, 1, 1)->SetData(new uint32_t(0xFFFFFFFF), sizeof(uint32_t));

    s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D::Shutdown() {
    MIST_PROFILE_FUNCTION();

    MIST_SHADERLIB->Remove(s_Data.QuadShaderName);
    MIST_SHADERLIB->Remove(s_Data.CircleShaderName);
    MIST_TEXLIB->Remove(s_Data.WhiteTexName);

    delete[] s_Data.QuadVertexBufferBase;
    delete[] s_Data.CircleVertexBufferBase;
}

void Renderer2D::BeginView(const glm::mat4& projection, const glm::mat4& transform) {
    MIST_PROFILE_FUNCTION();

    // Set the camera transform for this scene
    // MIST_SHADERLIB->Bind(s_Data.ShaderName);
    // glm::mat4 VP = projection * glm::inverse(transform);
    // MIST_SHADER(s_Data.ShaderName)->SetUniformMat4f("u_VP", VP);

    s_Data.CameraBuffer.ViewProjection = projection * glm::inverse(transform);
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

    BeginBatch();
}

void Renderer2D::BeginView(OrthographicCamera& camera) {
    MIST_PROFILE_FUNCTION();

    // Set the camera transform for this scene
    s_Data.CameraBuffer.ViewProjection = camera.GetVP();
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

    BeginBatch();
}

void Renderer2D::BeginView(EditorCamera& camera) {
    MIST_PROFILE_FUNCTION();

    // Set the camera transform for this scene
    s_Data.CameraBuffer.ViewProjection = camera.GetViewProj();
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

    BeginBatch();
}

void Renderer2D::EndView() {
    MIST_PROFILE_FUNCTION();

    FlushQuads();
    FlushCircles();
}

void Renderer2D::BeginBatch() {
    BeginQuads();
    BeginCircles();
}

void Renderer2D::BeginQuads() {
    // Reset indices count vertex data buffer
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    // Reset texture requirements
    for (uint32_t i = 0; i < s_Data.TextureSlots.size(); i++)
        s_Data.TextureSlots[i] = "";
    s_Data.TextureIndex = 0;
}

void Renderer2D::BeginCircles() {
    s_Data.CircleIndexCount = 0;
    s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
}

void Renderer2D::FlushQuads() {
    if (!s_Data.QuadIndexCount)
        return;

    // Upload the vertex data block to device
    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->UpdateBuffer(s_Data.QuadVertexBufferBase, dataSize);

    // Bind 2D batch shader and all the required textures
    MIST_SHADERLIB->Bind(s_Data.QuadShaderName);
    for (uint32_t i = 0; (int)i < s_Data.TextureIndex; i++)
        MIST_TEXLIB->Bind(s_Data.TextureSlots[i], i);

    // Draw call for entire batch
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer2D::FlushCircles() {
    if (!s_Data.CircleIndexCount)
        return;

    // Upload the vertex data block to device
    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
    s_Data.CircleVertexBuffer->UpdateBuffer(s_Data.CircleVertexBufferBase, dataSize);

    // Bind 2D batch shader and all the required textures
    MIST_SHADERLIB->Bind(s_Data.CircleShaderName);
    // for (uint32_t i = 0; (int)i < s_Data.TextureIndex; i++)
    //     MIST_TEXLIB->Bind(s_Data.TextureSlots[i], i);

    // Draw call for entire batch
    RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const QuadDrawArgs&& drawArgs) {
    MIST_PROFILE_FUNCTION();

    constexpr glm::vec4 QuadGeoCorners[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    };

    // If batch is already drawing maximum number of quads, flush and start new batch
    if (s_Data.QuadIndexCount == Renderer2DData::MaxIndices) {
        FlushQuads();
        BeginQuads();
    }

    std::string rootTextureName = MIST_TEXLIB->GetRootTexture(drawArgs.TextureName);

    // If texture is already bound to a slot, reference the same slot again
    int textureIndex = -1;
    for (int i = 0; (size_t)i < s_Data.TextureIndex; i++)
        if (s_Data.TextureSlots[i] == rootTextureName) {
            textureIndex = i;
            break;
        }

    // Otherwise, add the texture to the next available slot and reference that slot
    if (textureIndex == -1) {
        // If batch is already using all available texture slots, flush and start new batch
        if (s_Data.TextureIndex == s_Data.TextureSlots.size()) {
            FlushQuads();
            BeginQuads();
        }
        textureIndex = (int)s_Data.TextureIndex;
        s_Data.TextureSlots[s_Data.TextureIndex++] = rootTextureName;
    }

    Ref<Texture2D> texture = MIST_TEX(drawArgs.TextureName);

    // Populate the vertex data of the quad's 4 vertices
    for (size_t i = 0; i < 4; i++) {
        s_Data.QuadVertexBufferPtr->Position = drawArgs.Transform * QuadGeoCorners[i];
        s_Data.QuadVertexBufferPtr->Colour = drawArgs.Colour;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords(i);
        s_Data.QuadVertexBufferPtr->TilingFactor = drawArgs.TilingFactor;
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->EntityID = drawArgs.EntityID;
        s_Data.QuadVertexBufferPtr++;
    }

    // Count 6 indices per quad
    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawCircle(const CircleDrawArgs&& drawArgs) {
    MIST_PROFILE_FUNCTION();

    constexpr glm::vec4 CircleGeoCorners[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    };

    // If batch is already drawing maximum number of quads, flush and start new batch
    if (s_Data.CircleIndexCount == Renderer2DData::MaxIndices) {
        FlushCircles();
        BeginCircles();
    }

    //// If texture is already bound to a slot, reference the same slot again
    // int textureIndex = -1;
    // for (int i = 0; (size_t)i < s_Data.TextureIndex; i++)
    //     if (s_Data.TextureSlots[i] == "None") {
    //         textureIndex = i;
    //         break;
    //     }

    //// Otherwise, add the texture to the next available slot and reference that slot
    // if (textureIndex == -1) {
    //     // If batch is already using all available texture slots, flush and start new batch
    //     if (s_Data.TextureIndex == s_Data.TextureSlots.size()) {
    //         FlushBatch();
    //         BeginBatch();
    //     }
    //     textureIndex = (int)s_Data.TextureIndex;
    //     s_Data.TextureSlots[s_Data.TextureIndex++] = "None";
    // }

    // Ref<Texture2D> texture = MIST_TEX(drawArgs.TextureName);

    // Populate the vertex data of the quad's 4 vertices
    for (size_t i = 0; i < 4; i++) {
        s_Data.CircleVertexBufferPtr->WorldPosition = drawArgs.Transform * CircleGeoCorners[i];
        s_Data.CircleVertexBufferPtr->LocalPosition = CircleGeoCorners[i] * 2.0f;
        s_Data.CircleVertexBufferPtr->Colour = drawArgs.Colour;
        s_Data.CircleVertexBufferPtr->Thickness = drawArgs.Thickness;
        s_Data.CircleVertexBufferPtr->Fade = drawArgs.Fade;
        s_Data.CircleVertexBufferPtr->EntityID = drawArgs.EntityID;
        s_Data.CircleVertexBufferPtr++;
    }

    // Count 6 indices per circle
    s_Data.CircleIndexCount += 6;

    s_Data.Stats.CircleCount++;
}

Renderer2D::Statistics Renderer2D::GetStats() {
    return s_Data.Stats;
}

void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(Statistics));
}

} // namespace Mist