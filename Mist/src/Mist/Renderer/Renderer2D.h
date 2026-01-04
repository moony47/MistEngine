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
                 const std::string& textureName = "None",
                 float tilingFactor = 1.0f) :
        EntityID(entityID),
        Transform(transform),
        Colour(colour),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(int entityID, const glm::mat4& transform, const std::string& textureName, float tilingFactor = 1.0f) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(int entityID, const glm::mat4& transform, float tilingFactor) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        TextureName("None"),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform,
                 const glm::vec4& colour = glm::vec4(1.0f),
                 const std::string& textureName = "None",
                 float tilingFactor = 1.0f) :
        EntityID(-1),
        Transform(transform),
        Colour(colour),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform, const std::string& textureName, float tilingFactor = 1.0f) :
        EntityID(-1),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        TextureName(textureName),
        TilingFactor(tilingFactor) {};
    QuadDrawArgs(const glm::mat4& transform, float tilingFactor) :
        EntityID(-1),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        TextureName("None"),
        TilingFactor(tilingFactor) {};
};
struct CircleDrawArgs {
    int EntityID;
    const glm::mat4& Transform;
    const glm::vec4& Colour;
    float Thickness;
    float Fade;

    CircleDrawArgs(int entityID,
                   const glm::mat4& transform,
                   const glm::vec4& colour = glm::vec4(1.0f),
                   float thickness = 1.0f,
                   float fade = 0.005f) :
        EntityID(entityID),
        Transform(transform),
        Colour(colour),
        Thickness(thickness),
        Fade(fade) {};
    CircleDrawArgs(int entityID,
                   const glm::mat4& transform,
                   const glm::vec4& colour = glm::vec4(1.0f),
                   float fade = 0.005f) :
        EntityID(entityID),
        Transform(transform),
        Colour(colour),
        Thickness(1.0f),
        Fade(fade) {};
    CircleDrawArgs(int entityID, const glm::mat4& transform, float thickness = 1.0f, float fade = 0.005f) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        Thickness(thickness),
        Fade(fade) {};
    CircleDrawArgs(int entityID, const glm::mat4& transform, float fade) :
        EntityID(entityID),
        Transform(transform),
        Colour(glm::vec4(1.0f)),
        Thickness(1.0f),
        Fade(fade) {};
};
// struct LineDrawArgs {
//     int EntityID;
//     float Thickness;
//     const glm::vec3& EndPos1;
//     const glm::vec3& EndPos2;
//     const glm::vec4& Colour;
// };

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginView(const glm::mat4& projection, const glm::mat4& transform);
    static void BeginView(OrthographicCamera& camera);
    static void BeginView(EditorCamera& camera);
    static void EndView();

    static void DrawQuad(const QuadDrawArgs&& drawArgs);
    static void DrawCircle(const CircleDrawArgs&& drawArgs);
    // static void DrawLine(const LineDrawArgs&& drawArgs);

private:
    static void BeginBatch();
    static void BeginQuads();
    static void BeginCircles();
    static void FlushQuads();
    static void FlushCircles();

public:
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
        uint32_t CircleCount = 0;

        inline uint32_t VertexCount() const {
            return (QuadCount + CircleCount) * 4;
        }
        inline uint32_t IndexCount() const {
            return (QuadCount + CircleCount) * 6;
        }
    };

    static Statistics GetStats();
    static void ResetStats();
};

} // namespace Mist
