#pragma once

#include "Mist/Renderer/RendererAPI.h"

namespace Mist {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
        void Init() override;

		void SetClearColour(const glm::vec4& colour) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}

