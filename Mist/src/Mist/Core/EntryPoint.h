#pragma once

#ifdef MIST_PLATFORM_WINDOWS

extern Mist::Application* Mist::CreateApplication();

int main(int argc, char** argv) {
    Mist::Logger::Init();

    auto app = Mist::CreateApplication();
    app->Run();
    delete app;
}

#endif