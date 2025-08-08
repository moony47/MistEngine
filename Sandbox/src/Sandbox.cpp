#include <Mist.h>

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
    }

    void OnUpdate() override {
        MIST_INFO("ExampleLayer::Update");
    }

    void OnEvent(Mist::Event& e) override {
        MIST_TRACE(e);
    }
};

class Sandbox : public Mist::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }
    ~Sandbox() {
    }
};

Mist::Application* Mist::CreateApplication() {
    return new Sandbox();
}
