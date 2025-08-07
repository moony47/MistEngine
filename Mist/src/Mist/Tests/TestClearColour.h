#pragma once

#include "Test.h"
namespace Mist {

class ShaderController;

namespace Testing {

class TestClearColour : public Test {
public:
    TestClearColour(ShaderController& shaderController, float width, float height);

    void OnRender(const Renderer& renderer) override;
    void OnImGuiRender() override;

private:
    float m_ClearColour[4];
};

} // namespace Testing
} // namespace Mist
