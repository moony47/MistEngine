#pragma once

#include <Mist.h>

using namespace Mist;

const static float batchQuadVertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f, // 0
    +0.5f, -0.5f, 1.0f, 0.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f, // 1
    +0.5f, +0.5f, 1.0f, 1.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f, // 2
    -0.5f, +0.5f, 0.0f, 1.0f, 0.3f, 0.3f, 0.8f, 1.0f, 0.0f  // 3
};

static const unsigned int batchQuadIndices[] = {
    0, 1, 2, // 0
    2, 3, 0  // 1
};

struct Quad {
    bool star;
    glm::vec4 ColourVelocity;
    glm::vec3 Velocity;
    float* vertexBuffer;
    unsigned int* indexBuffer;

    bool init = false;

    void Update(float deltaTime, float left, float bottom, float right, float top, unsigned int spriteIndex) {
        ColourVelocity.r = Bounce(vertexBuffer[4]) * ColourVelocity.r;
        ColourVelocity.g = Bounce(vertexBuffer[5]) * ColourVelocity.g;
        ColourVelocity.b = Bounce(vertexBuffer[6]) * ColourVelocity.b;

        Velocity.x = Bounce(vertexBuffer[0] - batchQuadVertices[0], left, right) * Velocity.x;
        Velocity.y = Bounce(vertexBuffer[1] - batchQuadVertices[1], bottom, top) * Velocity.y;

        if (!init) {
            // Set buffer data which won't change
            init = true;
            for (int i = 0; i < 4; i++) {
                vertexBuffer[i * 9 + 0] = (left + right) / 2.0f + batchQuadVertices[i * 9 + 0];
                vertexBuffer[i * 9 + 1] = (bottom + top) / 2.0f + batchQuadVertices[i * 9 + 1];
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
                    glm::clamp(vertexBuffer[i * 9 + 0] + (float)deltaTime * Velocity.x, left, right);
                vertexBuffer[i * 9 + 1] =
                    glm::clamp(vertexBuffer[i * 9 + 1] + (float)deltaTime * Velocity.y, bottom, top);

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
    static inline char Bounce(float val, float min = 0.0f, float max = 1.0f) {
        if (val <= min || val >= max)
            return -1;
        return 1;
    }
};

class TestSpritesBatch : public Layer {
public:
    TestSpritesBatch();

    void OnUpdate(DeltaTime deltaTime) override;
    void OnFrameStart(DeltaTime deltaTime) override;
    void OnFrameEnd(DeltaTime deltaTime) override;
    void OnEvent(Event& e) override;

private:
    std::unique_ptr<float[]> m_VertexBuffer;
    std::unique_ptr<unsigned int[]> m_IndexBuffer;

    std::vector<Quad> m_Sprites;

    OrthographicCameraController m_CameraController;

    Ref<VertexArray> m_VA;
    Ref<BufferLayout> m_VBL;
};
