#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 TexCoord;

out vec3 myColor;
out vec2 myTexCoord;

uniform mat4 trans;

void main()
{
gl_Position =  trans * vec4(aPos, 1.0f);
myColor = aColor;
myTexCoord = TexCoord;
}