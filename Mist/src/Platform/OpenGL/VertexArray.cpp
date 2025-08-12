#include "mistpch.h"

#include "VertexArray.h"
#include "OpenGL/Renderer.h"
#include "VertexBufferLayout.h"

namespace Mist {

VertexArray::VertexArray() {
    MS_GLCALL(glGenVertexArrays(1, &m_RendererID));
    Bind();
}

VertexArray::~VertexArray() {
    MS_GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        MS_GLCALL(glEnableVertexAttribArray(i));
        MS_GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(),
                                        (const void*) (long long)offset));
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    MS_GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
    MS_GLCALL(glBindVertexArray(0));
}

} // namespace Mist