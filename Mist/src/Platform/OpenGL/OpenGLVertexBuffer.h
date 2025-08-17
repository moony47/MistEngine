#pragma once

#include "Mist/Renderer/Buffer.h"

namespace Mist {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const float* data, size_t size);
    ~OpenGLVertexBuffer() override;

    virtual void SetLayout(const BufferLayout& layout) override {
        m_Layout = layout;
    }
    virtual const BufferLayout& GetLayout() const override {
        return m_Layout;
    }

    void Bind() const override;
    void Unbind() const override;

private:
    unsigned int m_RendererID;
    BufferLayout m_Layout;
};

} // namespace Mist