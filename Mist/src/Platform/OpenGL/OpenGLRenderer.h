#pragma once

#include <GLAD/glad.h>
#include "glm/glm.hpp"

#define MIST_GLASSERT(x) if (!(x)) __debugbreak()
#define MIST_GLCALL(x) \
    ClearGLErrors();\
    x;\
    MIST_GLASSERT(LogGLCall(#x, __FILE__, __LINE__))

namespace Mist {

void ClearGLErrors();
bool LogGLCall(const char* function, const char* file, int line);

class VertexArray;
class Shader;

class OpenGLRenderer {
private:
public:
    void Clear();
    void Draw(const VertexArray& va, const Shader& shader, uint32_t count) const;
};

} // namespace Mist