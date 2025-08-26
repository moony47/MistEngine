#include "mistpch.h"
#include "Renderer2D.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Mist {

struct Renderer2DData {
    std::string shaderName;
    std::string whiteTextureName;
    Ref<VertexArray> vertexArray;
};

static Renderer2DData* s_Data;

void Renderer2D::Init() {
    s_Data = new Renderer2DData;

    s_Data->vertexArray = VertexArray::Create();

    float vertices[4 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
        +0.5f, -0.5f, 1.0f, 0.0f, // 1
        -0.5f, +0.5f, 0.0f, 1.0f, // 2
        +0.5f, +0.5f, 1.0f, 1.0f  // 3
    };

    uint32_t indices[] = {
        0, 1, 2, // 0
        1, 2, 3  // 1
    };

    auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        {ShaderDataType::Float2,  "a_Position"},
        {ShaderDataType::Float2, "a_TexCoords"}
    });
    s_Data->vertexArray->AddVertexBuffer(vertexBuffer);

    auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    s_Data->vertexArray->SetIndexBuffer(indexBuffer);

    s_Data->shaderName = "2D";
    MIST_SHADERLIB->Create(s_Data->shaderName, "res/shaders/2D.vert", "res/shaders/2D.frag");
    MIST_SHADERLIB->Bind(s_Data->shaderName);

    s_Data->whiteTextureName = "WHITE";
    MIST_TEXTURE2DLIB->Create(s_Data->whiteTextureName, 1, 1)->SetData(new uint32_t(0xFFFFFFFF), sizeof(uint32_t));
}

void Renderer2D::Shutdown() {
    MIST_SHADERLIB->Remove(s_Data->shaderName);
    delete s_Data;
}

void Renderer2D::BeginScene(OrthographicCamera& camera) {
    MIST_SHADERLIB->Bind(s_Data->shaderName);
    MIST_SHADER(s_Data->shaderName)->SetUniformMat4f("u_VP", camera.GetVP());
}

void Renderer2D::EndScene() {
}

void Renderer2D::DrawQuad(const glm::vec3& position,
                          const float angle,
                          const glm::vec2& size,
                          const glm::vec4& colour,
                          const std::string& textureName) {
    MIST_SHADERLIB->Bind(s_Data->shaderName);

    // Set TSR transform matrix
    MIST_SHADER(s_Data->shaderName)
        ->SetUniformMat4f("u_Transform", glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position),
                                                                glm::radians(angle), {0, 0, 1}),
                                                    glm::vec3(size, 1.0f)));

    // Set tint colour
    MIST_SHADER(s_Data->shaderName)->SetUniform4f("u_Colour", colour.r, colour.g, colour.b, colour.a);

    if (textureName.empty()) {
        // Use white texture
        MIST_TEXTURE2DLIB->Bind(s_Data->whiteTextureName);
        MIST_SHADER(s_Data->shaderName)->SetUniformTex2D("u_Texture", MIST_TEXTURE2D(s_Data->whiteTextureName));
    } else {
        // Set texture
        MIST_TEXTURE2DLIB->Bind(textureName);
        MIST_SHADER(s_Data->shaderName)->SetUniformTex2D("u_Texture", MIST_TEXTURE2D(textureName));
    }

    s_Data->vertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->vertexArray);
}

} // namespace Mist