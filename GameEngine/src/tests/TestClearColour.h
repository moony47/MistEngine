#pragma once

#include "Test.h"

namespace test {
    class TestClearColour : public Test {
    public:
        TestClearColour();
        ~TestClearColour();

        void OnUpdate(float deltaTime) override;
        void OnRender(const Renderer& renderer, const glm::mat4& proj) override;
        bool OnImGuiRender() override;
    private:
        float m_ClearColour[4];
    };
}

