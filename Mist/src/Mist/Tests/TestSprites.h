#pragma once

#include "Test.h"

static const float singleQuadVertices[] = {-50.0f, -50.0f, 0.0f, 0.0f, 50.0f,  -50.0f, 1.0f, 0.0f,
                                           50.0f,  50.0f,  1.0f, 1.0f, -50.0f, 50.0f,  0.0f, 1.0f};

static const unsigned int singleQuadIndices[] = {0, 1, 2, 2, 3, 0};

namespace Mist {

class ShaderController;
class VertexArray;
class IndexBuffer;
class Shader;
class Texture;

namespace Testing {

struct Sprite {
    bool star;
    glm::vec4 Colour;
    glm::vec4 ColourVelocity;
    glm::vec3 Position;
    glm::vec3 Velocity;

    void Update(float deltaTime, float width, float height) {
        Colour =
            glm::clamp(Colour + (float)deltaTime * ColourVelocity, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

        Position = glm::clamp(Position + (float)deltaTime * Velocity, {50.0f, 50.0f, 0.0f},
                              {width - 50.0f, height - 50.0f, 0.0f});

        ColourVelocity.r = Bounce(Colour.r, ColourVelocity.r);
        ColourVelocity.g = Bounce(Colour.g, ColourVelocity.g);
        ColourVelocity.b = Bounce(Colour.b, ColourVelocity.b);

        Velocity.x = Bounce(Position.x, Velocity.x, width, 50.0f);
        Velocity.y = Bounce(Position.y, Velocity.y, height, 50.0f);
    }

private:
    static inline float Bounce(float val, float increment, float max = 1.0f, float border = 0.0f) {
        if (val <= border || val >= max - border)
            return -increment;
        return increment;
    }
};

class TestSprites : public Test {
public:
    TestSprites(ShaderController& shaderController, float winWidth, float winHeight);

    void OnUpdate(float deltaTime) override;
    void OnRender(const Renderer& renderer) override;

private:
    float m_Width;
    float m_Height;

    std::vector<Sprite> m_StarSprites;
    std::vector<Sprite> m_DiamondSprites;

    std::unique_ptr<VertexArray> m_VA;
    std::unique_ptr<IndexBuffer> m_IB;

    ShaderController& m_ShaderController;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_TexDiamond;
    std::unique_ptr<Texture> m_TexStar;

    int m_uVPLoc;
    int m_uTexLoc;
    int m_uMLoc;
    int m_uColourLoc;
};

} // namespace Testing
} // namespace Mist