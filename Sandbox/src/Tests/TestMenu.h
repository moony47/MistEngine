#pragma once

#include <Mist.h>

using namespace Mist;

class TestMenu : public Layer {
public:
    TestMenu();
    ~TestMenu();

    void OnImGuiRender(DeltaTime deltaTime) override;

    template <typename T>
    void RegisterTest(const std::string& name) {
        MIST_INFO("Registering Test " + name);
        m_Tests.push_back(std::make_pair(name, [](Layer*& current) {
            Application::Get().PopLayer(current);
            current = new T;
            Application::Get().PushLayer(current);
        }));
    }

private:
    Layer* m_CurrentTest;
    std::vector<std::pair<std::string, std::function<void(Layer*&)>>> m_Tests;
};
