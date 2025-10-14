#pragma once

#ifdef MIST_PLATFORM_WINDOWS

extern Mist::Application* Mist::CreateApplication();

int main(int argc, char** argv) {
    Mist::Logger::Init();

    MIST_PROFILE_START("Start Up", "MistProfile_StartUp.json");
    auto app = Mist::CreateApplication();
    MIST_PROFILE_END();

    // Only profile when user has enabled it
    MIST_PROFILE_ENABLED = false;

    MIST_PROFILE_START("Runtime", "MistProfile_Runtime.json");
    app->Run();
    MIST_PROFILE_END();

    // Ensure profiling of shutdown 
    MIST_PROFILE_ENABLED = true;

    MIST_PROFILE_START("Shutdown", "MistProfile_Shutdown.json");
    delete app;
    MIST_PROFILE_END();
}

#endif