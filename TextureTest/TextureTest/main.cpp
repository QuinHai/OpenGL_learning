#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"

#define MY_TEXTURE "Resource//Cube.png"
#define MY_TEXTURE2 "Resource//awesomeface.png"
const GLchar* vertexPath = "Resource//Shader.vs";
const GLchar* fragmentPath = "Resource//Shader.fs";


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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Texture", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//当用户改变窗口大小时，调整视口
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	Shader shader(vertexPath, fragmentPath);

	float vertices[] = {
		//位置				//颜色				//纹理坐标
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 2.0f,		//右上
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	2.0f, 0.0f,		//右下
		-0.5, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//左下
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 2.0f		//左上
	};

	GLuint indexes[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO, VBO, EBO;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	

	
	//使用stbi_load载入文件
	int width, height, nrChannels;
	int width2, height2;
	unsigned char *imgData = stbi_load(MY_TEXTURE, &width, &height, &nrChannels, 0);
	unsigned char *imgData2 = stbi_load(MY_TEXTURE2, &width2, &height2, &nrChannels, 0);
	//(文件名， 宽度， 高度， 颜色通道的个数， ？？？)

	//生成纹理对象，类似VBO， id引用方式
	GLuint texture[2];
	glGenTextures(2, texture);

	//第一个纹理----------------------------------------------------------
	//绑定纹理对象,GL_TEXTURE_2D类似GL_ARRAY_BUFFER
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//设置纹理属性
	//为纹理的s和t轴设置贴图模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//为纹理的放大和缩小滤波设置相关的控制选项：
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//生成一个纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	//第二个纹理----------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	//设置纹理属性
	//为纹理的s和t轴设置贴图模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//为纹理的放大和缩小滤波设置相关的控制选项：
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//生成一个纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData2);
	/*
		*纹理目标
		*为纹理指定多级渐远纹理的级别（0为基本级别）
		*告诉OpenGL我们希望将纹理储存为何种格式
		*纹理宽度
		*纹理高度
		*？？？应该总是0（历史遗留问题）
		*图源格式
		*数据类型
		*图像数据
	*/
	//为当前绑定的纹理自动生成所有需要的多级渐远纹理（Mipmap）。
	glGenerateMipmap(GL_TEXTURE_2D);
	//--------------------------------------------------------------------
	//释放图像内存
	stbi_image_free(imgData);
	stbi_image_free(imgData2);

	shader.use();
	//将纹理单元（即GL_TEXTURE0,GL_TEXTURE1等）绑定到uniform定义的采样器
	//绑定ourTexture1和纹理单元0
	glUniform1i(glGetUniformLocation(shader.shaderProgram, "ourTexture1"), 0);
	//绑定ourTexture2和纹理单元1
	shader.setInt("ourTexture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW
		glActiveTexture(GL_TEXTURE0);// 在绑定纹理之前先激活纹理单元0
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);// 在绑定纹理之前先激活纹理单元1
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	system("pause");
	return 0;
}