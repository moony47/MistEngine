#pragma once

#include "Entity.h"
#include "Scene.h"

#include <filesystem>

#include <yaml-cpp/yaml.h>

namespace Mist {

class SceneSerialiser {
public:
    SceneSerialiser(const Ref<Scene>& scene);

    void Serialise(const std::string& filepath);
    void SerialiseRuntime(const std::string& filepath);

    bool Deserialise(const std::string& filepath);
    bool DeserialiseRuntime(const std::string& filepath);

private:
    void SerialiseEntity(YAML::Emitter& out, Entity entity);

private:
    Ref<Scene> m_Scene;

    std::unordered_map<std::string, std::filesystem::path> m_TexturePaths;
};

} // namespace Mist