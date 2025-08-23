#version 460 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoords;
in vec4 v_Colour;
flat in int v_TexIndex;

uniform sampler2D u_Texture[2];

void main() {
	vec4 texColour = texture(u_Texture[v_TexIndex], v_TexCoords);
	colour = texColour * v_Colour;
}