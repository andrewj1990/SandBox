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
	vec4 texColor = fs_in.color;
	float alpha = 0.0f;
	float sides = 0.0f;
	float stepSize = 1.0f / 32.0f;
	if (fs_in.tid > 0.0)
	{
		int tid = int(fs_in.tid - 0.5);
		
		if (outline)
		{		
			alpha = (1 - texture(textures[tid], fs_in.uv.xy).a);

			sides += texture(textures[tid], fs_in.uv.xy + vec2(stepSize, 0.0f)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-stepSize, 0.0f)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(0.0f, stepSize)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(0.0f, -stepSize)).a;
		
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-stepSize, -stepSize)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2( stepSize, -stepSize)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-stepSize,  stepSize)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2( stepSize,  stepSize)).a;

			alpha *= sides;	

			/*
			alpha = (1 - texture(textures[tid], fs_in.uv.xy).a);

			sides += texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, 0.0f)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, 0.0f)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(0.0f, fs_in.uv.w)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(0.0f, -fs_in.uv.w)).a;
		
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, -fs_in.uv.w)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, -fs_in.uv.w)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(-fs_in.uv.z, fs_in.uv.w)).a;
			sides += texture(textures[tid], fs_in.uv.xy + vec2(fs_in.uv.z, fs_in.uv.w)).a;

			alpha *= sides;
			*/
		}

		texColor = texture(textures[tid], fs_in.uv.xy) * fs_in.color;
	}

	float light_intensity = 1.0f;
	vec3 light = vec3(0.3f, 0.3f, 0.7f) * light_intensity;

	color = alpha > 0 ? vec4(0, 0, 0, alpha) : texColor;// * vec4(light, 1.0f);
}
