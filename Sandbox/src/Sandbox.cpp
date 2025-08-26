#include <Mist.h>

#include <EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"

#include "Tests/TestClearColour.h"
#include "Tests/TestMenu.h"
#include "Tests/TestSprites.h"
#include "Tests/TestSpritesBatch.h"

using namespace Mist;

class Sandbox : public Application {
public:
    Sandbox() :
        m_TestMenu(std::make_shared<TestMenu>()) {

        m_TestMenu->RegisterTest<Sandbox2D>("Sandbox2D");
        m_TestMenu->RegisterTest<ExampleLayer>("ExampleLayer");
        m_TestMenu->RegisterTest<TestClearColour>("TestClearColour");
        m_TestMenu->RegisterTest<TestSprites>("TestSprites");
        m_TestMenu->RegisterTest<TestSpritesBatch>("TestSpritesBatch");

        PushLayer(m_TestMenu);
    }

    ~Sandbox() {
    }

private:
    Ref<TestMenu> m_TestMenu;
};

Mist::Application* Mist::CreateApplication() {
    return new Sandbox();
}
