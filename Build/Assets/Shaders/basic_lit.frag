#version 460 core
in VS_OUT
{
	 vec2 texcoord;
	 vec3 color;
}fs_in;
 
in vec3 v_color;
in vec2 v_texcoord;

out vec4 f_color;

uniform struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
} u_material;

void main(){
	f_color = texture(u_material.baseMap, v_texcoord) * vec4(v_color,1);
	//f_color = vec4(v_color,1);
}