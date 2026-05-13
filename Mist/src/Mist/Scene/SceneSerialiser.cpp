#include "mistpch.h"
#include "SceneSerialiser.h"

#include "Components.h"
#include "Entity.h"
#include "Mist/Renderer/Texture.h"
#include "Scene.h"
#include <Mist/Cameras/SceneCamera.h>
#include <Mist/Core/Core.h>
#include <Mist/Core/Logger.h>
#include <Mist/Core/UUID.h>
#include <cstdint>
#include <entt.hpp>
#include <fstream>
#include <glm/fwd.hpp>
#include <sstream>
#include <string>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/emitterstyle.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

using namespace YAML;

namespace YAML {

template <>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs) {
        if (!node.IsSequence() || node.size() != 3)
            return false;
        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
        Node node;
        node.push_back(rhs.r);
        node.push_back(rhs.g);
        node.push_back(rhs.b);
        node.push_back(rhs.a);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs) {
        if (!node.IsSequence() || node.size() != 4)
            return false;
        rhs.r = node[0].as<float>();
        rhs.g = node[1].as<float>();
        rhs.b = node[2].as<float>();
        rhs.a = node[3].as<float>();
        return true;
    }
};

} // namespace YAML

namespace Mist {

Emitter& operator<<(Emitter& out, const glm::vec3& vec) {
    out << Flow << BeginSeq << vec.x << vec.y << vec.z << EndSeq;
    return out;
}
Emitter& operator<<(Emitter& out, const glm::vec4& vec) {
    out << Flow << BeginSeq << vec.r << vec.g << vec.b << vec.a << EndSeq;
    return out;
}

void SceneSerialiser::SerialiseEntity(Emitter& out, Entity entity) {
    MIST_CORE_ASSERT(entity.HasComponent<IDComponent>(),
                     "[SceneSerialiser::SerialiseEntity] Cannot serialise entity with no UUID");

    out << BeginMap;

    out << Key << "Entity" << Value << entity.UUID();
    out << Key << "Name" << Value << entity.Name();

    if (entity.HasComponent<TransformComponent>()) {
        out << Key << "TransformComponent" << BeginMap;
        auto& transform = entity.GetComponent<TransformComponent>();
        out << Key << "Position" << Value << transform.GetPosition();
        out << Key << "Rotation" << Value << transform.GetRotation();
        out << Key << "Scale" << Value << transform.GetScale();
        out << EndMap;
    }

    if (entity.HasComponent<CameraComponent>()) {
        out << Key << "CameraComponent" << BeginMap;
        auto& cameraComp = entity.GetComponent<CameraComponent>();
        out << Key << "FixedAspectRatio" << Value << cameraComp.FixedAspectRatio;
        {
            out << Key << "Camera" << Value << BeginMap;
            auto& camera = cameraComp.Camera;
            out << Key << "Type" << Value << (int)camera.GetType();

            out << Key << "PerspectiveFOV" << Value << camera.GetPerspectiveFOV();
            out << Key << "PerspectiveNear" << Value << camera.GetPerspectiveNear();
            out << Key << "PerspectiveFar" << Value << camera.GetPerspectiveFar();

            out << Key << "OrthographicSize" << Value << camera.GetOrthographicSize();
            out << Key << "OrthographicNear" << Value << camera.GetOrthographicNear();
            out << Key << "OrthographicFar" << Value << camera.GetOrthographicFar();
            out << EndMap;
        }
        out << EndMap;
    }

    if (entity.HasComponent<RenderableComponent>()) {
        auto& renderable = entity.GetComponent<RenderableComponent>();
        Renderable typeIndex = renderable.GetType();

        out << Key << "RenderableComponent" << BeginMap;
        out << Key << "Type" << Value << (size_t)typeIndex;

        switch (typeIndex) {
            case Renderable::Sprite: {
                SpriteComponent& sprite = renderable.GetComponent<SpriteComponent>();
                out << Key << "Colour" << Value << sprite.Colour;
                out << Key << "TilingFactor" << Value << sprite.TilingFactor;
                out << Key << "TextureName" << Value << sprite.TextureName;

                if (!m_TexturePaths.contains(sprite.TextureName))
                    m_TexturePaths[sprite.TextureName] = MIST_TEX(sprite.TextureName)->GetPath();
                break;
            }
            case Renderable::Circle: {
                CircleComponent& circle = renderable.GetComponent<CircleComponent>();
                out << Key << "Colour" << Value << circle.Colour;
                out << Key << "Thickness" << Value << circle.Thickness;
                out << Key << "Fade" << Value << circle.Fade;
                break;
            }
            case Renderable::Line: {
                LineComponent& line = renderable.GetComponent<LineComponent>();
                out << Key << "Point1" << Value << line.Point1;
                out << Key << "Point2" << Value << line.Point2;
                out << Key << "Colour" << Value << line.Colour;
                out << Key << "Thickness" << Value << line.Thickness;
                break;
            }
        }
        out << EndMap;
    }

    out << EndMap;
}

SceneSerialiser::SceneSerialiser(const Ref<Scene>& scene) :
    m_Scene(scene) {};

void SceneSerialiser::Serialise(const std::string& filepath) {

    Emitter out;

    out << BeginMap;
    out << Key << "Scene" << Value << "Untitled";
    out << Key << "PrimaryCamera" << Value
        << (m_Scene->m_PrimaryCameraEntity ? (uint64_t)m_Scene->m_PrimaryCameraEntity->GetComponent<IDComponent>().ID
                                           : 0);
    {
        out << Key << "Entities" << Value << BeginSeq;
        auto view = m_Scene->m_Registry.view<entt::entity>();
        for (auto iter = view.rbegin(); iter != view.rend(); iter++) {
            Entity entity = {m_Scene.get(), *iter};
            if (!entity)
                return;
            SerialiseEntity(out, entity);
        }
        out << EndSeq;
    }
    {
        out << Key << "Textures" << Value << BeginSeq;
        for (auto& [name, path] : m_TexturePaths) {
            if (path.string().empty())
                continue;
            out << BeginMap;
            out << Key << "Name" << Value << name;
            out << Key << "Path" << Value << path.string();
            out << EndMap;
        }
        out << EndSeq;
    }
    out << EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}

void SceneSerialiser::SerialiseRuntime(const std::string& filepath) {
    // TODO
    MIST_CORE_ASSERT(false, "Runtime serialisation not implemented");
}

bool SceneSerialiser::Deserialise(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::stringstream ss;
    ss << stream.rdbuf();

    Node data = Load(ss.str());
    if (!data["Scene"])
        return false;

    std::string sceneName = data["Scene"].as<std::string>();
    MIST_CORE_TRACE("Deserialising scene: {0}", sceneName);

    UUID primaryCameraUUID = data["PrimaryCamera"].as<uint64_t>();

    Node entities = data["Entities"];
    if (entities)
        for (auto entity : entities) {
            UUID uuid = entity["Entity"].as<uint64_t>();
            std::string name = entity["Name"].as<std::string>();

            MIST_CORE_TRACE("Deserialising entity: Name = {0}, UUID = {1}", name, uuid);

            Entity deserialisedEntity = m_Scene->CreateEntity(uuid, name);

            if (Node transformNode = entity["TransformComponent"]) {
                auto& transformComp = deserialisedEntity.GetComponent<TransformComponent>();
                transformComp.SetPosition(transformNode["Position"].as<glm::vec3>());
                transformComp.SetRotation(transformNode["Rotation"].as<glm::vec3>());
                transformComp.SetScale(transformNode["Scale"].as<glm::vec3>());
            }

            if (Node cameraNode = entity["CameraComponent"]) {
                auto& cameraComp = deserialisedEntity.AddComponent<CameraComponent>();
                auto& camera = cameraComp.Camera;

                auto props = cameraNode["Camera"];
                camera.SetType((SceneCamera::CameraType)props["Type"].as<int>());

                camera.SetPerspectiveFOV(props["PerspectiveFOV"].as<float>());
                camera.SetPerspectiveNear(props["PerspectiveNear"].as<float>());
                camera.SetPerspectiveFar(props["PerspectiveFar"].as<float>());

                camera.SetOrthographicSize(props["OrthographicSize"].as<float>());
                camera.SetOrthographicNear(props["OrthographicNear"].as<float>());
                camera.SetOrthographicFar(props["OrthographicFar"].as<float>());

                cameraComp.FixedAspectRatio = cameraNode["FixedAspectRatio"].as<bool>();

                if (primaryCameraUUID == uuid)
                    m_Scene->SetPrimaryCamera(deserialisedEntity);
            }

            if (Node renderableNode = entity["RenderableComponent"]) {
                Renderable type = (Renderable)renderableNode["Type"].as<size_t>();
                RenderableComponent& renderable = deserialisedEntity.AddComponent<RenderableComponent>();
                switch (type) {
                    case Renderable::Sprite: {
                        SpriteComponent& spriteComp = renderable.SetComponent<SpriteComponent>();
                        spriteComp.Colour = renderableNode["Colour"].as<glm::vec4>();
                        spriteComp.TilingFactor = renderableNode["TilingFactor"].as<float>();
                        spriteComp.TextureName = renderableNode["TextureName"].as<std::string>();
                        break;
                    }
                    case Renderable::Circle: {
                        CircleComponent& circleComp = renderable.SetComponent<CircleComponent>();
                        circleComp.Colour = renderableNode["Colour"].as<glm::vec4>();
                        circleComp.Thickness = renderableNode["Thickness"].as<float>();
                        circleComp.Fade = renderableNode["Fade"].as<float>();
                        break;
                    }
                    case Renderable::Line: {
                        LineComponent& lineComp = renderable.SetComponent<LineComponent>();
                        lineComp.Point1 = renderableNode["Point1"].as<glm::vec3>();
                        lineComp.Point2 = renderableNode["Point2"].as<glm::vec3>();
                        lineComp.Colour = renderableNode["Colour"].as<glm::vec4>();
                        lineComp.Thickness = renderableNode["Thickness"].as<float>();
                        break;
                    }
                }
            }
        }

    Node textures = data["Textures"];
    if (textures)
        for (auto texture : textures) {
            std::string texName = texture["Name"].as<std::string>();
            if (!MIST_TEXLIB->Exists(texName))
                MIST_TEXLIB->Create(texName, texture["Path"].as<std::string>());
        }

    return true;
}

bool SceneSerialiser::DeserialiseRuntime(const std::string& filepath) {
    // TODO
    MIST_CORE_ASSERT(false, "Runtime serialisation not implemented");
    return false;
}

} // namespace Mist