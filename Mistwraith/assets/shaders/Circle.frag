#version 450 core

layout(location = 0) out vec4 o_Colour;
layout(location = 1) out int o_Entity;

struct VertexOutput {
	vec3	LocalPosition;
	vec4	Colour;
	float	Thickness;
	float	Fade;
};

layout(location = 0) in VertexOutput	Input;
layout(location = 4) in flat int		v_EntityID;

void main() {
	float distance = 1.0 - length(Input.LocalPosition);
	float alpha = smoothstep(0.0, Input.Fade, distance) * smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);
	
	//if (alpha <= 0.1)
	//	discard;

	o_Colour = Input.Colour;
	o_Colour.a *= alpha;

	o_Entity = v_EntityID;

}