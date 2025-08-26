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
        MIST_TRACE("Registering Test " + name);
        m_Tests.push_back(std::make_pair(name, [](Ref<Layer> currentLayer) {
            MIST_APP.PopLayer(currentLayer);
            Ref<Layer> newLayer = std::make_shared<T>();
            MIST_APP.PushLayer(newLayer);
            return newLayer;
        }));
    }

private:
    Ref<Layer> m_CurrentTest;
    std::vector<std::pair<std::string, std::function<Ref<Layer>(Ref<Layer>)>>> m_Tests;
};
