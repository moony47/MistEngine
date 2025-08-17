#pragma once

#include "Test.h"

namespace Mist {

namespace Testing {

class TestClearColour : public Test {
public:
    TestClearColour(float width, float height);

    void OnRender() override;
    void OnImGuiRender() override;

private:
    float m_ClearColour[4];
};

} // namespace Testing
} // namespace Mist
