#pragma once

#include <Mist.h>

using namespace Mist;

class TestClearColour : public Layer {
public:
    TestClearColour();

    void OnUpdate(DeltaTime deltaTime) override;
    void OnImGuiRender(DeltaTime deltaTime) override;

private:
    glm::vec4 m_ClearColour;
};
