#pragma once

#include "Camera.h"

namespace Mist {

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginView(OrthographicCamera& camera);
    static void EndView();

    static void DrawQuad(const glm::mat4& transform,
                         const glm::vec4& colour = glm::vec4(1.0f),
                         const std::string& textureName = "WHITE",
                         float tilingFactor = 1.0f);
    static inline void DrawQuad(const glm::mat4& transform, const std::string& textureName, float tilingFactor = 1.0f) {
        DrawQuad(transform, glm::vec4(1.0f), textureName, tilingFactor);
    }

    static inline void DrawQuad(const glm::vec3& position,
                                const float angleRad,
                                const glm::vec2& size,
                                const glm::vec4& colour = glm::vec4(1.0f),
                                const std::string& textureName = "WHITE",
                                float tilingFactor = 1.0f) {
        // Compute transform for the quad geometry
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, angleRad, {0, 0, 1});
        transform = glm::scale(transform, glm::vec3(size, 1.0f));
        DrawQuad(transform, colour, textureName, tilingFactor);
    }
    static inline void DrawQuad(const glm::vec3& position,
                                const float angleRad,
                                const glm::vec2& size,
                                const std::string& textureName,
                                float tilingFactor = 1.0f) {
        DrawQuad(position, angleRad, size, glm::vec4(1.0f), textureName, tilingFactor);
    }

    static inline void DrawQuad(const glm::vec2& position,
                                const float angleRad,
                                const glm::vec2& size,
                                const glm::vec4& colour = glm::vec4(1.0f),
                                const std::string& textureName = "WHITE",
                                float tilingFactor = 1.0f) {
        DrawQuad(glm::vec3(position, 0.0f), angleRad, size, colour, textureName, tilingFactor);
    }
    static inline void DrawQuad(const glm::vec2& position,
                                const float angleRad,
                                const glm::vec2& size,
                                const std::string& textureName,
                                float tilingFactor = 1.0f) {
        DrawQuad(glm::vec3(position, 0.0f), angleRad, size, glm::vec4(1.0f), textureName, tilingFactor);
    }

private:
    static void BeginBatch();
    static void FlushBatch();

public:
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;

        inline uint32_t GetVertexCount() {
            return QuadCount * 4;
        }
        inline uint32_t GetIndexCount() {
            return QuadCount * 6;
        }
    };

    static Statistics GetStats();
    static void ResetStats();
};

} // namespace Mist
