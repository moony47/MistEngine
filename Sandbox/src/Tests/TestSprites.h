#pragma once

#include <Mist.h>

using namespace Mist;

static const float singleQuadVertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, // 0
    +0.5f, -0.5f, 1.0f, 0.0f, // 1
    +0.5f, +0.5f, 1.0f, 1.0f, // 2
    -0.5f, +0.5f, 0.0f, 1.0f  // 3
};

static const unsigned int singleQuadIndices[] = {
    0, 1, 2, // 0
    2, 3, 0  // 1
};

struct Sprite {
    bool star;
    glm::vec4 Colour;
    glm::vec4 ColourVelocity;
    glm::vec3 Position;
    glm::vec3 Velocity;

    void Update(float deltaTime, float left, float bottom, float right, float top) {
        Colour =
            glm::clamp(Colour + (float)deltaTime * ColourVelocity, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

        Position = glm::clamp(Position + (float)deltaTime * Velocity, {left, bottom, 0.0f}, {right, top, 0.0f});

        ColourVelocity.r = Bounce(Colour.r, ColourVelocity.r);
        ColourVelocity.g = Bounce(Colour.g, ColourVelocity.g);
        ColourVelocity.b = Bounce(Colour.b, ColourVelocity.b);

        Velocity.x = Bounce(Position.x, Velocity.x, left, right);
        Velocity.y = Bounce(Position.y, Velocity.y, bottom, top);
    }

private:
    static inline float Bounce(float val, float increment, float min = 0.0f, float max = 1.0f) {
        if (val <= min || val >= max)
            return -increment;
        return increment;
    }
};

class TestSprites : public Layer {
public:
    TestSprites();

    void OnDetach() override;

    void OnUpdate(DeltaTime deltaTime) override;
    void OnFrameStart(DeltaTime deltaTime) override;
    void OnFrameEnd(DeltaTime deltaTime) override;
    void OnEvent(Event& e) override;

private:
    std::vector<Sprite> m_StarSprites;
    std::vector<Sprite> m_DiamondSprites;

    OrthographicCameraController m_CameraController;

    Ref<VertexArray> m_VA;
};