#pragma once

#include "Mist/Window.h"
#include "Mist/Renderer/GraphicsContext.h"

#include "OpenGL/TestLayer.h"

#include <GLFW/glfw3.h>


namespace Mist {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdateStart() override;
    void OnUpdateEnd() override;

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
        unsigned int Width = 0, Height = 0;
        bool VSync = false;

        EventCallbackFn EventCallback;
    };

    GLFWwindow* m_Window;
    GraphicsContext* m_Context;

    Renderer m_Renderer;

    float m_LastTime = 0.0f;

    WindowData m_Data;

    TestLayer* TEMP_layer;

    std::unique_ptr<ImGuiIO> m_IO;
};

} // namespace Mist
