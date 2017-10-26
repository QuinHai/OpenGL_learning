#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexPath = "Resource//Shader.vs";
const char* fragmentPath = "Resource//Shader.fs";

float mixValue = 0.2;

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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mixValue += 0.002f;
		if (mixValue > 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mixValue -= 0.002f;
		if (mixValue < 0.0f)
			mixValue = 0.0f;
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

	Shader myShader(vertexPath, fragmentPath);


	float vertices[] = {
		//位置				//颜色				//纹理坐标
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		//右上
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		//右下
		-0.5, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//左下
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f		//左上
	};
	
	GLuint indexes[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int width, height, nrChannel;
	float borderColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	
	GLuint texture[2];
	glGenTextures(2, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	unsigned char *imgData1 = stbi_load("Resource//Cube.png", &width, &height, &nrChannel, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData1);
	glGenerateMipmap(GL_TEXTURE_2D);



	glBindTexture(GL_TEXTURE_2D, texture[1]);
	unsigned char *imgData2 = stbi_load("Resource//awesomeface.png", &width, &height, &nrChannel, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData2);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgData1);
	stbi_image_free(imgData2);

	myShader.use();
	
	glUniform1i(glGetUniformLocation(myShader.shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(myShader.shaderProgram, "texture2"), 1);
	//(location , value)

	/*
	//变化矩阵
	glm::mat4 trans;
	trans = glm::translate(trans, glm::radians(90.0f) glm::vec3(0.0f, 0.0f, 1.0f));//radians 弧度//第三个参数：绕z轴旋转
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	//将矩阵变化信息传递给uniform
	glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "trans"), 1, GL_FALSE, glm::value_ptr(trans));
	//glUniformMatrix4fv 
		
		*参数一：uniform地址
		*参数二：发送矩阵数量
		*参数三：是否对矩阵进行置换（交换矩阵的行和列）
		*参数四：矩阵数据
		
	*/

	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUniform1f(glGetUniformLocation(myShader.shaderProgram, "mix_color"), mixValue);
		
		glm::mat4 trans;		
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));		
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "trans"),1 , GL_FALSE,glm::value_ptr(trans));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//(绘制模式， 打算绘制的顶点数， 索引的类型， EBO中的偏移量)

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	system("pause");
	return 0;

}