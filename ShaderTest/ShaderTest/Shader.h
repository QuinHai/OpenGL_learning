#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader
{
public:
	Shader();
	~Shader();

	//����ID
	GLuint ID;

	//��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//ʹ��/�������
	void use();
	/*
	void func ()const;
	��1�������޸ĳ�Ա������ 
	��2�����ܵ��÷�const������ 
	��3������const�������Ե���������������const������ȻҲ���Ե������� 
	��4��ֻ�ܴ�const���󣨻�������ָ�룩�ϵ��ã����磺const   A   a;   a.f();
	*/
	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setColor_vec3(const std::string &name, float v1, float v2, float v3) const;
};


