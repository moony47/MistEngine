#pragma once

#include "OpenGL/Renderer.h"

namespace Mist::Testing {

class Test {
public:
    Test() {
    }
    Test(float width, float height) {
    }
    virtual ~Test() {
    }

    virtual void OnUpdate(float deltaTime) {
    }
    virtual void OnRender(const Renderer& renderer) {
    }
    virtual void OnImGuiRender() {
    }
    virtual void Resize(unsigned int width, unsigned int height){}
};

} // namespace Testing