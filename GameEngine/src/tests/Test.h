#pragma once

#include "Renderer.h"

namespace Testing {

class Test {
public:
    Test() {
    }
    Test(float width, float height) {
    }
    virtual ~Test() {
    }

    virtual void OnUpdate(double deltaTime) {
    }
    virtual void OnRender(const Renderer& renderer) {
    }
    virtual void OnImGuiRender() {
    }
};

} // namespace Testing