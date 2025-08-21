#pragma once

#include "Mist/Renderer/VertexArray.h"

namespace Mist {

class OpenGLVertexArray : public VertexArray {
private:

public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

	void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
	void SetIndexBuffer(const Ref<IndexBuffer>& ib) override;

	const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
		return m_VertexBuffers;
	}
	const Ref<IndexBuffer> GetIndexBuffer() const override {
		return m_IndexBuffer;
	}

private:
	uint32_t m_RendererID;

	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
	Ref<IndexBuffer> m_IndexBuffer;
};

} // namespace Mist
