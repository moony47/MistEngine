#pragma once

#include "Renderer.h"

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender(const Renderer& renderer, const glm::mat4& proj) {}
		virtual bool OnImGuiRender() { return false; }
	};
}