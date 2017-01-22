#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 uv;
layout(location = 2) in float tid;
layout(location = 3) in vec4 color;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);
uniform vec2 mouse_pos;

out DATA
{
	vec4 position;
	vec4 uv;
	float tid;
	vec4 color;
} vs_out;


void main()
{
	//vec4 newPos = vec4(position.x + sin(mouse_pos.x), position.y + cos(mouse_pos.y), position.z, position.w);
	gl_Position = projection * view * position;//newPos;
	vs_out.uv = uv;
	vs_out.tid = tid;
	vs_out.position = model * position;
	vs_out.color = color;
}