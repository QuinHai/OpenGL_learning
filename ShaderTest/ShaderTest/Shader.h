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

	//程序ID
	GLuint ID;

	//构造器读取并创建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//使用/激活程序
	void use();
	/*
	void func ()const;
	（1）不能修改成员变量； 
	（2）不能调用非const函数； 
	（3）其他const函数可以调用它；（其他非const函数当然也可以调用它） 
	（4）只能从const对象（或引用用指针）上调用，比如：const   A   a;   a.f();
	*/
	// uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setColor_vec3(const std::string &name, float v1, float v2, float v3) const;
};


