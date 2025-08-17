#version 460 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_VP;
uniform mat4 u_M;

void main() {
   gl_Position = u_VP * u_M * a_Position;
   v_TexCoords = a_TexCoords;
}