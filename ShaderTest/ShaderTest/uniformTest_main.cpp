#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


const GLchar* vertexShaderSource =
"#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0f);\n"
"}\0";
/*
顶点着色器
*/

const GLchar* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = ourColor;\n"
"}\0";
/*
片段着色器
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//OpenGL副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILD\n" <<
			infoLog << std::endl;
	}



	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILD\n" <<
			infoLog << std::endl;
	}


	GLint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGAME::LINKING_FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		//位置             
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		0.0f,  0.5f, 0.0f,  
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		//uniform
		float timeValue = glfwGetTime();//获取运行时间
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//设置颜色变化
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//查询并获取uniform变量地址 （如果glGetUniformLocation返回-1则未找到）
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//设置uniform的值
																	   /*
																	   由于OpenGL在其核心是一个C库，没有函数重载，所以当函数参数不同时定义新的函数
																	   glUniform是一个典型例子：glUniform+特定后缀
																	   后缀：
																	   f	函数需要一个float作为它的值
																	   i	函数需要一个int作为它的值
																	   ui	函数需要一个unsigned int作为它的值
																	   3f	函数需要3个float作为它的值(4f同理)
																	   fv	函数需要一个float向量/数组作为它的值
																	   */

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	system("pause");
	return 0;
}

