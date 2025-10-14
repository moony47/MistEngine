#include "mistpch.h"

#include "OpenGLVertexArray.h"
#include "OpenGLRenderer.h"

namespace Mist {

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		}
		MIST_ASSERT(false, "Unrecognised ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		MIST_PROFILE_FUNCTION();

		MIST_GLCALL(glCreateVertexArrays(1, &m_RendererID));
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		MIST_PROFILE_FUNCTION();

		MIST_GLCALL(glDeleteVertexArrays(1, &m_RendererID));
	}

	void OpenGLVertexArray::Bind() const
	{
		MIST_PROFILE_FUNCTION();

		MIST_GLCALL(glBindVertexArray(m_RendererID));
	}

	void OpenGLVertexArray::Unbind() const
	{
		MIST_PROFILE_FUNCTION();

		MIST_GLCALL(glBindVertexArray(0));
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		MIST_PROFILE_FUNCTION();

		MIST_CORE_ASSERT(vb->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		m_VertexBuffers.push_back(vb);

		MIST_GLCALL(glBindVertexArray(m_RendererID));
		vb->Bind();

		uint32_t index = 0;
		for (const auto& elem : vb->GetLayout()) {
			MIST_GLCALL(glEnableVertexAttribArray(index));
            switch (elem.Type) {
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                    MIST_GLCALL(
                        glVertexAttribIPointer(index, elem.GetComponentCount(),
                                                      ShaderDataTypeToOpenGLType(elem.Type),
                                                      vb->GetLayout().GetStride(),
                                                      (const void*)(uint64_t)elem.Offset));
                    break;
                default:
                    MIST_GLCALL(glVertexAttribPointer(index, elem.GetComponentCount(),
                                                      ShaderDataTypeToOpenGLType(elem.Type),
                                                      elem.Normalised ? GL_TRUE : GL_FALSE, vb->GetLayout().GetStride(),
                                                      (const void*)(uint64_t)elem.Offset));
                    break;
			}
			
			
			index++;
		}

		vb->Unbind();
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		MIST_PROFILE_FUNCTION();

		m_IndexBuffer = ib;

		MIST_GLCALL(glBindVertexArray(m_RendererID));
		ib->Bind();

		//ib->Unbind();
	}

} // namespace Mist