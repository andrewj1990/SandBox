#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;
uniform vec2 step_size;

in DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	vec4 texColor = fs_in.color;
	float alpha = 0.0f;
	float stepSize = 1.0f / 32.0f;
	if (fs_in.tid > 0.0)
	{
		int tid = int(fs_in.tid - 0.5);

		alpha = 8 * texture(textures[tid], fs_in.uv).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(step_size.x, 0.0f)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(-step_size.x, 0.0f)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(0.0f, step_size.y)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(0.0f, -step_size.y)).a;
		
		alpha -= texture(textures[tid], fs_in.uv + vec2(-step_size.x, -step_size.y)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(step_size.x, -step_size.y)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(-step_size.x, step_size.y)).a;
		alpha -= texture(textures[tid], fs_in.uv + vec2(step_size.x, step_size.y)).a;

		texColor = texture(textures[tid], fs_in.uv);
	}

	color = alpha > 0 ? vec4(0, 0, 0, alpha) : texColor;
}
