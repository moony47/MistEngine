#pragma once
#include <cstdint>

namespace Mist {

enum class ShaderDataType {
    None = 0,
    Mat3,
    Mat4,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static size_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Mat3:
            return 3 * 3 * sizeof(float);
        case ShaderDataType::Mat4:
            return 4 * 4 * sizeof(float);
        case ShaderDataType::Float:
            return sizeof(float);
        case ShaderDataType::Float2:
            return 2 * sizeof(float);
        case ShaderDataType::Float3:
            return 3 * sizeof(float);
        case ShaderDataType::Float4:
            return 4 * sizeof(float);
        case ShaderDataType::Int:
            return sizeof(int);
        case ShaderDataType::Int2:
            return 2 * sizeof(int);
        case ShaderDataType::Int3:
            return 3 * sizeof(int);
        case ShaderDataType::Int4:
            return 4 * sizeof(int);
        case ShaderDataType::Bool:
            return sizeof(bool);
    }

    MIST_ASSERT(false, "Unrecognised ShaderDataType");
    return 0;
}

struct BufferElement {
    std::string Name;
    ShaderDataType Type;
    size_t Size;
    uint32_t Offset;
    bool Normalised;

    BufferElement() :
        Name("Buffer"),
        Type(ShaderDataType::None),
        Size(ShaderDataTypeSize(ShaderDataType::None)),
        Offset(0),
        Normalised(false) {
    }

    BufferElement(ShaderDataType type, const std::string& name, bool normalised = false) :
        Name(name),
        Type(type),
        Size(ShaderDataTypeSize(type)),
        Offset(0),
        Normalised(normalised) {
    }

    uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Int:
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::Float2:
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Float3:
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Float4:
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Mat3:
                return 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4;
        }

        MIST_ASSERT(false, "Unrecognised ShaderDataType");
        return 0;
    }
};

class BufferLayout {
public:
    BufferLayout() {
    }
    BufferLayout(const std::initializer_list<BufferElement>& elements) :
        m_Elements(elements) {
        CalculateOffsetsAndStride();
    }

    inline std::vector<BufferElement> GetElements() const {
        return m_Elements;
    }

    inline uint32_t GetStride() const {
        return m_Stride;
    }

    std::vector<BufferElement>::iterator begin() {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::iterator end() {
        return m_Elements.end();
    }
    std::vector<BufferElement>::const_iterator begin() const {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const {
        return m_Elements.end();
    }

private:
    void CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto& elem : m_Elements) {
            elem.Offset = offset;
            offset += (uint32_t)elem.Size;
            m_Stride += (uint32_t)elem.Size;
        }
    }

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& GetLayout() const = 0;

    static Ref<VertexBuffer> Create(const float* vertices, size_t size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual size_t GetCount() const = 0;

    static Ref<IndexBuffer> Create(const uint32_t* indices, size_t count);
};

} // namespace Mist