#pragma once

#include "mistpch.h"

#include "Mist/Core/Core.h"
#include "Mist/Core/DeltaTime.h"
#include "Mist/Events/EventSystem.h"

namespace Mist {

struct WindowProps {
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    WindowProps(const std::string& title, uint32_t width = 1920, uint32_t height = 1080) :
        Title(title),
        Width(width),
        Height(height) {
    }
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {
    }

    virtual DeltaTime OnUpdate() = 0;

    virtual void OnFrameStart(DeltaTime deltaTime) = 0;
    virtual void OnFrameEnd(DeltaTime deltaTime) = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props);
};

} // namespace Mist