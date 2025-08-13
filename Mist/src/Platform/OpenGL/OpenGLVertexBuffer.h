#pragma once

#include "Mist/Renderer/Buffer.h"

namespace Mist {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const float* data, size_t size);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

private:
    unsigned int m_RendererID;
};

} // namespace Mist