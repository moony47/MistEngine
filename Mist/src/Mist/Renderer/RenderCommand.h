#pragma once

#include "RendererAPI.h"

namespace Mist {

class RenderCommand {
public:
    inline static void Init() {
        s_RendererAPI->Init();
    }

    inline static void Shutdown() {
        throw std::logic_error("RenderCommand::Shutdown not implemented");
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    inline static void SetClearColour(const glm::vec4& colour) {
        s_RendererAPI->SetClearColour(colour);
    }

    inline static void Clear() {
        s_RendererAPI->Clear();
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }
    inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) {
        s_RendererAPI->DrawLines(vertexArray, vertexCount);
    }
    inline static void SetLineThickness(float thickness) {
        s_RendererAPI->SetLineThickness(thickness);
    }

private:
    static RendererAPI* s_RendererAPI;
};

} // namespace Mist