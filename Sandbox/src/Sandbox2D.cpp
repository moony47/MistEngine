#include "Sandbox2D.h"

template <typename Func>
class Timer {
public:
    Timer(const char* name, Func&& func) :
        m_Name(name),
        m_Stopped(false),
        m_Func(func) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        if (!m_Stopped)
            Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        float duration = (end - start) * 0.001f;

        m_Stopped = true;

        m_Func({m_Name, duration});
        // MIST_TRACE("{0} Duration: {1}ms", m_Name, duration);
    };

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
    Func m_Func;
};

#define PROFILE_SCOPE(name)                                                                                            \
    Timer timer##__LINE__(name,                                                                                        \
                          [&](Sandbox2D::ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
}

void Sandbox2D::OnAttach() {
    RenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    MIST_TEXTURE2DLIB->Create("Diamond", "res/textures/Diamond.png");
}

void Sandbox2D::OnDetach() {
    MIST_TEXTURE2DLIB->Remove("Diamond");
}

void Sandbox2D::OnUpdate(DeltaTime deltaTime) {
    PROFILE_SCOPE("Sandbox2D::OnUpdate");

    m_CameraController.OnUpdate(deltaTime);
}

void Sandbox2D::OnFrameStart(DeltaTime deltaTime) {
    PROFILE_SCOPE("Sandbox2D::OnFrameStart");

    Renderer2D::BeginScene(m_CameraController.GetCamera());
}

void Sandbox2D::OnFrameEnd(DeltaTime deltaTime) {
    PROFILE_SCOPE("Sandbox2D::OnFrameEnd");

    // Renderer2D::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
    Renderer2D::DrawQuad({0.0f, 0.0f}, 0.0f, {100.0f, 100.0f}, "Diamond");
    Renderer2D::DrawQuad({-0.5f, -0.5f}, 30.0f, {0.5f, 0.5f}, m_SpriteColour, "Diamond");
    Renderer2D::DrawQuad({0.5f, 0.5f}, 0.0f, {1.5f, 0.5f}, glm::vec4{1.0f, 1.0f, 1.0f, 2.0f} - m_SpriteColour);
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender(DeltaTime deltaTime) {
    ImGui::Begin("Sandbox2D");
    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour), ImGuiColorEditFlags_Float);

    for (auto& result : m_ProfileResults) {
        char label[75];
        strcpy(label, "%.3fms ");
        strcat(label, result.Name);
        ImGui::Text(label, result.Time);
    }
    m_ProfileResults.clear();

    ImGui::End();
}

void Sandbox2D::OnEvent(Event& e) {
    m_CameraController.OnEvent(e);
}
