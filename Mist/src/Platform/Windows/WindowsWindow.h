#pragma once

#include "Mist/Renderer/GraphicsContext.h"

#include "Mist/Core/DeltaTime.h"
#include "Mist/Core/Window.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>

namespace Mist {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    DeltaTime OnUpdate() override;

    void OnFrameStart(DeltaTime deltaTime) override;
    void OnFrameEnd(DeltaTime deltaTime) override;

    inline uint32_t GetWidth() const override {
        return m_Data.Width;
    }

    inline uint32_t GetHeight() const override {
        return m_Data.Height;
    }

    inline void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline void* GetNativeWindow() const override {
        return m_Window;
    }

private:
    virtual void Init(const WindowProps& props);
    virtual void InitEventCallbacks();
    virtual void Shutdown();

private:
    struct WindowData {
        std::string Title;
        uint32_t Width = 0, Height = 0;
        bool VSync = false;

        EventCallbackFn EventCallback;
    };

    GLFWwindow* m_Window;
    GraphicsContext* m_Context;

    float m_LastFrameTime = 0.0f;

    WindowData m_Data;

    Scope<ImGuiIO> m_IO;
};

} // namespace Mist
