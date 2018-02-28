#version 330 core

struct TextureSettings
{
	sampler2D id;
	vec2 scale;
	vec2 shift;
	float intensity;
};

struct DirLight
{
	float ambient;
	vec3 color;
	vec3 pos;
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

uniform vec4 color;
uniform TextureSettings textures[MAX_TEX_COUNT];
uniform DirLight dirLight;

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
	
	vec3 ambient = dirLight.ambient * dirLight.color;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(dirLight.pos - fPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * dirLight.color;

	result = (t0 + t1 + t2 + t3) * vec4(ambient + diffuse, 1.0f) * color * vColor;
}