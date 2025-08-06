#pragma once

#include "Test.h"

#include <memory>
#include <vector>

class ShaderController;
class Sprite;
class VertexArray;
class VertexBufferLayout;
class IndexBuffer;
class Shader;
class Texture;

const static float batchQuadVertices[] = {
    -50.0f, -50.0f, 0.0f, 0.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f, 50.0f,  -50.0f, 1.0f, 0.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f,
    50.0f,  50.0f,  1.0f, 1.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f, -50.0f, 50.0f,  0.0f, 1.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f};

const unsigned int batchQuadIndices[] = {0, 1, 2, 2, 3, 0};

namespace Testing {

struct Quad {
    bool star;
    glm::vec4 ColourVelocity;
    glm::vec3 Velocity;
    float* vertexBuffer;
    unsigned int* indexBuffer;

    bool init = false;

    void Update(double deltaTime, float width, float height, unsigned int spriteIndex) {
        ColourVelocity.r = Bounce(vertexBuffer[4]) * ColourVelocity.r;
        ColourVelocity.g = Bounce(vertexBuffer[5]) * ColourVelocity.g;
        ColourVelocity.b = Bounce(vertexBuffer[6]) * ColourVelocity.b;

        Velocity.x = Bounce(vertexBuffer[0] - batchQuadVertices[0], width, 50.0f) * Velocity.x;
        Velocity.y = Bounce(vertexBuffer[1] - batchQuadVertices[1], height, 50.0f) * Velocity.y;

        if (!init) {
            // Set buffer data which won't change
            init = true;
            for (int i = 0; i < 4; i++) {
                vertexBuffer[i * 9 + 0] = width / 2.0f + batchQuadVertices[i * 9 + 0];
                vertexBuffer[i * 9 + 1] = height / 2.0f + batchQuadVertices[i * 9 + 1];
                vertexBuffer[i * 9 + 2] = batchQuadVertices[i * 9 + 2];
                vertexBuffer[i * 9 + 3] = batchQuadVertices[i * 9 + 3];
                vertexBuffer[i * 9 + 4] = 0.5f;
                vertexBuffer[i * 9 + 5] = 0.5f;
                vertexBuffer[i * 9 + 6] = 0.5f;
                vertexBuffer[i * 9 + 7] = 1.0f;
                vertexBuffer[i * 9 + 8] = (float)star;
            }
            for (int i = 0; i < 6; i++)
                indexBuffer[i] = batchQuadIndices[i] + spriteIndex * 4;
        } else {
            // Update buffer data which does change
            for (int i = 0; i < 4; i++) {
                vertexBuffer[i * 9 + 0] =
                    glm::clamp(vertexBuffer[i * 9 + 0] + (float)deltaTime * Velocity.x,
                               50.0f + batchQuadVertices[i * 9 + 0], width - 50.0f + batchQuadVertices[i * 9 + 0]);
                vertexBuffer[i * 9 + 1] =
                    glm::clamp(vertexBuffer[i * 9 + 1] + (float)deltaTime * Velocity.y,
                               50.0f + batchQuadVertices[i * 9 + 1], height - 50.0f + batchQuadVertices[i * 9 + 1]);

                vertexBuffer[i * 9 + 4] =
                    glm::clamp(vertexBuffer[i * 9 + 4] + (float)deltaTime * ColourVelocity.r, 0.0f, 1.0f);
                vertexBuffer[i * 9 + 5] =
                    glm::clamp(vertexBuffer[i * 9 + 5] + (float)deltaTime * ColourVelocity.g, 0.0f, 1.0f);
                vertexBuffer[i * 9 + 6] =
                    glm::clamp(vertexBuffer[i * 9 + 6] + (float)deltaTime * ColourVelocity.b, 0.0f, 1.0f);
            }
        }
    }

private:
    static inline char Bounce(float val, float max = 1.0f, float border = 0.0f) {
        if (val <= border || val >= max - border)
            return -1;
        return 1;
    }
};

class TestSpritesBatch : public Test {
public:
    TestSpritesBatch(ShaderController& shaderController, float winWidth, float winHeight);

    void OnUpdate(double deltaTime) override;
    void OnRender(const Renderer& renderer) override;

private:
    float m_Width;
    float m_Height;

    std::unique_ptr<float[]> m_VertexBuffer;
    std::unique_ptr<unsigned int[]> m_IndexBuffer;

    std::vector<Quad> m_Sprites;

    std::unique_ptr<VertexArray> m_VA;
    std::unique_ptr<VertexBufferLayout> m_VBL;

    ShaderController& m_ShaderController;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_TexDiamond;
    std::unique_ptr<Texture> m_TexStar;

    int m_uMVPLoc;
    int m_uTexLoc;
};

} // namespace Testing
