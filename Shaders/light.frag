#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D lightMap;
uniform sampler2D shadowMap;

uniform float lightIntensity;
uniform float ambientIntensity;

const float offset = 1.0 / 300;

void main()
{ 
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );

	float side = 2.0;
	float corner = 1.0;
	float center = 4.0;
	float div = 16;

	float kernel[9] = float[](
		corner / div, side / div, corner / div,
		side / div, center / div, side / div,
		corner / div, side / div, corner/ div  
	);

	vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * ambientIntensity;

	vec4 diffuseColor = texture(screenTexture, TexCoords);
	vec4 shadow = texture(shadowMap, TexCoords);
	vec4 light = texture(lightMap, TexCoords) * lightIntensity * shadow;

	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(lightMap, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    //color = vec4(col, 1.0);
	vec3 intensity = ambient + vec3(light);
	vec3 finalColor = diffuseColor.rgb * intensity;

    color = vec4(finalColor, diffuseColor.a);
	//color = light;
}