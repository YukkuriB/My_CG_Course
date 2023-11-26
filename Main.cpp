#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>


#include"Model.h"





int main()
{
	glfwInit();
	// ����opengl�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 
	// ���ô��ڳߴ�

	const unsigned int width = 2160;
	const unsigned int height = 2160;


	// �������ڣ�������ڳ��������failed����

	GLFWwindow* window = glfwCreateWindow(width, height, "HelloWorld", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// ����glsl��Ҫʹ�ô���
	glfwMakeContextCurrent(window);

	gladLoadGL();

	// ������ͼ�����ض���ɫˢ�»�����
	glViewport(0, 0, width, height);

	
	//����shader
	Shader shaderProgram("default.vert", "default.frag");
	Shader outliningProgram("outlining.vert", "outlining.frag");
	//���ɹ�Դ
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	
	//�����Դshader	
		shaderProgram.Activate();
		
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// ��������������������������������������������������������������ͼ���֡�������������������������������������������������������������������
  

	glEnable(GL_DEPTH_TEST);
	//���ʹ�õĴ���
	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//�����޳�ʵ��
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	//ģ������λ��
	Model model1("models/crow/scene.gltf");
	//fps������
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	//��ֱͬ������
	//glfwSwapInterval(0);


	
	//����������
	while (!glfwWindowShouldClose(window))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "TestFps - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
			//���ò�����֡ͬ��
			//camera.Inputs(window);
		}

		//������ɫ����
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//�����ò�����֡ͬ��
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//ģ�ͻ��Ʋ���
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		model1.Draw(shaderProgram, camera);

		//�����ǹ�������ߵ�ʵ�����

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//outliningProgram.Activate();
		//glUniform1f(glGetUniformLocation(outliningProgram.ID, "outlining"), 0.5f);
		//model1.Draw(outliningProgram, camera);
		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error: " << err << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	shaderProgram.Delete();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}