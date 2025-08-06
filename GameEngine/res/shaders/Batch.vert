#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colour;
layout(location = 3) in float texIndex;

out vec2 v_TexCoords;
out vec4 v_Colour;
flat out int v_TexIndex;

uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoords = texCoords;
   v_Colour = colour;
	v_TexIndex = int(texIndex);
}