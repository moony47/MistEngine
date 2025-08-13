#pragma once
#include <cstdint>

namespace Mist {

class VertexBuffer {
public:
    virtual ~VertexBuffer() {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static VertexBuffer* Create(const float* vertices, size_t size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual size_t GetCount() const = 0;

    static IndexBuffer* Create(const uint32_t* indices, size_t size);
};

} // namespace Mist