#pragma once

#include <GLAD/glad.h>

#include "Renderer.h"

namespace Mist {

struct VertexBufferLayoutElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalised;

    VertexBufferLayoutElement(unsigned int t, unsigned int c, unsigned char n) :
        type(t),
        count(c),
        normalised(n) {
    }

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLbyte);
        }
        MS_ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferLayoutElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() :
        m_Stride(0) {
    }

    template <typename T> void Push(unsigned int count) {
        static_assert(false);
    }

    template <> void Push<float>(unsigned int count) {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
    }

    template <> void Push<unsigned int>(unsigned int count) {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template <> void Push<unsigned char>(unsigned int count) {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferLayoutElement> GetElements() const {
        return m_Elements;
    }
    inline unsigned int GetStride() const {
        return m_Stride;
    }
};

} // namespace Mist