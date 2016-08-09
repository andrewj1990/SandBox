#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;

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
	vec4 texColor = fs_in.color;
	float alpha = 0.0f;
	float stepSize = 1.0f / 32.0f;
	if (fs_in.tid > 0.0)
	{
		int tid = int(fs_in.tid - 0.5);

		alpha = 8 * texture(textures[tid], fs_in.uv.xy).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, 0.0f)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, 0.0f)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(0.0f, fs_in.uv.w)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(0.0f, -fs_in.uv.w)).a;
		
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, -fs_in.uv.w)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, -fs_in.uv.w)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, fs_in.uv.w)).a;
		alpha -= texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, fs_in.uv.w)).a;

		texColor = texture(textures[tid], fs_in.uv.xy);
	}

	color = vec4(0, 0, 0, alpha);
}