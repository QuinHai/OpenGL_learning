#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLchar* VertexShaderSource =
"#version 450 core\n"
"layout(location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
"}\n";

const GLchar* FragmentShaperSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

void framebuffer_size_calback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}




int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "More_VAO_VBO", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_calback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Init GLAD failed" << std::endl;
		return -1;
	}


	int success;
	char infoLog[512];
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FIALED\n" << infoLog << std::endl;

	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaperSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float triangle_vertices[] = {
		//x,y,z
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};
	float triangle_vertices2[] = {
		//x,y,z
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};


	GLuint VAOid1, VAOid2, VBOid1, VBOid2;
	glGenBuffers(1, &VBOid1);
	glGenBuffers(1, &VBOid2);

	glGenVertexArrays(1, &VAOid1);
	glGenVertexArrays(1, &VAOid2);

	glBindVertexArray(VAOid1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//ERROR::glVertexAttribLPointer

	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOid2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices2), triangle_vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE ,0 , 0);
	//ERROR::glVertexAttribLPointer

	glEnableVertexAttribArray(0);
	/*
	每用glBufferData输入一次数据
	就要用glVertexAttribPointer处理一次
	同时，每次使用glVertexAttribPointer就要用glEnableVertexAttribArray激活一次
	
	*/


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.5f, 0.3f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAOid1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAOid2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;

}