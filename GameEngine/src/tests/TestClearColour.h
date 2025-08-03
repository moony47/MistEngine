#pragma once

#include "Test.h"

namespace test {
    class TestClearColour : public Test {
    public:
        TestClearColour(float width, float height);

        void OnRender(const Renderer& renderer) override;
        void OnImGuiRender() override;
    private:
        float m_ClearColour[4];
    };
}

