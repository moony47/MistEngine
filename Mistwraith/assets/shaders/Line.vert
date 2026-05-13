#version 450 core

layout(location = 0) in vec3	a_Position;
layout(location = 1) in vec4	a_Colour;
layout(location = 2) in float	a_Thickness;
layout(location = 3) in int		a_EntityID;

layout(std140, binding = 0) uniform Camera {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec3	Position;
	vec4	Colour;
	float	Thickness;
};

layout (location = 0) out VertexOutput	Output;
layout (location = 3) out flat int		v_EntityID;

void main() {
	Output.Position = a_Position;
	Output.Colour = a_Colour;
	Output.Thickness = a_Thickness;
	
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}