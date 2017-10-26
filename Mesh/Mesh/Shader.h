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

public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();

	//uniform
	//...
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}
	
	GLuint shaderProgram;
};

