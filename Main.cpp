#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "shaderClass.h"



void updateVertexColors(GLfloat vertices[], const GLfloat color[3], int numVertices) {
	for (int i = 0; i < numVertices; ++i) {
		int colorIndex = i * 6 + 3; // 计算颜色在数组中的起始索引
		std::copy(color, color + 3, &vertices[colorIndex]);
	}
}



int main()
{
	glfwInit();
	// 申明opengl版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 
	// 定义顶点
	float triangleColor[3] = { 1.0f, 1.0f, 1.0f }; // 白色
	float rotationSpeed = -50.0f;
	float BgColor[4] = { 0.07f, 0.07f, 0.07f, 0.07f }; // 白色
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 1.0f, 1.0f, 1.0f, // 第一个顶点的位置和颜色
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 1.0f, 1.0f, 1.0f,   // 第二个顶点的位置和颜色
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 1.0f, 1.0f // 第三个顶点的位置和颜色
	};
	
	GLuint indices[] =
	{
		0, 1, 2, 

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

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	glfwSwapBuffers(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	//等需要关闭窗口再关闭窗口
	while (!glfwWindowShouldClose(window))
	{
		float timeValue = glfwGetTime();
		float angle = timeValue * glm::radians(rotationSpeed);  // 旋转速度为50度/秒
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

		glClearColor(0.07f, 0.07f, 0.07f, 0.07f);
		glClear(GL_COLOR_BUFFER_BIT);
		//激活着色器程序

	

		shaderProgram.Activate();
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "triangleColor"), 1, triangleColor);

		//下面是关于旋转的自加程序
		GLuint rotationLoc = glGetUniformLocation(shaderProgram.ID, "rotationMatrix");
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotation));

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		//imgui控制部分
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//窗口创建
		static GLfloat currentColor[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::Begin("Triangle Color Picker");
		//右键菜单
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("Color Context Menu");
		}

		if (ImGui::BeginPopup("Color Context Menu")) {
			//1弹出菜单项
			if (ImGui::MenuItem("Red")) {
				GLfloat red[3] = { 1.0f, 0.0f, 0.0f };
				updateVertexColors(vertices, red, 3);
			}
			if (ImGui::MenuItem("Green")) {
				GLfloat green[3] = { 0.0f, 1.0f, 0.0f };
				updateVertexColors(vertices, green, 3);
			}
			if (ImGui::MenuItem("Blue")) {
				GLfloat blue[3] = { 0.0f, 0.0f, 1.0f };
				updateVertexColors(vertices, blue, 3);
			}
			//刷新vbo
			VBO1.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //注意使用subdata
			VBO1.Unbind();
			// 其他颜色...

			ImGui::EndPopup();
		}

		//2更改旋转速度部分
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, -10000.0f, 10000.0f); // 范围从0到100
		

		//3更改色彩部分
		if (ImGui::ColorEdit3("Triangle Color", currentColor)) {
			//如果颜色改变就更新颜色
			updateVertexColors(vertices, currentColor, 3); // 更新颜色

			//刷新vbo
			VBO1.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //注意使用subdata
			VBO1.Unbind();
		}
		ImGui::Text("Homework C1");

		
		ImGui::End();


		// imgui界面渲染
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}