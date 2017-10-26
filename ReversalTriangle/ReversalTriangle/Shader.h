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
	void setFloat_moveSpeed(const std::string &name, float value) const;

private:
	GLuint programId;

};

