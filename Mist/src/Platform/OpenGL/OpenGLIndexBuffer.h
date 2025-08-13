#pragma once

#include "Mist/Renderer/Buffer.h"

namespace Mist {

class OpenGLIndexBuffer : public IndexBuffer {
private:
    uint32_t m_RendererID;
    size_t m_Count;

public:
    OpenGLIndexBuffer(const uint32_t* data, size_t count);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    inline size_t GetCount() const override {
        return m_Count;
    }
};

} // namespace Mist