#include <Engine.h>

class Sandbox : public GameEngine::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}
