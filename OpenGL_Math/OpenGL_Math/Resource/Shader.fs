#version 450 core

out vec4 FragColor;

in vec3 myColor;
in vec2 myTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix_color = 0.2f;

void main()
{
	FragColor = mix(texture(texture1, myTexCoord) * vec4(myColor, 1.0f), texture(texture2, myTexCoord), mix_color);
}