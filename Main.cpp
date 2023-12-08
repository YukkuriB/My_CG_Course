#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <glm/glm.hpp> // 基本的glm功能
#include <glm/gtc/matrix_transform.hpp> // 矩阵变换功能
#include <glm/gtc/type_ptr.hpp> // 类型转换功能
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"OBJModel.h"
#include"Model.h"





int main()
{
	glfwInit();
	// 申明opengl版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 
	// 设置窗口尺寸

	const unsigned int width = 2160;
	const unsigned int height = 2160;
	bool EdgeMode = true;
	bool PointMode = false;
	bool FillMode = true;

	// 创建窗口，如果窗口出问题就以failed结束

	GLFWwindow* window = glfwCreateWindow(width, height, "HelloWorld", NULL, NULL);
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
	glViewport(0, 0, width, height);

	
	
	Shader shaderProgram("default.vert", "default.frag");
	Shader fixShader("fixColor.vert", "fixColor.frag");
	//生成物体
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

	// 创建并绑定 VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//检查错误
	std::vector<float> vertices = objModel.GetVertexData();

	
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL setup error: " << err << std::endl;
	}

	// 加载顶点数据到 VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// 配置顶点属性
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 法线属性
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ——————————————————————————————贴图部分——————————————————————————————————
  

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Model model("models/sword/scene.gltf");

	//运行主函数
	while (!glfwWindowShouldClose(window))
	{


		glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

	
		if (FillMode) {
			// 填充模式渲染
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			shaderProgram.Activate();
			model.Draw(shaderProgram, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		if (EdgeMode) {
			// 线框模式渲染
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			fixShader.Activate();
			model.Draw(fixShader, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		if (PointMode) {
			// 点模式渲染
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			fixShader.Activate();
			model.Draw(fixShader, camera);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		// 清除着色器程序
		glUseProgram(0);
		
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
		//imgui控制部分
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//窗口创建
		static GLfloat currentColor[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::Begin("OBJ Render");
		// 边缘模式检查
		if (ImGui::Checkbox("Edge Mode", &EdgeMode)) {
			std::cout << "Edge Mode changed to " << (EdgeMode ? "true" : "false") << std::endl;
		}

		// 点模式检查
		if (ImGui::Checkbox("Point Mode", &PointMode)) {
			std::cout << "Point Mode changed to " << (PointMode ? "true" : "false") << std::endl;
		}

		// 填充模式检查
		if (ImGui::Checkbox("Fill Mode", &FillMode)) {
			std::cout << "Fill Mode changed to " << (FillMode ? "true" : "false") << std::endl;
		}

		ImGui::End();
	
		// imgui界面渲染
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