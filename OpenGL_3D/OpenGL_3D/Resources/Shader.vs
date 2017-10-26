#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
void main()
{
//�˷�Ҫ���������ȡ
	gl_Position =  projection * view * model * vec4(aPos, 1.0f);
	//gl_Position =  vec4(aPos, 1.0f);
	TexCoord = aTexture;
}