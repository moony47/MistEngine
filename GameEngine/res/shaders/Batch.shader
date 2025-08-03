#shader vertex
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

#shader fragment
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