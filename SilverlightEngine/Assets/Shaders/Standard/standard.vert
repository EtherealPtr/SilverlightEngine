#version 450

layout (location = 0) in vec3 in_vertex_position; 
layout (location = 1) in vec2 in_vertex_texCoord;
layout (location = 2) in vec3 in_vertex_normal;

layout (location = 0) out vec2 out_vertex_texCoord;
layout (location = 1) out vec3 out_vertex_normal;
layout (location = 2) out int out_texture_index;
layout (location = 3) out vec3 out_camera_position;
layout (location = 4) out vec3 out_fragment_position;
layout (location = 5) out vec4 out_fragment_light_space_position;

layout (set = 0, binding = 0) uniform ViewProjBuffer
{
	mat4 view;
	mat4 proj;
} u_ViewProj;

layout (push_constant) uniform PushConstants
{
	mat4 model;
	vec3 camera_position;
	int textureId; 
} u_PushConstants;

layout (set = 0, binding = 5) uniform ShadowMapBuffer
{
	mat4 lightSpaceMatrix;
} u_ShadowMapData;

void main()
{
	const vec4 world_position = u_PushConstants.model * vec4(in_vertex_position, 1.0f);
	gl_Position = u_ViewProj.proj * u_ViewProj.view * world_position;

	out_fragment_position = vec3(u_PushConstants.model * vec4(in_vertex_position, 1.0));
	out_vertex_normal = mat3(transpose(inverse(u_PushConstants.model))) * in_vertex_normal; 
	out_vertex_texCoord = in_vertex_texCoord;
	out_texture_index = u_PushConstants.textureId;
	out_camera_position = u_PushConstants.camera_position;
	out_fragment_light_space_position = u_ShadowMapData.lightSpaceMatrix * vec4(out_fragment_position, 1.0);
}