#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
	//初始化GLFW
	glfwInit();
	//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//OpenGL副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	


	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLAD
	/*
	GLAD用于管理OpenGL的函数指针，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);//窗口大小设置 （左下坐标x，左下坐标y，宽度，高度）
	
	//当用户改变窗口大小时，调整视口
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//循环动作（渲染）
	while (!glfwWindowShouldClose(window)) {
		//输入
		processInput(window);
		
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);//状态使用
		/*
		OpenGL就是一个大型的状态机
		*/
							 
		//检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	
	system("pause");
	return 0;
}

