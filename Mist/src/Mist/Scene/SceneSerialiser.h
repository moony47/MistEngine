#pragma once

#include "Scene.h"

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
    Ref<Scene> m_Scene;
};

} // namespace Mist