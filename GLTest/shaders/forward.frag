#version 330 core

struct TextureSettings
{
	sampler2D id;
	vec2 scale;
	vec2 shift;
	float intensity;
};

struct Light
{
	int type; // 0 - directional, 1 - point, 2 - flashlight

	vec3 pos;
	vec3 dir;

	float ambientStrength;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;

	float cutoffIn;
	float cutoffOut;

	float intensity;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

const int MAX_TEX_COUNT = 4;
const int MAX_LIGHTS = 32;

in vec4 vColor;
in vec2 uv0;
in vec2 uv1;
in vec2 uv2;
in vec2 uv3;
in vec3 normal;
in vec3 fPos;

out vec4 result;

uniform TextureSettings textures[MAX_TEX_COUNT];
uniform Material material;
uniform vec3 cameraPos;		

uniform Light lights[MAX_LIGHTS];
uniform int actualLightsCount;

vec4 TexColor(int i, vec2 uv)
{
	return texture(textures[i].id, uv / textures[i].scale + textures[i].shift) * textures[i].intensity;
}

vec3 CalculateLight(Light light)
{
	vec3 norm = normalize(normal);
    vec3 dirToLight = light.type == 0 ? -normalize(light.dir) : normalize(light.pos - fPos);

    // diffuse shading
    float diff = max(dot(norm, dirToLight), 0.0);

    // specular shading
	vec3 dirToCamera = normalize(cameraPos - fPos);
    vec3 reflectDir = reflect(-dirToLight, norm);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0.0), material.shiness);

    vec3 ambient  = light.ambient  * light.ambientStrength * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
	
    // attenuation
	if (light.type == 1)
	{
		float distance    = length(light.pos - fPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
	}

	float intensity = light.intensity;

	// cutoff
	if (light.type == 2)
	{
		float theta = dot(dirToLight, normalize(-light.dir));
		float epsilon = cos(light.cutoffIn) - cos(light.cutoffOut);
		intensity = intensity * clamp((theta - cos(light.cutoffOut)) / epsilon, 0.0, 1.0);

	}

    return (ambient + diffuse + specular) * intensity;
}

void main()
{
	vec4 t0 = TexColor(0, uv0);
	vec4 t1 = TexColor(1, uv1);
	vec4 t2 = TexColor(2, uv2);
	vec4 t3 = TexColor(3, uv3);
	
	result = (t0 + t1 + t2 + t3) * vColor;

	vec3 lightColor = vec3(0, 0, 0);
	for (int i = 0; i < min(MAX_LIGHTS, actualLightsCount); ++i)
	{
		lightColor += CalculateLight(lights[i]);
	}
	
	result *= vec4(lightColor, 1.0);
}