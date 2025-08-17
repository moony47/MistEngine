#pragma once

#include "Test.h"


namespace Mist {

namespace Testing {

class TestMenu : public Test {
public:
    TestMenu(Test*& currentTestPointer, float winWidth, float winHeight);
    ~TestMenu();

    void OnImGuiRender() override;

    template <typename T> void RegisterTest(const std::string& name) {
        MIST_CORE_INFO ("Registering Test " + name);
        m_Tests.push_back(std::make_pair(
            name, [](float w, float h) { return new T(w, h); }));
    }

private:
    float m_Width;
    float m_Height;
    Test*& m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Test*(float, float)>>> m_Tests;

};

} // namespace Testing
} // namespace Mist
