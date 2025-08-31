#include "mistpch.h"

#include "OpenGLVertexBuffer.h"

#include <GLAD/glad.h>
#include "OpenGL/OpenGLRenderer.h"

namespace Mist {

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

void OpenGLVertexBuffer::UpdateBuffer(const float* data, size_t size) {
    PROFILE_FUNCTION();

    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

} // namespace Mist