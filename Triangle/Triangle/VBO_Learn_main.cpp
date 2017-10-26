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
������ɫ��
*/

const GLchar* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
/*
Ƭ����ɫ��
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
	//��ʼ��GLFW
	glfwInit();
	//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//OpenGL���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//����
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};



	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}
	/*
	GLAD���ڹ���OpenGL�ĺ���ָ�룬�����ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
	*/

	glViewport(0, 0, 800, 600);//���ڴ�С���� ����������x����������y����ȣ��߶ȣ�
	
	//���û��ı䴰�ڴ�Сʱ�������ӿ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	unsigned int VBO;
	
	//���㻺�����
	glGenBuffers(1, &VBO);//(����id, ����)
	/*
	����һ���Խ�һ���������͵�GPU
	*/

	//���´����Ļ���󶨵����㻺�����Ļ�������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
	����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�
	������ö����������õ�ǰ�󶨵Ļ���(VBO)
	*/

	//���������ݸ��Ƶ������ڴ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//(Ŀ�껺�����ͣ� �������ݴ�С[�ֽ�]�� ���͵����ݣ� �Կ�������ʽ)
	/*
	�Կ�������ʽ��
	GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣

	#�Կ��Ὣ�����ַ����ܹ�����д����ڴ�
	*/

	//����������ɫ������
	unsigned int vertexShader;//���� GLuint vertexShader;	VOBͬ�� (���ʲ��GL->opengl uint->�޷�������)
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	//����ɫ��Դ�븽�ӵ���ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//����
	glCompileShader(vertexShader);

	//�������Ƿ����
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILD\n" <<
			infoLog << std::endl;
	}

	//����Ƭ����ɫ������
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/*
	 *��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
	 *���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������
	 *Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
	 */
	//������ɫ������
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	//���������ɫ������������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//��glLinkProgram������Щ��ɫ��
	glLinkProgram(shaderProgram);

	//����������
	glUseProgram(shaderProgram);
	//ɾ��������ɫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	/*
	 *���ϳ����Ǹ���GPU��δ�������
	 *����������������������
	 */
	//����GPU��ν�����������
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*
	1.Ҫ���õĶ�������(������ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location))
		(�˴�����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��)
	2.ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	3.vec*�ɸ�����ֵ���
	4.�Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮��
	5.����(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
		����Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã�
	6.����ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0
	*/

	//�Զ�������λ��ֵΪ������������������
	//�������Լ�Ϊ������ɫ����ʹ��layout(location = 0)����
	glEnableVertexAttribArray(0);

	//glBindVertexArray(VBO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//(ͼԪ���ͣ������������ʼ���������ƶ����� )


	//ѭ����������Ⱦ��
	while (!glfwWindowShouldClose(window)) {
		//����
		processInput(window);
		
		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT);//״̬ʹ��
		/*
		OpenGL����һ�����͵�״̬��
		*/
							 
		//��鲢�����¼�����������
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	
	system("pause");
	return 0;
}

