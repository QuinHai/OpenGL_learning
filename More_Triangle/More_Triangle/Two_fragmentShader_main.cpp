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

const GLchar* firstFragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

const GLchar* secondFragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.0f, 1.0f, 0.1f, 1.0f);\n"
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
	
	GLFWwindow *window = glfwCreateWindow(800, 600, "FragmentShader TEST", NULL, NULL);
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	GLuint firstFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(firstFragmentShader, 1, &firstFragmentShaderSource, NULL);
	glCompileShader(firstFragmentShader);

	glGetShaderiv(firstFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(firstFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	GLuint secondFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(secondFragmentShader, 1, &secondFragmentShaderSource, NULL);
	glCompileShader(secondFragmentShader);

	glGetShaderiv(secondFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(secondFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram[2];
	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();

	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[0], firstFragmentShader);
	glAttachShader(shaderProgram[1], secondFragmentShader);
	glLinkProgram(shaderProgram[0]);
	glLinkProgram(shaderProgram[1]);

	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS ,&success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM0::LINKING_FIALED\n" <<infoLog << std::endl;
	}

	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM1::LINKING_FIALED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(firstFragmentShader);
	glDeleteShader(secondFragmentShader);

	float first_triangle_vertices[] = {
		//x,y,z
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};
	float second_triangle_vertices[] = {
		//x,y,z
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	GLuint VAOs[2], VBOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle_vertices), first_triangle_vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle_vertices), second_triangle_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glfwTerminate();
	return 0;
}
