#pragma once

#include <GLAD/glad.h>
#include "glm/glm.hpp"

#define MS_ASSERT(x) if (!(x)) __debugbreak()
#define MS_GLCALL(x) \
    ClearGLErrors();\
    x;\
    MS_ASSERT(LogGLCall(#x, __FILE__, __LINE__))

namespace Mist {

void ClearGLErrors();
bool LogGLCall(const char* function, const char* file, int line);

class VertexArray;
class Shader;

class Renderer {
private:
public:
    void Clear();
    void Draw(const VertexArray& va, const Shader& shader, unsigned int count) const;
};

} // namespace Mist