#version 450

layout (location = 0) in vec3 in_vertex_texCoord;
layout (location = 0) out vec4 out_frag_color;

layout (set = 0, binding = 1) uniform samplerCube u_SkyboxSampler;

void main() 
{
	out_frag_color = texture(u_SkyboxSampler, in_vertex_texCoord);
}
