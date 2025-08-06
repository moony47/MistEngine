#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_TexCoords;

uniform mat4 u_M;
uniform mat4 u_VP;

void main() {
   gl_Position = u_VP * u_M * position;
   v_TexCoords = texCoords;
}