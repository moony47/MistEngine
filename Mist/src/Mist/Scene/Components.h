#pragma once
#include <glm/glm.hpp>
#include <string>

namespace Mist {

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) :
        Tag(tag) {
    }
};

struct SpriteComponent {
    std::string TextureName;
    glm::vec4 Colour;

    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(const std::string& textureName = "WHITE", const glm::vec4& colour = {1.0f, 1.0f, 1.0f, 1.0f}) :
        TextureName(textureName),
        Colour(colour) {
    }
    SpriteComponent(const glm::vec4& colour) :
        Colour(colour) {
    }
};

struct TransformComponent {
    glm::vec3 Position;
    float Rotation;
    glm::vec2 Scale;

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position = glm::vec3(0.0f),
                       const float rotation = 0.0f,
                       const glm::vec2& scale = glm::vec2(1.0f)) :
        Position(position),
        Rotation(rotation),
        Scale(scale) {
    }
};

} // namespace Mist