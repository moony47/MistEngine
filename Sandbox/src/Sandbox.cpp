#include <Mist.h>

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
    }

    void OnUpdateStart() override {
    }

    void OnUpdateEnd() override {
        // MIST_INFO("ExampleLayer::Update");

        if (Mist::Input::IsKeyPressed(MIST_KEY_TAB))
            MIST_INFO("Tab is currently pressed");
    }

    void OnEvent(Mist::Event& e) override {
        if (e.GetEventType() == Mist::EventType::KeyPressed) {
            Mist::KeyPressedEvent& ev = (Mist::KeyPressedEvent&)e;
            if (ev.GetKeyCode() == MIST_KEY_TAB)
                MIST_INFO("Tab pressed event");
            //MIST_TRACE((char)ev.GetKeyCode());
        }
    }
};

class Sandbox : public Mist::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new Mist::ImGuiLayer());
    }
    ~Sandbox() {
    }
};

Mist::Application* Mist::CreateApplication() {
    return new Sandbox();
}
