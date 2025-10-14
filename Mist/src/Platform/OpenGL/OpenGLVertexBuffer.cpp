#include "mistpch.h"

#include "OpenGLVertexBuffer.h"

#include "OpenGL/OpenGLRenderer.h"
#include <GLAD/glad.h>

namespace Mist {
OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) :
    m_Layout({}) {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, size_t size) :
    m_Layout({}) {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void OpenGLVertexBuffer::Bind() const {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void OpenGLVertexBuffer::Unbind() const {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::UpdateBuffer(const void* data, size_t size) {
    MIST_PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

} // namespace Mist