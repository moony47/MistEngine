#pragma once
#include "Core.h"

namespace GameEngine {

class API Application {
public:
    Application();
    virtual ~Application();

    void Run();
};

Application* CreateApplication();

} // namespace GameEngine