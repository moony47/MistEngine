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
};

struct Renderer2DData {
    const uint32_t MaxQuads = 20000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6;

    const std::string ShaderName = "2D";
    const std::string WhiteTexName = "WHITE";

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    size_t TextureIndex = 1;
    std::array<std::string, 32> TextureSlots;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
    PROFILE_FUNCTION();

    s_Data.QuadVertexArray = VertexArray::Create();

    {
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3,     "a_Position"},
            {ShaderDataType::Float4,       "a_Colour"},
            {ShaderDataType::Float2,    "a_TexCoords"},
            {   ShaderDataType::Int,     "a_TexIndex"},
            { ShaderDataType::Float, "a_TilingFactor"}
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
    }

    {
        uint32_t* indices = new uint32_t[s_Data.MaxIndices];
        for (auto [i, offset] = std::pair<size_t, uint32_t>{0, 0}; i < s_Data.MaxIndices; i += 6, offset += 4) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
        }

        auto indexBuffer = IndexBuffer::Create(indices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
        delete[] indices;
    }

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    int samplers[32];
    for (int i = 0; i < s_Data.TextureSlots.size(); i++)
        samplers[i] = i;

    MIST_SHADERLIB->Create(s_Data.ShaderName, "res/shaders/2D.vert", "res/shaders/2D.frag")->Bind();
    MIST_SHADER(s_Data.ShaderName)->SetUniform1iv("u_Texture", s_Data.TextureSlots.size(), samplers);

    MIST_TEXTURE2DLIB->Create(s_Data.WhiteTexName, 1, 1)->SetData(new uint32_t(0xFFFFFFFF), sizeof(uint32_t));
}

void Renderer2D::Shutdown() {
    PROFILE_FUNCTION();

    MIST_SHADERLIB->Remove(s_Data.ShaderName);
    MIST_TEXTURE2DLIB->Remove(s_Data.WhiteTexName);

    delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(OrthographicCamera& camera) {
    PROFILE_FUNCTION();

    MIST_SHADERLIB->Bind(s_Data.ShaderName);
    MIST_SHADER(s_Data.ShaderName)->SetUniformMat4f("u_VP", camera.GetVP());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    for (uint32_t i = 0; i < s_Data.TextureSlots.size(); i++)
        s_Data.TextureSlots[i] = "";
    s_Data.TextureIndex = 0;
}

void Renderer2D::EndScene() {
    PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::Flush() {
    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->UpdateBuffer(s_Data.QuadVertexBufferBase, dataSize);

    MIST_SHADERLIB->Bind(s_Data.ShaderName);
    for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        MIST_TEXTURE2DLIB->Bind(s_Data.TextureSlots[i], i);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
}

void Renderer2D::DrawQuad(const glm::vec3& position,
                          const float angle,
                          const glm::vec2& size,
                          const glm::vec4& colour,
                          const std::string& textureName,
                          float tilingFactor) {
    PROFILE_FUNCTION();

    // If texture is already bound to a slot, reference the same slot again
    int textureIndex = -1;
    for (size_t i = 0; i < s_Data.TextureIndex; i++)
        if (s_Data.TextureSlots[i] == textureName) {
            textureIndex = i;
            break;
        }

    // Otherwise, add the texture to the next available slot and reference that slot
    if (textureIndex == -1) {
        textureIndex = s_Data.TextureIndex;
        s_Data.TextureSlots[s_Data.TextureIndex++] = textureName;
    }

    static const glm::vec2 corners[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    // Populate the vertex data of the quad's 4 vertices
    for (size_t i = 0; i < 4; i++) {
        s_Data.QuadVertexBufferPtr->Position = position + glm::vec3(corners[i] * size, 0);
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = corners[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    // Count 6 indices per quad
    s_Data.QuadIndexCount += 6;

    // MIST_SHADERLIB->Bind(s_Data.shaderName);

    // Set Tiling Factor
    // MIST_SHADER(s_Data.shaderName)->SetUniform1f("u_TilingFactor", tilingFactor);

    //// Set TSR transform matrix
    // glm::mat4 transform = glm::mat4(1.0f);
    // transform = glm::translate(transform, position);
    // transform = glm::rotate(transform, glm::radians(angle), {0, 0, 1});
    // transform = glm::scale(transform, glm::vec3(size, 1.0f));
    // MIST_SHADER(s_Data.shaderName)->SetUniformMat4f("u_Transform", transform);

    //// Set tint colour
    // MIST_SHADER(s_Data.shaderName)->SetUniform4f("u_Colour", colour.r, colour.g, colour.b, colour.a);

    // if (textureName.empty()) {
    //     // Use white texture
    //     MIST_TEXTURE2DLIB->Bind(s_Data.whiteTextureName);
    //     MIST_SHADER(s_Data.shaderName)->SetUniformTex2D("u_Texture", MIST_TEXTURE2D(s_Data.whiteTextureName));
    // } else {
    //     // Set texture
    //     MIST_TEXTURE2DLIB->Bind(textureName);
    //     MIST_SHADER(s_Data.shaderName)->SetUniformTex2D("u_Texture", MIST_TEXTURE2D(textureName));
    // }

    // s_Data.vertexArray->Bind();
    // RenderCommand::DrawIndexed(s_Data.vertexArray);
}

} // namespace Mist