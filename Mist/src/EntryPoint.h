#pragma once

#ifdef MIST_PLATFORM_WINDOWS

extern Mist::Application* Mist::CreateApplication();

int main(int argc, char** argv) {
    Mist::Logger::Init();

    PROFILE_SESSION_START("Start Up", "MistProfile_StartUp.json");
    auto app = Mist::CreateApplication();
    PROFILE_SESSION_END();

    // Only profile when user has enabled it
    PROFILE_ENABLED = false;

    PROFILE_SESSION_START("Runtime", "MistProfile_Runtime.json");
    app->Run();
    PROFILE_SESSION_END();

    // Ensure profiling of shutdown 
    PROFILE_ENABLED = true;

    PROFILE_SESSION_START("Shutdown", "MistProfile_Shutdown.json");
    delete app;
    PROFILE_SESSION_END();
}

#endif