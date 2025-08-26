#include "mistpch.h"

#include "OpenGLAssets.h"

#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

namespace Mist {

void OpenGLShaderLibrary::Unbind_Impl() {
    MIST_GLCALL(glUseProgram(0));
}

Ref<Shader> OpenGLShaderLibrary::Create_Impl(const std::string& name,
                                             const std::string& vertShaderPath,
                                             const std::string& fragShaderPath) {
    return std::make_shared<OpenGLShader>(name, vertShaderPath, fragShaderPath);
}

void OpenGLTexture2DLibrary::Unbind_Impl(uint32_t slot) {
    MIST_GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    MIST_GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Ref<Texture2D> OpenGLTexture2DLibrary::Create_Impl(const std::string& name, const std::string& path) {
    return std::make_shared<OpenGLTexture2D>(name, path);
}

Ref<Texture2D> OpenGLTexture2DLibrary::Create_Impl(const std::string& name,
                                                               uint32_t width, uint32_t height) {
    return std::make_shared<OpenGLTexture2D>(name, width, height);
}

} // namespace Mist