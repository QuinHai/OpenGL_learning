#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

const char* vertexPath = "resources\\vshader.glsl";
const char* fragmentPath = "resources\\fshader.glsl";
const char* lightFragmentPath = "resources\\flampshader.glsl";
const char* texturePath = "resources\\container2.png";
const char* textureSpecularPath = "resources\\container2_specular.png";

const int screenWidth = 800;
const int screenHeight = 600;
float deltaTime = 0;
float lastFrame = 0;
float lastX = 400;
float lastY = 300;
bool isFirst = true;
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (isFirst) {
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeybroad(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeybroad(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeybroad(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeybroad(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeybroad(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeybroad(DOWN, deltaTime);
}

GLuint loadTexture(char const * path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannel;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannel, 0);
	if (data)
	{
		GLenum format;
		if (nrChannel == 1) format = GL_RED;
		else if (nrChannel == 3) format = GL_RGB;
		else if (nrChannel == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OPNEGL_LIGHTTEST", NULL, NULL);
	if (window == NULL) {
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

	Shader myShader(vertexPath, fragmentPath);
	Shader lampShader(vertexPath, lightFragmentPath);

	float vertices[] = {
		// positions          // normals 法线         // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//顶点数据处理方案
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/************************************************************************/
	/* 灯源                                                                     */
	/************************************************************************/
	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_ARRAY_BUFFER已经有数据无需继续传入
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	GLuint diffuseMap = loadTexture(texturePath);
	GLuint specularMap = loadTexture(textureSpecularPath);

	myShader.use();
	myShader.setInt("material.diffuse", 0);//设置纹理单元
	myShader.setInt("material.specular", 1);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();//LookAt Matrix
		glm::mat4 model;

		/************************************************************************/
		/*光源显示                                                                      */
		/************************************************************************/
		lampShader.use();

		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		glBindVertexArray(lampVAO);
		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);		
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/************************************************************************/
		/* 物体显示                                                                     */
		/************************************************************************/
		myShader.use();

		// directional light
		myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		myShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		myShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		myShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		myShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		myShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		myShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		myShader.setFloat("pointLights[0].constant", 1.0f);
		myShader.setFloat("pointLights[0].linear", 0.09);
		myShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		myShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		myShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		myShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		myShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		myShader.setFloat("pointLights[1].constant", 1.0f);
		myShader.setFloat("pointLights[1].linear", 0.09);
		myShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		myShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		myShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		myShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		myShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		myShader.setFloat("pointLights[2].constant", 1.0f);
		myShader.setFloat("pointLights[2].linear", 0.09);
		myShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		myShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		myShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		myShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		myShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		myShader.setFloat("pointLights[3].constant", 1.0f);
		myShader.setFloat("pointLights[3].linear", 0.09);
		myShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		myShader.setVec3("spotLight.position", camera.Position);
		myShader.setVec3("spotLight.direction", camera.Front);
		myShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		myShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		myShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		myShader.setFloat("spotLight.constant", 1.0f);
		myShader.setFloat("spotLight.linear", 0.09);
		myShader.setFloat("spotLight.quadratic", 0.032);
		myShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		myShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		myShader.setFloat("material.shininess", 32.0f);
		//myShader.setVec3("light.position", lightPos);//用于漫反射计算
		myShader.setVec3("viewPos", camera.Position);//用于镜面反射计算

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	system("pause");
	return 0;
}

