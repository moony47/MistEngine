#pragma once
#include "Mist/Core.h"
#include "Mist/Events/Event.h"

namespace Mist {

class MIST_API Application {
public:
    Application();
    virtual ~Application();

    void Run();
};

Application* CreateApplication();

} // namespace Mist