#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec4 v_Colour;
out vec2 v_TexCoord;
out flat int v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_VP;

void main() {
	v_Colour = a_Colour;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}