#pragma once

#include "Mist/Window.h"
#include "TestLayer.h"

#include <GLFW/glfw3.h>

namespace Mist {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override {
        return m_Data.Width;
    }

    inline unsigned int GetHeight() const override {
        return m_Data.Height;
    }

    inline void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    void Resize(unsigned int width, unsigned int height) override;

private:
    virtual void Init(const WindowProps& props);
    virtual void InitEventCallbacks();
    virtual void Shutdown();

private:
    GLFWwindow* m_Window;
    Renderer m_Renderer;

    float m_LastTime = 0.0f;

    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;

    TestLayer* TEMP_layer;

    std::unique_ptr<ImGuiIO> m_IO;
};

} // namespace Mist
