#version 330 core

in vec2 uv0;
in vec2 uv1;
in vec2 uv2;
in vec2 uv3;

out vec4 result;

struct TextureSettings
{
	sampler2D id;
	vec2 scale;
	vec2 shift;
	float intensity;
};

const int MAX_TEX_COUNT = 4;

uniform vec4 color;
uniform TextureSettings textures[MAX_TEX_COUNT];

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float tex0Intensity;
uniform float tex1Intensity;
uniform vec2 tex0Scale;
uniform vec2 tex1Scale;
uniform vec2 tex0Shift;
uniform vec2 tex1Shift;

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
	
	//vec4 t1 = texture(tex0, uv0 / tex0Scale + tex0Shift) * tex0Intensity;
	//vec4 t2 = texture(tex1, uv1 / tex1Scale + tex1Shift) * tex1Intensity;
	result = (t0 + t1 + t2 + t3);// * color;
}