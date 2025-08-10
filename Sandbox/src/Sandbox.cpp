#include <Mist.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Mist::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
    }

    void OnUpdateStart() override {
    }

    void OnUpdateEnd() override {
    }

    void OnImGuiRender() override {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
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
    }
    ~Sandbox() {
    }
};

Mist::Application* Mist::CreateApplication() {
    return new Sandbox();
}
