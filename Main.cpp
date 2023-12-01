#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
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
	bool useFlatShading = false;//flat��ɫ������
	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	glm::vec3 translation2(0.0f, 0.0f, 0.0f);

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
		-0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, // ���½� (����ɫ)
		 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.0f, // ���½� (����ɫ)
		 0.5f,  0.5f, 0.0f, 1.0f, 0.9f, 0.5f, // ���Ͻ� (�ǳ����Ļ�ɫ)

		// �ڶ���������
		-0.5f,  0.5f, 0.0f, 0.2f, 0.2f, 0.0f, // ���Ͻ� (�ϰ���ɫ)

	};

	GLuint indicesSquare[] = {
		0, 1, 2, // ��һ��������
		2, 3, 0  // �ڶ���������
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
	Shader shaderFlat("flat.vert", "flat.frag");


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
		translation.x = sin(timeValue) * translationSpeed;
		translation2.y = tan(timeValue) * translationSpeed;


		float angle = timeValue * glm::radians(rotationSpeed);  // ��ת�ٶ�Ϊ50��/��
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotationMatrixReverse = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 modelMatrix = translationMatrix * rotationMatrix;

		glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), translation2);
		glm::mat4 modelMatrix2 = translationMatrix2 * rotationMatrixReverse;


		//ˢ�±���
		glClearColor(0.07f, 0.07f, 0.07f, 0.07f);
		glClear(GL_COLOR_BUFFER_BIT);
		//������ɫ������

	

		shaderProgram.Activate();
		
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "triangleColor"), 1, triangleColor);

		//��һ��������ת������
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));


		//����������
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		VAO1.Unbind();
		//�ڶ�������
		if (useFlatShading) {
			shaderFlat.Activate();
		}
		else {

		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
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
		//flat����
		ImGui::Checkbox("Use Flat Shading", &useFlatShading);

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
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, -50.0f, 50.0f); // ��Χ��0��100
		ImGui::Text("Homework C2");
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
	shaderFlat.Delete();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}