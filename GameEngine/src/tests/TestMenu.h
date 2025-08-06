#pragma once

#include "Test.h"

#include <functional>
#include <iostream>
#include <vector>

class ShaderController;

namespace Testing {

class TestMenu : public Test {
public:
    TestMenu(ShaderController& shaderController, Test*& currentTestPointer, float winWidth, float winHeight);
    ~TestMenu();

    void OnImGuiRender() override;

    template <typename T> void RegisterTest(const std::string& name) {
        std::cout << "Registering Test " << name << std::endl;
        m_Tests.push_back(std::make_pair(
            name, [](ShaderController& shaderController, float w, float h) { return new T(shaderController, w, h); }));
    }

private:
    float m_Width;
    float m_Height;
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test*(ShaderController&, float, float)>>> m_Tests;

    ShaderController& m_ShaderController;
};

} // namespace Testing
