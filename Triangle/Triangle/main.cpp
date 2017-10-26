#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


const GLchar* vertexShaderSource =
"#version 450 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
"}\0";
/*
顶点着色器
*/

const GLchar* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	//当用户改变窗口大小时，调整视口
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}
	/*
	GLAD用于管理OpenGL的函数指针，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	*/



	//创建顶点着色器对象
	unsigned int vertexShader;//或者 GLuint vertexShader;	VOB同理 (单词拆解GL->opengl uint->无符号整形)
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//将着色器源码附加到着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译
	glCompileShader(vertexShader);
	//检测编译是否错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILD\n" <<
			infoLog << std::endl;
	}

	//创建片段着色器对象
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//检测编译错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILD\n" <<
			infoLog << std::endl;
	}



	/*
	*着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	*如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，
	*然后在渲染对象的时候激活这个着色器程序
	*/
	//创建着色器对象
	GLint shaderProgram;
	shaderProgram = glCreateProgram();

	//将编译的着色器附到对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//用glLinkProgram连接这些着色器
	glLinkProgram(shaderProgram);
	//判断链接着色器程序是否正确
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGAME::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//删除已用着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	*以上程序是告诉GPU如何处理数据
	*接下来将解析并输入数据
	*/

	//顶点
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	//创建VAO与VBO对象
	GLuint VAO, VBO;
	//形成VAO顶点数组对象
	glGenVertexArrays(1, &VAO);
	//形成VBO顶点缓冲对象
	glGenBuffers(1, &VBO);
	//绑定VAO
	glBindVertexArray(VAO);
	//绑定VBO,并将VBO输入GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//告诉GPU如何解析顶点数据
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*
	1.要配置的顶点属性(顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location))
	(此处我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。)
	2.指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	3.vec*由浮点数值组成
	4.是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
	5.步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）
	6.它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
	*/

	//以顶点属性位置值为参数，启动顶点属性
	//顶点属性即为顶点着色器中使用layout(location = 0)定义
	glEnableVertexAttribArray(0);

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//(图元类型，顶点数组的起始索引，绘制顶点数 )


									 //检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	system("pause");
	return 0;
}

