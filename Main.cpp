#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"


#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>


#include"Mesh.h"





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
	float transVecX = 1.0f;
	float transVecY = 2.0f;
	float transVecZ = 3.0f;
	float ambientIntensity = 0.2f;
	float lightIntensity = 1.0f;
	float specularIntensity = 0.5f;
	glm::vec3 translationVector(transVecX, transVecY, transVecZ);

	Vertex vertices[] =
	{ //     坐标                         |              色彩          |        法向           |        贴图坐标       //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.5f), glm::vec2(1.0f, 0.0f)},
	Vertex{glm::vec3(0.0f, sqrt(2.0f),  0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f), glm::vec2(0.5f, 0.5f)}

	};
	//读取顺序

	GLuint indices[] = {
		// 底面
		0, 1, 2,
		0, 2, 3,

		// 侧面
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};
	//光源的坐标
	Vertex lightVertices[] =
	{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)} 
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

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

	Texture textures[]
	{
	Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};
	//生成物体
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh Pyramid(verts, ind, tex);
	//这一段创建一个光源
	Shader lightShader("light.vert", "light.frag");

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);
		//定义光色
		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


		glm::vec3 lightPos = glm::vec3(1.5f, 3.5f, 3.5f);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		glm::vec3 pyramidPos = glm::vec3(1.0f, 2.0f, 3.0f);
		glm::mat4 pyramidModel = glm::mat4(1.0f);
		pyramidModel = glm::translate(pyramidModel, pyramidPos);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm:: value_ptr(pyramidModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// ——————————————————————————————贴图部分——————————————————————————————————
  

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(1.0f, 2.0f, 3.0f));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//运行主函数
	while (!glfwWindowShouldClose(window))
	{


		glClearColor(ambientIntensity, ambientIntensity, ambientIntensity, ambientIntensity);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//imgui控制部分
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//窗口创建
		static GLfloat currentColor[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::Begin("Triangle Color Picker");
		ImGui::SliderFloat("Translate X", &transVecX, -10.0f, 10.0f);
		ImGui::SliderFloat("Translate Y", &transVecY, -10.0f, 10.0f);
		ImGui::SliderFloat("Translate Z", &transVecZ, -10.0f, 10.0f);
		ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("Light Intensity", &lightIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 1.0f);

		ImGui::End();
		translationVector = glm::vec3(transVecX, transVecY, transVecZ);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
		pyramidModel = translationMatrix; // 如果你还想应用其他变换（如旋转或缩放），你可以在这里组合它们
		shaderProgram.Activate();
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambientIntensity"), ambientIntensity);
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "lightIntensity"), lightIntensity);
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularIntensity"), specularIntensity);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));


		Pyramid.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error: " << err << std::endl;
		}

		
		
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
	lightShader.Delete();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}