#shader vertex
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

#shader fragment
#version 460 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoords;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main() {
	vec4 texColour = texture(u_Texture, v_TexCoords);
	colour = texColour * u_Colour;
}