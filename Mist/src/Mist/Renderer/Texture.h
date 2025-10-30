#pragma once

#include "glm/glm.hpp"

namespace Mist {

class Texture2D {
public:
    virtual ~Texture2D() = default;

    virtual void Bind(uint32_t slot) = 0;
    virtual void SetData(void* data, uint32_t size) = 0;

    virtual const glm::vec2& GetTexCoords(size_t index) const = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    float GetAspectRatio() const {
        return (float)GetWidth() / (float)GetHeight();
    }
};

class SubTexture2D : Texture2D {
public:
    SubTexture2D(const std::string& textureName,
                 const glm::vec2& coords,
                 const glm::vec2& cellSize,
                 const glm::vec2& spriteSize);

    // Forward all Texture2D methods to wrapped Texture2D instance
    inline void Bind(uint32_t slot) override {
        m_SourceTexture->Bind(slot);
    }
    inline void SetData(void* data, uint32_t size) override {
        MIST_ASSERT(false, "[SubTexture2D::SetData] Cannot set data of a SubTexture2D");
    }

    inline const glm::vec2& GetTexCoords(size_t index) const override {
        return m_TexCoords[index];
    }

    inline uint32_t GetWidth() const override {
        return (uint32_t)(m_SourceTexture->GetWidth() * (m_TexCoords[2].x - m_TexCoords[0].x));
    }
    inline uint32_t GetHeight() const override {
        return (uint32_t)(m_SourceTexture->GetHeight() * (m_TexCoords[2].y - m_TexCoords[0].y));
    }

    inline uint32_t GetRendererID() const override {
        return m_SourceTexture->GetRendererID();
    }

private:
    Ref<Texture2D> m_SourceTexture;
    glm::vec2 m_TexCoords[4];
};

class Texture2DLibrary {
public:
    void Bind(const std::string& name, uint32_t slot);
    void Unbind(uint32_t slot);

    Ref<Texture2D> Get(const std::string& name);

    Ref<Texture2D> Create(const std::string& name, const std::string& path);
    Ref<Texture2D> Create(const std::string& name, uint32_t width, uint32_t height);
    Ref<Texture2D> CreateSub(const std::string& subTextureName,
                             const std::string& textureName,
                             const glm::vec2& coords,
                             const glm::vec2& cellSize,
                             const glm::vec2& spriteSize = {1, 1});

    void Remove(const std::string& name, bool removeSubTextures = false);
    
    inline const std::string& GetSourceTexture(const std::string& name) {
        std::string& source = m_Textures[name].second;
        if (source.empty())
            return name;
        return GetSourceTexture(source);
    }

    inline auto Begin() {
        return m_Textures.begin();
    }
    inline auto End() {
        return m_Textures.end();
    }

    inline static Texture2DLibrary* GetInstance() {
        return s_Instance;
    }

protected:
    virtual void Unbind_Impl(uint32_t slot) = 0;
    virtual Ref<Texture2D> Create_Impl(const std::string& path) = 0;
    virtual Ref<Texture2D> Create_Impl(uint32_t width, uint32_t height) = 0;

private:
    inline bool TextureExists(const std::string& name) const {
        return m_Textures.contains(name);
    }

private:
    static Texture2DLibrary* s_Instance;

    std::unordered_map<std::string, std::pair<Ref<Texture2D>, std::string>> m_Textures;
};

#define MIST_TEXLIB Texture2DLibrary::GetInstance()
#define MIST_TEX(x) Texture2DLibrary::GetInstance()->Get(x)

} // namespace Mist