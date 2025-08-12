#include "mistpch.h"

#include "IndexBuffer.h"

#include "OpenGL/Renderer.h"
#include <GLAD/glad.h>

namespace Mist {

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
    m_Count(count) {
    MS_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    MS_GLCALL(glGenBuffers(1, &m_RendererID));
    MS_GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    MS_GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    MS_GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    MS_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
    MS_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

}
