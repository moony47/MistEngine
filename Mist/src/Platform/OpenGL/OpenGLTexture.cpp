#include "mistpch.h"

#include "OpenGLTexture.h"
#include "Mist/Renderer/ShaderController.h"

#include "stb_image.h"

namespace Mist {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
		m_RendererID(0),
		m_Filepath(path),
		m_LocalBuffer(nullptr),
		m_Width(0),
		m_Height(0),
		m_BPP(0),
		m_Slot(0) {

		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		MIST_GLCALL(glGenTextures(1, &m_RendererID));
		MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		MIST_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		MIST_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		delete[] m_LocalBuffer;
		ShaderController::GetInstance()->DeregisterTexture(m_RendererID);
		MIST_GLCALL(glDeleteTextures(1, &m_RendererID));
	}

	void OpenGLTexture2D::Bind(unsigned int slot) {
		if (ShaderController::GetInstance()->BindTexture(slot, m_RendererID))
			m_Slot = slot;
	}

	void OpenGLTexture2D::Unbind() {
		if (m_Slot != -1) {
			ShaderController::GetInstance()->UnbindTexture((unsigned int)m_Slot);
			m_Slot = -1;
		}
	}

} // namespace Mist