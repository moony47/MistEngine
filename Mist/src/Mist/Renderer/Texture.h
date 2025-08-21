#pragma once

namespace Mist {

class Texture {
public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot) = 0;
    virtual void Unbind() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

class Texture2D : public Texture {
public:
};

} // namespace Mist