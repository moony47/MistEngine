#include "mistpch.h"

#include "OpenGLIndexBuffer.h"

#include "OpenGL/Renderer.h"
#include <GLAD/glad.h>

namespace Mist {

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, size_t count) :
    m_Count(count) {
    MIST_GLASSERT(sizeof(uint32_t) == sizeof(GLuint));

    MIST_GLCALL(glCreateBuffers(1, &m_RendererID));
    MIST_GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    MIST_GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    MIST_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void OpenGLIndexBuffer::Bind() const {
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void OpenGLIndexBuffer::Unbind() const {
    MIST_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

}
