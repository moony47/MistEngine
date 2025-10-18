#include <Mist.h>

#include <EntryPoint.h>

#include "EditorLayer.h"

using namespace Mist;

class Mistwraith : public Application {
public:
    Mistwraith() : Application("Mistwraith Editor") {
        PushLayer(CreateRef<EditorLayer>());
    }

    ~Mistwraith() {
    }
};

Mist::Application* Mist::CreateApplication() {
    return new Mistwraith();
}
