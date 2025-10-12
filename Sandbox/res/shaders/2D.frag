#version 460 core

layout(location = 0) out vec4 colour;

in vec4 v_Colour;
in vec2 v_TexCoord;
in flat int v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];

void main() {
	colour = texture(u_Texture[v_TexIndex], v_TexCoord * v_TilingFactor) * v_Colour;
}