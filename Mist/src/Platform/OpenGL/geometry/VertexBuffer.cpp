#include "mistpch.h"

#include "VertexBuffer.h"

#include <GLAD/glad.h>
#include "Renderer.h"

namespace Mist {

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    MS_GLCALL(glGenBuffers(1, &m_RendererID));
    MS_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MS_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    MS_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
    MS_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    MS_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // namespace Mist