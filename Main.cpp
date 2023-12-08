#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <glm/glm.hpp> // ������glm����
#include <glm/gtc/matrix_transform.hpp> // ����任����
#include <glm/gtc/type_ptr.hpp> // ����ת������
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"OBJModel.h"
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
	bool EdgeMode = true;
	bool PointMode = false;
	bool FillMode = true;

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

	
	
	Shader shaderProgram("default.vert", "default.frag");
	Shader fixShader("fixColor.vert", "fixColor.frag");
	//��������
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	
		
	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	OBJModel objModel;
	objModel.LoadFromFile("models/eight.uniform.obj");


	int vertexCount = objModel.GetVertexCount();
	std::cout << "Loaded " << vertexCount << " vertices." << std::endl;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// �������� VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//������
	std::vector<float> vertices = objModel.GetVertexData();

	
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL setup error: " << err << std::endl;
	}

	// ���ض������ݵ� VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// ���ö�������
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ��������
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ��������������������������������������������������������������ͼ���֡�������������������������������������������������������������������
  

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Model model("models/sword/scene.gltf");

	//����������
	while (!glfwWindowShouldClose(window))
	{


		glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

	
		if (FillMode) {
			// ���ģʽ��Ⱦ
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			shaderProgram.Activate();
			model.Draw(shaderProgram, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		if (EdgeMode) {
			// �߿�ģʽ��Ⱦ
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			fixShader.Activate();
			model.Draw(fixShader, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		if (PointMode) {
			// ��ģʽ��Ⱦ
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			fixShader.Activate();
			model.Draw(fixShader, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		// �����ɫ������
		glUseProgram(0);
		
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
		//imgui���Ʋ���
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//���ڴ���
		static GLfloat currentColor[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::Begin("OBJ Render");
		// ��Եģʽ���
		if (ImGui::Checkbox("Edge Mode", &EdgeMode)) {
			std::cout << "Edge Mode changed to " << (EdgeMode ? "true" : "false") << std::endl;
		}

		// ��ģʽ���
		if (ImGui::Checkbox("Point Mode", &PointMode)) {
			std::cout << "Point Mode changed to " << (PointMode ? "true" : "false") << std::endl;
		}

		// ���ģʽ���
		if (ImGui::Checkbox("Fill Mode", &FillMode)) {
			std::cout << "Fill Mode changed to " << (FillMode ? "true" : "false") << std::endl;
		}

		ImGui::End();
	
		// imgui������Ⱦ
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemHovered())
		{
			camera.Inputs(window);
			camera.updateMatrix(45.0f, 0.1f, 100.0f);
		}



		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	shaderProgram.Delete();
	glfwDestroyWindow(window);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}