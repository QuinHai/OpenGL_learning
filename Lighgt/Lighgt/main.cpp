#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* vertexShaderPath = "Resources\\Shader.vs";
const char* fragmentShaderPath = "Resources\\Shader.fs";
const char* lightFragmentShaderPath = "Resources\\lightShader.fs";
const char* lightVertexShaderPath = "Resources\\lightShader.vs";
const int screenWidth = 800;
const int screenHeight = 600;

Camera camera;
float deltaTime = 0;
float lastFrame = 0;
float lastX = 400;
float lastY = 300;
bool isFirst = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
}
void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
	if (isFirst)
	{
		lastX = xPos;
		lastY = yPos;
		isFirst = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	Shader myShader(vertexShaderPath, fragmentShaderPath);
	Shader lightShader(lightVertexShaderPath, lightFragmentShaderPath);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
	};

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	GLuint lightVAO;//µÆ
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		myShader.use();
		GLuint lightColorLoc = glGetUniformLocation(myShader.shaderProgram, "lightColor");
		GLuint objectColorLoc = glGetUniformLocation(myShader.shaderProgram, "objectColor");
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 1.0f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;
		
		myShader.use();
		glBindVertexArray(VAO);

		GLuint projectionLoc = glGetUniformLocation(myShader.shaderProgram, "projection");
		GLuint viewLoc = glGetUniformLocation(myShader.shaderProgram, "view");
		GLuint modelLoc = glGetUniformLocation(myShader.shaderProgram, "model");
		
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		glBindVertexArray(lightVAO);

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		
		projectionLoc = glGetUniformLocation(lightShader.shaderProgram, "projection");
		viewLoc = glGetUniformLocation(lightShader.shaderProgram, "view");
		modelLoc = glGetUniformLocation(lightShader.shaderProgram, "model");
		
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	system("pause");
	return 0;
}