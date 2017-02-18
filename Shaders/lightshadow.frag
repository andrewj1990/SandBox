#version 330 core

layout (location = 0) out vec4 color;

uniform bool outline;

uniform vec4 colour;
uniform vec2 light_pos;
//uniform float light_intensity;

in DATA
{
	vec4 position;
	vec4 uv;
	float tid;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	float a = 0.0;
	float radius = fs_in.uv.z;
	float distance = length(fs_in.position.xy - fs_in.uv.xy);

	float att = clamp(1.0 - (distance*distance)/(radius*radius), 0.0, 1.0); 
	att *= att;

	color = vec4(fs_in.color.rgb, att);
}
