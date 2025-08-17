#include "mistpch.h"

#include "OpenGLVertexBuffer.h"

#include <GLAD/glad.h>
#include "OpenGL/OpenGLRenderer.h"

namespace Mist {

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, size_t size) :
    m_Layout({}) {
    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    MIST_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void OpenGLVertexBuffer::Bind() const {
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void OpenGLVertexBuffer::Unbind() const {
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // namespace Mist