#include "mistpch.h"

#include "Texture.h"

#include "OpenGL/OpenGLAssets.h"

namespace Mist {

Texture2DLibrary* Texture2DLibrary::s_Instance = new OpenGLTexture2DLibrary;

// void Texture2DLibrary::Bind(const std::string& name, uint32_t slot) {
//     MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Bind] Texture not found");
//
//     if (name == m_CurrentTextures[slot])
//         return;
//
//     m_Textures[name]->Bind(slot);
//     m_CurrentTextures[slot] = name;
// }

void Texture2DLibrary::Bind(const std::string& name, uint32_t slot) {
    PROFILE_FUNCTION();

    MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Bind] Texture not found");
    m_Textures[name]->Bind(slot);
}

void Texture2DLibrary::Unbind(uint32_t slot) {
    PROFILE_FUNCTION();

    Unbind_Impl(slot);
}

Ref<Texture2D> Texture2DLibrary::Get(const std::string& name) {
    PROFILE_FUNCTION();

    MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Get] Texture not found");

    return m_Textures[name];
}
Ref<Texture2D> Texture2DLibrary::Create(const std::string& name, const std::string& path) {
    PROFILE_FUNCTION();

    MIST_CORE_ASSERT(!Exists(name), "[Texture2DLibrary::Create] Texture already exists");

    Ref<Texture2D> texture = Create_Impl(path);
    m_Textures[name] = texture;
    return texture;
}

Ref<Texture2D> Texture2DLibrary::Create(const std::string& name, uint32_t width, uint32_t height) {
    PROFILE_FUNCTION();

    MIST_CORE_ASSERT(!Exists(name), "[Texture2DLibrary::Create] Texture already exists");

    Ref<Texture2D> texture = Create_Impl(width, height);
    m_Textures[name] = texture;
    return texture;
}

void Texture2DLibrary::Remove(const std::string& name) {
    PROFILE_FUNCTION();

    MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Remove] Texture not found");

    m_Textures.erase(name);
}

} // namespace Mist
