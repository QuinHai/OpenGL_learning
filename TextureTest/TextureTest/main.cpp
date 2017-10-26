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
	//���û��ı䴰�ڴ�Сʱ�������ӿ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	Shader shader(vertexPath, fragmentPath);

	float vertices[] = {
		//λ��				//��ɫ				//��������
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 2.0f,		//����
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	2.0f, 0.0f,		//����
		-0.5, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//����
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 2.0f		//����
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

	

	
	//ʹ��stbi_load�����ļ�
	int width, height, nrChannels;
	int width2, height2;
	unsigned char *imgData = stbi_load(MY_TEXTURE, &width, &height, &nrChannels, 0);
	unsigned char *imgData2 = stbi_load(MY_TEXTURE2, &width2, &height2, &nrChannels, 0);
	//(�ļ����� ��ȣ� �߶ȣ� ��ɫͨ���ĸ����� ������)

	//���������������VBO�� id���÷�ʽ
	GLuint texture[2];
	glGenTextures(2, texture);

	//��һ������----------------------------------------------------------
	//���������,GL_TEXTURE_2D����GL_ARRAY_BUFFER
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//������������
	//Ϊ�����s��t��������ͼģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Ϊ����ķŴ����С�˲�������صĿ���ѡ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//����һ������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	//�ڶ�������----------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	//������������
	//Ϊ�����s��t��������ͼģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//Ϊ����ķŴ����С�˲�������صĿ���ѡ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//����һ������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData2);
	/*
		*����Ŀ��
		*Ϊ����ָ���༶��Զ����ļ���0Ϊ��������
		*����OpenGL����ϣ����������Ϊ���ָ�ʽ
		*������
		*����߶�
		*������Ӧ������0����ʷ�������⣩
		*ͼԴ��ʽ
		*��������
		*ͼ������
	*/
	//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����Mipmap����
	glGenerateMipmap(GL_TEXTURE_2D);
	//--------------------------------------------------------------------
	//�ͷ�ͼ���ڴ�
	stbi_image_free(imgData);
	stbi_image_free(imgData2);

	shader.use();
	//������Ԫ����GL_TEXTURE0,GL_TEXTURE1�ȣ��󶨵�uniform����Ĳ�����
	//��ourTexture1������Ԫ0
	glUniform1i(glGetUniformLocation(shader.shaderProgram, "ourTexture1"), 0);
	//��ourTexture2������Ԫ1
	shader.setInt("ourTexture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW
		glActiveTexture(GL_TEXTURE0);// �ڰ�����֮ǰ�ȼ�������Ԫ0
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);// �ڰ�����֮ǰ�ȼ�������Ԫ1
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