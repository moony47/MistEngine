#include <Mist.h>

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
    }

    void OnUpdateStart() override {
    }

    void OnUpdateEnd() override {
    }

    void OnEvent(Mist::Event& e) override {
        if (e.GetEventType() == Mist::EventType::KeyPressed) {
            Mist::KeyPressedEvent& ev = (Mist::KeyPressedEvent&)e;
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
