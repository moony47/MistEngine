#pragma once

#include "Mist/Renderer/Buffer.h"

namespace Mist {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const float* data, size_t size);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    void SetLayout(const BufferLayout& layout) override {
        m_Layout = layout;
    }
    const BufferLayout& GetLayout() const override {
        return m_Layout;
    }

    void UpdateBuffer(const float* data, size_t size) override;

private:
    unsigned int m_RendererID;
    BufferLayout m_Layout;
};

} // namespace Mist