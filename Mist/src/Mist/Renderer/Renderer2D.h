#pragma once

#include "Mist/Cameras/Camera.h"
#include "Mist/Cameras/EditorCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Mist {

struct QuadDrawArgs {
    int EntityID;
    const glm::mat4& Transform;
    const glm::vec4& Colour;
    const std::string& TextureName;
    float TilingFactor;

    QuadDrawArgs(int entityID,
                 const glm::mat4& transform,
                 const glm::vec4& colour = glm::vec4(1.0f),
                 const std::string& textureName = "WHITE",
                 float tilingFactor = 1.0f) :
        EntityID(entityID),
        Transform(transform),
        Colour(colour),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(int entityID, const glm::mat4& transform, const std::string& textureName, float tilingFactor = 1.0f) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(int entityID, const glm::mat4& transform, float tilingFactor) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}),
        TextureName("WHITE"),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform,
                 const glm::vec4& colour = glm::vec4(1.0f),
                 const std::string& textureName = "WHITE",
                 float tilingFactor = 1.0f) :
        EntityID(-1),
        Transform(transform),
        Colour(colour),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform, const std::string& textureName, float tilingFactor = 1.0f) :
        EntityID(-1),
        Transform(transform),
        Colour(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform, float tilingFactor) :
        EntityID(-1),
        Transform(transform),
        Colour(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}),
        TextureName("WHITE"),
        TilingFactor(tilingFactor) {};
};

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginView(const glm::mat4& projection, const glm::mat4& transform);
    static void BeginView(OrthographicCamera& camera);
    static void BeginView(EditorCamera& camera);
    static void EndView();

    static void DrawQuad(const QuadDrawArgs&& drawArgs);

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
