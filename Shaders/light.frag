#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D lightMap;

uniform float lightIntensity;
uniform float ambientIntensity;

void main()
{ 
	vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * ambientIntensity;

	vec4 diffuseColor = texture(screenTexture, TexCoords);
	vec4 light = texture(lightMap, TexCoords) * lightIntensity;
	
	vec3 intensity = ambient + light.rgb;
	vec3 finalColor = diffuseColor.rgb * intensity;

    color = vec4(finalColor, diffuseColor.a);
}