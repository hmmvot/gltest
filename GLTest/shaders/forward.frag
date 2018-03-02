#version 330 core

struct TextureSettings
{
	sampler2D id;
	vec2 scale;
	vec2 shift;
	float intensity;
};

struct DirectionalLight
{
	vec3 dir;

	float ambientStrength;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
	vec3 pos;

	float ambientStrength;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

const int MAX_TEX_COUNT = 4;
const int MAX_POINT_LIGHTS = 32;

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

uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int actualPointLights;

vec4 TexColor(int i, vec2 uv)
{
	return texture(textures[i].id, uv / textures[i].scale + textures[i].shift) * textures[i].intensity;
}

vec3 CalculateDirectionalLight(DirectionalLight light)
{
	vec3 ambient = material.ambient * light.ambientStrength * light.ambient;

	vec3 norm = normalize(normal);
	vec3 dirToLight = normalize(-light.dir); 
	float diff = max(dot(norm, dirToLight), 0.0);
	vec3 diffuse = material.diffuse * diff * light.diffuse;

	vec3 dirToCamera = normalize(cameraPos - fPos);
	vec3 reflectDir = reflect(-dirToLight, norm); 
	float spec = pow(max(dot(dirToCamera, reflectDir), 0.0), material.shiness);
	vec3 specular = material.specular * spec * light.specular;

	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light)
{
    vec3 lightDir = normalize(light.pos - fPos);
    // diffuse shading
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    // specular shading
	vec3 dirToCamera = normalize(cameraPos - fPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0.0), material.shiness);
    // attenuation
    float distance    = length(light.pos - fPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient;//(ambient + diffuse + specular);
}

void main()
{
	vec4 t0 = TexColor(0, uv0);
	vec4 t1 = TexColor(1, uv1);
	vec4 t2 = TexColor(2, uv2);
	vec4 t3 = TexColor(3, uv3);
	
	result = (t0 + t1 + t2 + t3) * vColor;

	vec3 lightColor = CalculateDirectionalLight(dirLight);
	for (int i = 0; i < min(MAX_POINT_LIGHTS, actualPointLights); ++i)
	{
		lightColor += CalculatePointLight(pointLights[i]);
	}
	
	result *= vec4(lightColor, 1.0);
}