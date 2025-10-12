#pragma once

#include "Camera.h"
#include "Texture.h"

namespace Mist {

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();

    static void Flush();

    static void DrawQuad(const glm::vec3& position,
                         const float angle,
                         const glm::vec2& size,
                         const glm::vec4& colour = glm::vec4(1.0f),
                         const std::string& textureName = "WHITE",
                         float tilingFactor = 1.0f);
    static inline void DrawQuad(const glm::vec3& position,
                                const float angle,
                                const glm::vec2& size,
                                const std::string& textureName,
                                float tilingFactor = 1.0f) {
        DrawQuad(position, angle, size, glm::vec4(1.0f), textureName, tilingFactor);
    }
    static inline void DrawQuad(const glm::vec2& position,
                                const float angle,
                                const glm::vec2& size,
                                const glm::vec4& colour = glm::vec4(1.0f),
                                const std::string& textureName = "WHITE",
                                float tilingFactor = 1.0f) {
        DrawQuad(glm::vec3(position, 0.0f), angle, size, colour, textureName, tilingFactor);
    }
    static inline void DrawQuad(const glm::vec2& position,
                                const float angle,
                                const glm::vec2& size,
                                const std::string& textureName,
                                float tilingFactor = 1.0f) {
        DrawQuad(glm::vec3(position, 0.0f), angle, size, glm::vec4(1.0f), textureName, tilingFactor);
    }
};

} // namespace Mist
