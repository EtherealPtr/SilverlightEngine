#version 450

layout (location = 0) in vec3 in_vertex_position;
layout (location = 0) out vec3 out_vertex_texCoord;

layout (set = 0, binding = 0) uniform ViewProjBuffer
{
	mat4 view;
	mat4 proj;
} u_ViewProj;

void main()
{
	mat4 view = mat4(mat3(u_ViewProj.view));
	vec4 pos = u_ViewProj.proj * view * vec4(in_vertex_position, 1.0);
	
	out_vertex_texCoord = in_vertex_position;
	gl_Position = vec4(pos.xyw, pos.w);
}