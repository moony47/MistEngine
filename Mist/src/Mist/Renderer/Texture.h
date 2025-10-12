#pragma once

namespace Mist {

class Texture {
public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot) = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetData(void* data, uint32_t size) = 0;
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
    Ref<Texture2D> Create(const std::string& name, uint32_t width, uint32_t height);
    void Remove(const std::string& name);

    inline static Texture2DLibrary* GetInstance() {
        return s_Instance;
    }

protected:
    virtual void Unbind_Impl(uint32_t slot) = 0;
    virtual Ref<Texture2D> Create_Impl(const std::string& path) = 0;
    virtual Ref<Texture2D> Create_Impl(uint32_t width, uint32_t height) = 0;

private:
    inline bool Exists(const std::string& name) const {
        return m_Textures.find(name) != m_Textures.end();
    }

private:
    static Texture2DLibrary* s_Instance;

    std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
};

#define MIST_TEXTURE2DLIB Texture2DLibrary::GetInstance()
#define MIST_TEXTURE2D(x) Texture2DLibrary::GetInstance()->Get(x)

} // namespace Mist