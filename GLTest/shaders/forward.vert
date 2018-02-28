#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _uv0;
layout(location = 2) in vec2 _uv1;
layout(location = 3) in vec2 _uv2;
layout(location = 4) in vec2 _uv3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv0;
out vec2 uv1;
out vec2 uv2;
out vec2 uv3;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	uv0 = _uv0;
	uv1 = _uv1;
	uv2 = _uv2;
	uv3 = _uv3;
}