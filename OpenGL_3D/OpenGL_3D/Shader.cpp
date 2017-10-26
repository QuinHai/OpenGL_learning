#include "Shader.h"



Shader::Shader()
{
}


Shader::~Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string uncvShaderCode;//储存GLSL代码
	std::string uncfShaderCode;
	std::ifstream vShaderFile;//用于打开读取文件
	std::ifstream fShaderFile;
	std::stringstream vShaderString;//用于将文件读入流中
	std::stringstream fShaderString;

	//保证ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		vShaderString << vShaderFile.rdbuf();
		fShaderString << fShaderFile.rdbuf();

		uncvShaderCode = vShaderString.str();
		uncfShaderCode = fShaderString.str();

		vShaderFile.close();
		fShaderFile.close();
	}
	catch (...)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char *vShaderCode = uncvShaderCode.c_str();
	const char *fShaderCode = uncfShaderCode.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	int success;
	char infoLog[512];

	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glShaderSource(fragment, 1, &fShaderCode, NULL);

	//vertexShader
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << 
			infoLog << std::endl;
	}

	//fragmentShader
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//ShaderProgram
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
void Shader::use()
{
	glUseProgram(shaderProgram);
}