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
struct LineDrawArgs {
    int EntityID;
    const glm::vec3& Point1;
    const glm::vec3& Point2;
    const glm::vec4& Colour;
    float Thickness;

    LineDrawArgs(int entityID,
                 const glm::vec3& pos1,
                 const glm::vec3& pos2,
                 const glm::vec4& colour = glm::vec4(1.0f),
                 float thickness = 1.0f) :
        EntityID(entityID),
        Point1(pos1),
        Point2(pos2),
        Colour(colour),
        Thickness(thickness) {};
    LineDrawArgs(int entityID, const glm::vec3& pos1, const glm::vec3& pos2, float thickness) :
        EntityID(entityID),
        Point1(pos1),
        Point2(pos2),
        Colour(glm::vec4(1.0f)),
        Thickness(thickness) {};
};
struct RectDrawArgs {
    int EntityID;
    const glm::vec3& Point1;
    const glm::vec3& Point2;
    const glm::vec3& Point3;
    const glm::vec3& Point4;
    const glm::vec4& Colour;
    float Thickness;

    RectDrawArgs(int entityID,
                 const glm::vec3& pos1,
                 const glm::vec3& pos2,
                 const glm::vec3& pos3,
                 const glm::vec3& pos4,
                 const glm::vec4& colour = glm::vec4(1.0f),
                 float thickness = 1.0f) :
        EntityID(entityID),
        Point1(pos1),
        Point2(pos2),
        Point3(pos3),
        Point4(pos4),
        Colour(colour),
        Thickness(thickness) {};
    RectDrawArgs(int entityID,
                 const glm::vec3& pos1,
                 const glm::vec3& pos2,
                 const glm::vec3& pos3,
                 const glm::vec3& pos4,
                 float thickness) :
        EntityID(entityID),
        Point1(pos1),
        Point2(pos2),
        Point3(pos3),
        Point4(pos4),
        Colour(glm::vec4(1.0f)),
        Thickness(thickness) {};
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
    static void DrawCircle(const CircleDrawArgs&& drawArgs);
    static void DrawLine(const LineDrawArgs&& drawArgs);
    static void DrawRect(const RectDrawArgs&& drawArgs);

private:
    static void BeginBatch();

    static void BeginQuads();
    static void BeginCircles();
    static void BeginLines();

    static void FlushQuads();
    static void FlushCircles();
    static void FlushLines();

public:
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
        uint32_t CircleCount = 0;
        uint32_t LineCount = 0;

        inline uint32_t VertexCount() const {
            return (QuadCount + CircleCount) * 4 + LineCount * 2;
        }
        inline uint32_t IndexCount() const {
            return (QuadCount + CircleCount) * 6;
        }
    };

    static Statistics GetStats();
    static void ResetStats();
};

} // namespace Mist
