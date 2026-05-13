#version 450 core

layout(location = 0) out vec4 o_Colour;
layout(location = 1) out int o_Entity;

struct VertexOutput {
	vec3	Position;
	vec4	Colour;
	float	Thickness;
};

layout(location = 0) in VertexOutput	Input;
layout(location = 3) in flat int		v_EntityID;

void main() {
	o_Colour = Input.Colour;
	o_Entity = v_EntityID;
}