#pragma once

#include "Test.h"

#include "Texture.h"

#include <memory>
#include <vector>

namespace test {
    static const float batchQuadVertices[] = {
        -50.0f, -50.0f,     0.0f, 0.0f,     0.3f, 0.3f, 0.8f, 1.0f,     0.0f,
         50.0f, -50.0f,     1.0f, 0.0f,     0.3f, 0.3f, 0.8f, 1.0f,     0.0f,
         50.0f,  50.0f,     1.0f, 1.0f,     0.3f, 0.3f, 0.8f, 1.0f,     0.0f,
        -50.0f,  50.0f,     0.0f, 1.0f,     0.3f, 0.3f, 0.8f, 1.0f,     0.0f
    };

    static const unsigned int batchQuadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    struct Quad {
        bool star;
        glm::vec4 Colour;
        glm::vec4 ColourVelocity;
        glm::vec3 Position;
        glm::vec3 Velocity;

        void Update(float deltaTime, float width, float height, unsigned int spriteIndex, float* vertexBuffer, unsigned int* indexBuffer) {
            Colour = glm::clamp(
                Colour + deltaTime * ColourVelocity,
                { 0.0f, 0.0f, 0.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f }
            );

            Position = glm::clamp(
                Position + deltaTime * Velocity,
                { 50.0f,         50.0f,          0.0f },
                { width - 50.0f, height - 50.0f, 0.0f }
            );

            ColourVelocity.r = Bounce(Colour.r, ColourVelocity.r);
            ColourVelocity.g = Bounce(Colour.g, ColourVelocity.g);
            ColourVelocity.b = Bounce(Colour.b, ColourVelocity.b);

            Velocity.x = Bounce(Position.x, Velocity.x, width, 50.0f);
            Velocity.y = Bounce(Position.y, Velocity.y, height, 50.0f);

            for (int i = 0; i < 4; i++) {
                vertexBuffer[i * 9 + 0] = Position.x + batchQuadVertices[i * 9 + 0];
                vertexBuffer[i * 9 + 1] = Position.y + batchQuadVertices[i * 9 + 1];
                vertexBuffer[i * 9 + 2] = batchQuadVertices[i * 9 + 2];
                vertexBuffer[i * 9 + 3] = batchQuadVertices[i * 9 + 3];
                vertexBuffer[i * 9 + 4] = Colour.r;
                vertexBuffer[i * 9 + 5] = Colour.g;
                vertexBuffer[i * 9 + 6] = Colour.b;
                vertexBuffer[i * 9 + 7] = Colour.a;
                vertexBuffer[i * 9 + 8] = (float)star;
            }

            for (int i = 0; i < 6; i++)
                indexBuffer[i] = batchQuadIndices[i] + spriteIndex * 4;
        }
    private:

        static inline float Bounce(float val, float increment, float max = 1.0f, float border = 0.0f) {
            if (val <= border || val >= max - border)
                return -increment;
            return increment;
        }
    };

    class TestSpritesBatch : public Test {
    public:
        TestSpritesBatch(float winWidth, float winHeight);

        void OnUpdate(float deltaTime) override;
        void OnRender(const Renderer& renderer) override;
    private:
        float m_Width;
        float m_Height;

        std::vector<Quad> m_Sprites;

        std::unique_ptr<VertexArray> m_VA;
        std::unique_ptr<VertexBufferLayout> m_VBL;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_TexDiamond;
        std::unique_ptr<Texture> m_TexStar;

        int m_uMVPLoc;
        int m_uTexLoc;
    };
}

