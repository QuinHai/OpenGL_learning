#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* vertexPath = "Resources\\Shader.vs";
const char* fragmentPath = "Resources\\Shader.fs";
const int screenWidth = 800;
const int screenHeight = 600;

bool firstMouse = true;//是否第一次调用mouse_callback
float lastX = 400, lastY = 300;//设置全局变量，用于储存上一帧鼠标位置
float yaw = 0; //偏航角
float pitch = 0;//俯仰角
float fov = 45.0f;//视野//用于prejection
float deltaTime = 0;//与上一帧时间差
float lastFrame = 0;//上一帧时间
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//摄像机方向向量//cameraDirection*-1
													 //即为相机指向方向，为单位向量，这里指向z轴负方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机向上向量
//cameraRight = cross(up, direction)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = deltaTime * 2.5f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}//右向量标准化为单位向量，使之只表示方向
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		cameraPos += cameraUp * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK))
	{
		cameraPos -= cameraUp * cameraSpeed;
	}

}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)//用第一次鼠标位置初始化上一帧鼠标位置
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	//xpos,ypos->鼠标当前位置
	//lastX,lastY->鼠标上一帧位置
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; //???
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;//灵敏度
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	
	yaw += xoffset;//偏航角
	pitch += yoffset;//仰俯角

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0)
		pitch = -89.0f;
	//对仰俯角限制

	glm::vec3 front;//摄影机新的方向向量
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));//??公式??
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{//鼠标滚轮回调函数
	//yoffset表示竖直滚动大小
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//设置光标不可见
	glfwSetCursorPosCallback(window, mouse_callback);
	//注册鼠标位置回调函数，类似于窗口回调函数
	//回调函数是为了将相关参数传递给用户自定义的函数
	glfwSetScrollCallback(window, scroll_callback);
	//注册鼠标滚轮回调函数

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	Shader myShader(vertexPath, fragmentPath);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	myShader.use();

	int width, height, nrChannel;
	unsigned char* imgData;
	GLuint texture1, texture2;
	imgData = stbi_load("Resources\\Cube.png", &width, &height, &nrChannel, 0);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgData);
	stbi_set_flip_vertically_on_load(true);
	imgData = stbi_load("Resources\\timg.jpg", &width, &height, &nrChannel, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(myShader.shaderProgram, "myTexture1"), 0);
	glUniform1i(glGetUniformLocation(myShader.shaderProgram, "myTexture2"), 1);

	stbi_image_free(imgData);

	glEnable(GL_DEPTH_TEST);


	glm::vec3 cubePosition[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	
	//glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);

		glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.use();
		glm::mat4 model;//模型矩阵 -》用于将局部坐标变化到世界坐标
		glm::mat4 view;//观察矩阵 -》用于摄像机
		glm::mat4 projection;//透视

		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//fov = Field of view
		glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(VAO);
		
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePosition[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glUniformMatrix4fv(uniform地址， 矩阵数量， 是否转置矩阵，矩阵数据)

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	//system("pause");
	return 0;
}