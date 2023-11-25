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

	
	//��������
	Shader shaderProgram("default.vert", "default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	
		
		shaderProgram.Activate();
		
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// ��������������������������������������������������������������ͼ���֡�������������������������������������������������������������������
  

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model1("models/trees/scene.gltf");
	Model model2("models/ground/scene.gltf");
	Model model3("models/sword/scene.gltf");
	
	//����������
	while (!glfwWindowShouldClose(window))
	{


		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		model1.Draw(shaderProgram, camera);
		model2.Draw(shaderProgram, camera);
		model3.Draw(shaderProgram, camera);


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