#include <Mist.h>

#include <EntryPoint.h>

#include "EditorLayer.h"

using namespace Mist;

class Mistwraith : public Application {
public:
    Mistwraith(ApplicationCommandLineArgs args) :
        Application("Mistwraith Editor", args) {
        PushLayer(CreateRef<EditorLayer>());
    }

    ~Mistwraith() {
    }
};

Application* Mist::CreateApplication(ApplicationCommandLineArgs args) {
    return new Mistwraith(args);
}
