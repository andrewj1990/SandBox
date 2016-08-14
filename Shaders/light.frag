#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D lightMap;

void main()
{ 
	vec3 ambient = vec3(0.6f, 0.6f, 0.7f);

	vec4 diffuseColor = texture(screenTexture, TexCoords);
	vec4 light = texture(lightMap, TexCoords);

	vec3 intensity = ambient + light.rgb;
	vec3 finalColor = diffuseColor.rgb * intensity;

    color = vec4(finalColor, diffuseColor.a);
}