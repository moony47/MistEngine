#include "mistpch.h"

#include "OpenGLVertexBuffer.h"

#include "OpenGL/OpenGLRenderer.h"
#include <GLAD/glad.h>

namespace Mist {
OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) :
    m_Layout({}) {
    PROFILE_FUNCTION();

    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, size_t size) :
    m_Layout({}) {
    PROFILE_FUNCTION();

    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    PROFILE_FUNCTION();

    MIST_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void OpenGLVertexBuffer::Bind() const {
    PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void OpenGLVertexBuffer::Unbind() const {
    PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::UpdateBuffer(const void* data, size_t size) {
    PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

} // namespace Mist