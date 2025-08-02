#pragma once

#include "Test.h"

#include "Texture.h"
#include "VertexBufferLayout.h"

#include <vector>

struct Sprite {
public:
    glm::vec4 Colour;
    glm::vec4 ColourVelocity;
    glm::vec3 Position;
    glm::vec3 Velocity;

    Sprite Update(float deltaTime) {
        glm::vec4 colourVelocity(
            BounceFloat(ColourVelocity.r),
            BounceFloat(ColourVelocity.g),
            BounceFloat(ColourVelocity.b),
            ColourVelocity.a
        );

        glm::vec3 velocity(
            BounceFloat(Velocity.x),
            BounceFloat(Velocity.y),
            Velocity.z
        );

        return Sprite(
            Colour + deltaTime * ColourVelocity,
            colourVelocity,
            Position + deltaTime * Velocity,
            velocity
        );
    }
private:
    static inline float BounceFloat(float f) {
        if (f < 0.0f || f > 1.0f)
            return -f;
        return f;
    }
};

namespace test {
    class TestSprites: public Test {
    public:
        TestSprites();
        ~TestSprites();

        void OnUpdate(float deltaTime) override;
        void OnRender(const Renderer& renderer, const glm::mat4& proj) override;
        bool OnImGuiRender() override;
    private:
        std::vector<Sprite> m_Sprites;

        VertexArray va;
        VertexBuffer vb;
        VertexBufferLayout layout;
        IndexBuffer ib;
        Shader shader;
        Texture texture;

        glm::mat4 view;
    };
}

