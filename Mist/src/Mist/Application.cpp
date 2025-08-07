#include "Application.h"
#include "TestLayer.h"

#include "Events/ApplicationEvent.h"
#include "Logger.h"

namespace Mist {

TestLayer* layer;

Application::Application() {
    layer = new TestLayer(1600, 900);
}

Application::~Application() {
    delete layer;
}

void Application::Run() {
    //  Loop until the user closes the window
    while (!layer->WindowShouldClose())
        layer->Update();
}

} // namespace Mist