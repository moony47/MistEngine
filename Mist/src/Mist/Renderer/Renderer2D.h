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

    // Primitives
    static void DrawQuad(const glm::vec3& position,
                         const float angle,
                         const glm::vec2& size,
                         const glm::vec4& colour = glm::vec4(1.0f),
                         const std::string& textureName = "");
    static inline void DrawQuad(const glm::vec3& position,
                                const float angle,
                                const glm::vec2& size,
                                const std::string& textureName) {
        DrawQuad(position, angle, size, glm::vec4(1.0f), textureName);
    }
    static inline void DrawQuad(const glm::vec2& position,
                                const float angle,
                                const glm::vec2& size,
                                const glm::vec4& colour = glm::vec4(1.0f),
                                const std::string& textureName = "") {
        DrawQuad(glm::vec3(position, 0.0f), angle, size, colour, textureName);
    }
    static inline void DrawQuad(const glm::vec2& position,
                                const float angle,
                                const glm::vec2& size,
                                const std::string& textureName) {
        DrawQuad(glm::vec3(position, 0.0f), angle, size, glm::vec4(1.0f), textureName);
    }

private:
};

} // namespace Mist
