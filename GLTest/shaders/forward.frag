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

in vec4 vColor;
in vec2 uv0;
in vec2 uv1;
in vec2 uv2;
in vec2 uv3;
in vec3 normal;
in vec3 fPos;

out vec4 result;

uniform TextureSettings textures[MAX_TEX_COUNT];
uniform Light light;
uniform Material material;
uniform vec3 cameraPos;		

uniform int DirectionalLight;
uniform int PointLight;

vec4 TexColor(int i, vec2 uv)
{
	return texture(textures[i].id, uv / textures[i].scale + textures[i].shift) * textures[i].intensity;
}

void main()
{
	vec4 t0 = TexColor(0, uv0);
	vec4 t1 = TexColor(1, uv1);
	vec4 t2 = TexColor(2, uv2);
	vec4 t3 = TexColor(3, uv3);
	
	result = (t0 + t1 + t2 + t3) * vColor;

	vec3 ambient = vec3(1.0);
	vec3 diffuse = vec3(1.0);
	vec3 specular = vec3(1.0);

	if (PointLight > 0 || DirectionalLight > 0)
	{
		ambient = material.ambient * light.ambientStrength * light.ambient;

		vec3 norm = normalize(normal);
		vec3 dirToLight = normalize(light.pos - fPos); 
		float diff = max(dot(norm, dirToLight), 0.0);
		diffuse = material.diffuse * diff * light.diffuse;

		vec3 dirToCamera = normalize(cameraPos - fPos);
		vec3 reflectDir = reflect(-dirToLight, norm); 
		float spec = pow(max(dot(dirToCamera, reflectDir), 0.0), material.shiness);
		specular = material.specular * spec * light.specular;
	}

	if (PointLight > 0)
	{
		float distance    = length(light.pos - fPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   
	}
	
	result *= vec4(ambient + diffuse + specular, 1.0f);
}