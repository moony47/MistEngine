#include "mistpch.h"

#include "Texture.h"

#include "OpenGL/OpenGLAssets.h"

namespace Mist {

Texture2DLibrary* Texture2DLibrary::s_Instance = new OpenGLTexture2DLibrary;

void Texture2DLibrary::Bind(const std::string& name, uint32_t slot) {
    MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Bind] Texture not found");

    m_Textures[name]->Bind(slot);
    m_CurrentTextures[slot] = name;
}
void Texture2DLibrary::Unbind(uint32_t slot) {
    Unbind_Impl(slot);
    m_CurrentTextures[slot] = "";
}

Ref<Texture2D> Texture2DLibrary::Get(const std::string& name) {
    MIST_CORE_ASSERT(Exists(name), "[Texture2DLibrary::Get] Texture not found");

    return m_Textures[name];
}
Ref<Texture2D> Texture2DLibrary::Create(const std::string& name, const std::string& path) {
    MIST_CORE_ASSERT(!Exists(name), "[Texture2DLibrary::Create] Texture already exists");

    Ref<Texture2D> texture = Create_Impl(name, path);
    m_Textures[name] = texture;
    return texture;
}
void Texture2DLibrary::Remove(const std::string& name) {
    MIST_CORE_ASSERT(!Exists(name), "[Texture2DLibrary::Remove] Texture not found");

    for (char i = 0; i < m_CurrentTextures.size(); i++)
        if (name == m_CurrentTextures[i])
            m_Textures[name]->Unbind();
    m_Textures.erase(name);
}

} // namespace Mist
