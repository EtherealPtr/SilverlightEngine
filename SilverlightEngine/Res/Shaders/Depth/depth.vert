#version 450

layout (location = 0) in vec3 in_vertex_position; 

layout (set = 0, binding = 0) uniform ShadowMapBuffer
{
	mat4 lightSpaceMatrix;
} u_ShadowMapData;

layout (push_constant) uniform PushConstants
{
	mat4 model;
} u_PushConstants;

void main()
{
	gl_Position = u_ShadowMapData.lightSpaceMatrix * u_PushConstants.model * vec4(in_vertex_position, 1.0f);
}