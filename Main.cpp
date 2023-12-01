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
		int colorIndex = i * 6 + 3; // ������ɫ�������е���ʼ����
		std::copy(color, color + 3, &vertices[colorIndex]);
	}
}



int main()
{
	glfwInit();
	// ����opengl�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 

	float triangleColor[3] = { 1.0f, 1.0f, 1.0f }; // ��ɫ
	float rotationSpeed = -50.0f;
	float rotationSpeed2 = -50.0f;
	float translationSpeed = 0.5f;
	glm::vec3 translation(0.0f, 0.0f, 0.0f);

	float BgColor[4] = { 0.07f, 0.07f, 0.07f, 0.07f }; // ��ɫ
	// ���嶥��
	GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // ��һ�������λ�ú���ɫ
		0.7f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // �ڶ��������λ�ú���ɫ
		0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f // �����������λ�ú���ɫ
	};
	
	GLuint indices[] =
	{
		0, 1, 2, 

	};

	GLfloat verticesSquare[] = {
		// ��һ��������
		-0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.0f, // ���½� (����ɫ)
		 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.0f, // ���½� (����ɫ)
		 0.5f,  0.5f, 0.0f, 1.0f, 0.9f, 0.5f, // ���Ͻ� (�ǳ����Ļ�ɫ)

		// �ڶ���������
		 0.5f,  0.5f, 0.0f, 1.0f, 0.9f, 0.0f, // ���Ͻ� (�ǳ����Ļ�ɫ)
		-0.5f,  0.5f, 0.0f, 0.2f, 0.2f, 0.0f, // ���Ͻ� (�ϰ���ɫ)
		-0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.5f  // ���½� (����ɫ)
	};

	GLuint indicesSquare[] = {
		0, 1, 2, // ��һ��������
		3, 4, 5  // �ڶ���������
	};

	// �������ڣ�������ڳ��������failed����

	GLFWwindow* window = glfwCreateWindow(1080, 1080, "HelloWorld", NULL, NULL);
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
	glViewport(0, 0, 1080, 1080);
	
	//����ͶӰ����
	glm::mat4 projection;
	projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);


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
	//��������

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(verticesSquare, sizeof(verticesSquare));
	EBO EBO2(indicesSquare, sizeof(indicesSquare));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	glfwSwapBuffers(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	//����Ҫ�رմ����ٹرմ���
	while (!glfwWindowShouldClose(window))
	{
		float timeValue = glfwGetTime();
		//����ƽ������
		translation.x += translationSpeed * timeValue;

		float angle = timeValue * glm::radians(rotationSpeed);  // ��ת�ٶ�Ϊ50��/��
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 modelMatrix = translationMatrix * rotation;

		//ͶӰ����
	

		//ˢ�±���
		glClearColor(0.07f, 0.07f, 0.07f, 0.07f);
		glClear(GL_COLOR_BUFFER_BIT);
		//������ɫ������

	

		shaderProgram.Activate();

		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "triangleColor"), 1, triangleColor);

		//��ת������
		GLuint rotationLoc = glGetUniformLocation(shaderProgram.ID, "rotationMatrix");
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotation));


		//����������
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		VAO1.Unbind();
		//�ڶ�������
		glm::mat4 reverseMatrix = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(reverseMatrix));
		//���Ʒ���
		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
		VAO2.Unbind();


		//imgui���Ʋ���
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//���ڴ���
		static GLfloat currentColor[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::Begin("Triangle Color Picker");
		//�Ҽ��˵�
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("Color Context Menu");
		}
		if (ImGui::BeginPopup("Color Context Menu")) {
			//1�����˵���
			if (ImGui::MenuItem("Red")) {
				GLfloat red[3] = { 1.0f, 0.0f, 0.0f };
				//updateVertexColors(vertices, red, 3);
			}
			if (ImGui::MenuItem("Green")) {
				GLfloat green[3] = { 0.0f, 1.0f, 0.0f };
				//updateVertexColors(vertices, green, 3);
			}
			if (ImGui::MenuItem("Blue")) {
				GLfloat blue[3] = { 0.0f, 0.0f, 1.0f };
				//updateVertexColors(vertices, blue, 3);
			}
			//ˢ��vbo
			VBO1.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //ע��ʹ��subdata
			VBO1.Unbind();
			// ������ɫ...

			ImGui::EndPopup();
		}
		//2������ת�ٶȲ���
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, -10000.0f, 10000.0f); // ��Χ��0��100
		//3����ɫ�ʲ���
		if (ImGui::ColorEdit3("Triangle Color", currentColor)) {
			//�����ɫ�ı�͸�����ɫ
			//updateVertexColors(vertices, currentColor, 3); // ������ɫ

			//ˢ��vbo
			VBO1.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //ע��ʹ��subdata
			VBO1.Unbind();
		}
		ImGui::Text("Homework C1");
		ImGui::End();
		// imgui������Ⱦ
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