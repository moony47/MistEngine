#pragma once

#include <GL/glew.h>

#include "glm/glm.hpp"
//#include "geometry/VertexArray.h"
//#include "geometry/IndexBuffer.h"
//#include "shading/Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class Shader;

class Renderer {
private:

public:
    void Clear();
    void Draw(const VertexArray& va, const Shader& shader, unsigned int count) const;
};