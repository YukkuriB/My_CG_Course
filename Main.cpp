#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include<glm/glm.hpp>
#include"Texture.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "shaderClass.h"





int main()
{
	glfwInit();
	// 申明opengl版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 
	// 定义定点位置

	GLfloat vertices[] =
	{ //坐标                 /   色彩                 //
		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,           0.0f, 0.0f,//下左
		-0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,           0.0f, 1.0f, //上左
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,           1.0f, 1.0f, //上右
		 0.5f,  -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,           1.0f, 0.0f, //下右

	};
	
	//读取顺序

	GLuint indices[] =
	{
		0, 2, 1, //上半三角形
		0, 3, 2, //下半三角形
	};

	// 创建窗口，如果窗口出问题就以failed结束

	GLFWwindow* window = glfwCreateWindow(800, 800, "HelloWorld", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 告诉glsl需要使用窗口
	glfwMakeContextCurrent(window);

	gladLoadGL();

	// 创建视图并以特定颜色刷新缓冲区
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");


	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3* sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6* sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// ——————————————————————————————贴图部分——————————————————————————————————
	Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);


	//等需要关闭窗口再关闭窗口
	while (!glfwWindowShouldClose(window))
	{
		float timeValue = glfwGetTime();
		float angle = timeValue * glm::radians(50.0f);  // 旋转速度为50度/秒
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//激活着色器程序
		shaderProgram.Activate();

		glUniform1f(uniID, 0.5f);
		popCat.Bind();
		//下面是关于旋转的自加程序
		GLuint rotationLoc = glGetUniformLocation(shaderProgram.ID, "rotationMatrix");
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotation));

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}