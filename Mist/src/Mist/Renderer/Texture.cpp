#include "mistpch.h"

#include "Texture.h"

#include "OpenGL/OpenGLAssets.h"

namespace Mist {

SubTexture2D::SubTexture2D(const std::string& textureName,
                           const glm::vec2& coords,
                           const glm::vec2& cellSize,
                           const glm::vec2& spriteSize) :
    m_SourceTexture(MIST_TEX(textureName)) {

    glm::vec2 sheetSize{m_SourceTexture->GetWidth(), m_SourceTexture->GetHeight()};
    glm::vec2 min = (coords * cellSize) / sheetSize;
    glm::vec2 max = ((spriteSize + coords) * cellSize) / sheetSize;

    m_TexCoords[0] = {min.x, min.y};
    m_TexCoords[1] = {max.x, min.y};
    m_TexCoords[2] = {max.x, max.y};
    m_TexCoords[3] = {min.x, max.y};
}

Texture2DLibrary* Texture2DLibrary::s_Instance = new OpenGLTexture2DLibrary;

void Texture2DLibrary::Bind(const std::string& name, uint32_t slot) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(TextureExists(name), "[Texture2DLibrary::Bind] Texture name not found");
    m_Textures[name].first->Bind(slot);
}

void Texture2DLibrary::Unbind(uint32_t slot) {
    MIST_PROFILE_FUNCTION();

    Unbind_Impl(slot);
}

Ref<Texture2D> Texture2DLibrary::Get(const std::string& name) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(TextureExists(name), "[Texture2DLibrary::Get] Texture name not found");

    return m_Textures[name].first;
}
Ref<Texture2D> Texture2DLibrary::Create(const std::string& name, const std::string& path) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(!TextureExists(name), "[Texture2DLibrary::Create] Texture name already in used");

    Ref<Texture2D> texture = Create_Impl(path);
    m_Textures[name] = std::make_pair(texture, "");
    return texture;
}

Ref<Texture2D> Texture2DLibrary::Create(const std::string& name, uint32_t width, uint32_t height) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(!TextureExists(name), "[Texture2DLibrary::Create] Texture name already in used");

    Ref<Texture2D> texture = Create_Impl(width, height);
    m_Textures[name] = std::make_pair(texture, "");
    return texture;
}

Ref<Texture2D> Texture2DLibrary::CreateSub(const std::string& subTextureName,
                                           const std::string& textureName,
                                           const glm::vec2& coords,
                                           const glm::vec2& cellSize,
                                           const glm::vec2& spriteSize) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(TextureExists(textureName), "[Texture2DLibrary::CreateSub] Texture name not found");
    MIST_CORE_ASSERT(!TextureExists(subTextureName), "[Texture2DLibrary::CreateSub] Texture name already in used");

    Ref<Texture2D> subTexture = Ref<Texture2D>((Texture2D*)new SubTexture2D(textureName, coords, cellSize, spriteSize));
    m_Textures[subTextureName] = std::make_pair(subTexture, textureName);
    return subTexture;
}

void Texture2DLibrary::Remove(const std::string& name, bool removeSubTextures) {
    MIST_PROFILE_FUNCTION();

    MIST_CORE_ASSERT(TextureExists(name), "[Texture2DLibrary::Remove] Texture name not found");

    m_Textures.erase(name);

    if (removeSubTextures) {
        std::vector<std::string> dependents;
        for (auto& texture : m_Textures)
            if (texture.second.second == name)
                dependents.push_back(texture.first);
        for (std::string& dependent : dependents)
            Remove(dependent, true);
    }
}

} // namespace Mist
