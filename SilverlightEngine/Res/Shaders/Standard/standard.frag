#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec2 in_vertex_texCoord;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) flat in int in_texture_index;
layout (location = 3) in vec3 in_camera_position;
layout (location = 4) in vec3 in_fragment_position;
layout (location = 5) in vec4 in_fragment_light_space_position;

layout (location = 0) out vec4 out_frag_color;

layout (set = 0, binding = 2) uniform texture2D textures[];
layout (set = 0, binding = 3) uniform sampler texture_sampler;

layout (set = 0, binding = 1) uniform Material 
{
    vec4 diffuseColor;
    vec4 specularColor;    // w: shininess
} u_Material;

struct LightData 
{
    vec4 positionAndType;  // xyz: position, w: type
    vec4 direction;        // xyz: direction
    vec4 color;            // xyz: color, w: color intensity
    vec4 attenuation;      // x: constant, y: linear, z: quadratic
    vec4 angles;           // x: inner cutoff angle, y: outer cutoff angle (for spotlight)
};

layout(set = 0, binding = 4) 
readonly buffer LightBuffer 
{ 
    int totalLights; 
    LightData lights[25];
};

layout (set = 0, binding = 6) uniform sampler2D shadowMap;

float CalculateShadow(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir)
{
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;
    projCoords.xy = projCoords.xy * 0.5f + 0.5f;
    float currentDepth = projCoords.z;
    
    if (projCoords.z > 1.0f)
    {
        return 0.0f;
    }

    float bias = max(0.002f * (1.0f - dot(_normal, _lightDir)), 0.0005f);

    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            shadow += currentDepth > (pcfDepth + bias) ? 1.0f : 0.0f;
        }
    }

    shadow /= 9.0f;
    return shadow;
}

vec3 CalculateDirectionalLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;

    vec3 lightDir = normalize(-_light.direction.xyz);
    vec3 halfwayDir = normalize(lightDir + _viewDir);
    float diffImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = diffImpact * _light.color.xyz * lightIntensity;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_normal, halfwayDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * u_Material.specularColor.xyz * lightIntensity;

    return diffuse + specular;
}

vec3 CalculatePointLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;

    vec3 lightDir = normalize(_light.positionAndType.xyz - in_fragment_position);
    vec3 halfwayDir = normalize(lightDir + _viewDir);
    float diffuseImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = diffuseImpact * (_light.color.xyz * lightIntensity);

    float distance = length(_light.positionAndType.xyz - in_fragment_position);
    float attenuation = 1.0f / (_light.attenuation.x + _light.attenuation.y * distance + _light.attenuation.z * (distance * distance));
    diffuse *= attenuation;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_normal, halfwayDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * (u_Material.specularColor.xyz * lightIntensity) * attenuation;

    return diffuse + specular;
}

vec3 CalculateSpotLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;
    float innerCutoff = _light.angles.x; 
    float outerCutoff = _light.angles.y; 

    vec3 lightDir = normalize(_light.positionAndType.xyz - in_fragment_position);
    vec3 halfwayDir = normalize(lightDir + _viewDir);
    float diffuseImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = _light.color.xyz * diffuseImpact * lightIntensity; 

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_normal, halfwayDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * u_Material.specularColor.xyz * lightIntensity; 

    float theta = dot(lightDir, normalize(-_light.direction.xyz));
    float epsilon = innerCutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(_light.positionAndType.xyz - in_fragment_position);
    float attenuation = 1.0f / (_light.attenuation.x + _light.attenuation.y * distance + _light.attenuation.z * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

void main() 
{
    vec4 baseColor = u_Material.diffuseColor;

    if (in_texture_index > 0) 
    {
        vec4 texColor = texture(sampler2D(textures[in_texture_index], texture_sampler), in_vertex_texCoord);
        baseColor = texColor * u_Material.diffuseColor;
    }

    vec3 normal = normalize(in_vertex_normal);
    vec3 viewDir = normalize(in_camera_position - in_fragment_position);
    vec3 lighting = vec3(0.0f);

    const float ambientStrength = 0.1f;
    const vec3 ambient = ambientStrength * baseColor.xyz;

    for (int i = 0; i < totalLights; ++i) 
    {        
        LightData light = lights[i];

        if (light.positionAndType.w == 0.0f) // Directional light
        { 
            const vec3 lightDir = normalize(-light.direction.xyz);
            const float shadow = CalculateShadow(in_fragment_light_space_position, normal, lightDir);
            lighting += (ambient + (1.0 - shadow) * CalculateDirectionalLight(light, normal, viewDir));
        } 
        else if (light.positionAndType.w == 1.0f) // Point light
        { 
            lighting += ambient + CalculatePointLight(light, normal, viewDir);
        }
        else if (light.positionAndType.w == 2.0f) // Spotlight
        {
            lighting += ambient + CalculateSpotLight(light, normal, viewDir);
        }
    }

    vec3 resultColor = baseColor.xyz * lighting;
    out_frag_color = vec4(resultColor, 1.0f);
}
