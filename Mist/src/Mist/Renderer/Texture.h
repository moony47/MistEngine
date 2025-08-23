#pragma once

namespace Mist {

class Texture {
public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot) = 0;
    virtual void Unbind() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetSlot() const = 0;
};

class Texture2D : public Texture {
public:
};

class Texture2DLibrary {
public:
    void Bind(const std::string& name, uint32_t slot);
    void Unbind(uint32_t slot);

    Ref<Texture2D> Get(const std::string& name);
    Ref<Texture2D> Create(const std::string& name, const std::string& path);
    void Remove(const std::string& name);

    inline static Texture2DLibrary* GetInstance() {
        return s_Instance;
    }

protected:
    virtual void Unbind_Impl(uint32_t slot) = 0;
    virtual Ref<Texture2D> Create_Impl(const std::string& name, const std::string& path) = 0;

private:
    inline bool Exists(const std::string& name) const {
        return m_Textures.find(name) != m_Textures.end();
    }

private:
    static Texture2DLibrary* s_Instance;

    std::array<std::string, 8> m_CurrentTextures = {"", "", "", "", "", "", "", ""};
    std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
};

#define MIST_TEXTURE2DLIB Texture2DLibrary::GetInstance()
#define MIST_TEXTURE2D(x) Texture2DLibrary::GetInstance()->Get(x)

} // namespace Mist