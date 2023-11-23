#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"
//利用json解析模型文件数据
using json = nlohmann::json;

class Model
{

public:
//file用于指定文件名称
	Model(const char* file);
	void Draw(Shader& Shader, Camera& camera);

//存储自解向量和一个JSON
private:
	const char* file;
	std::vector<unsigned char>data;
	json JSON;

	// 所有网格和变换
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;


	// 此部分防止读取两次贴图
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;
	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json  accessor);
	std::vector<Texture> getTextures();
	//存储已经读取的网格
	void loadMesh(unsigned int indMesh);

	// 读取变换
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	//获取顶点法线和纹理，存储
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);


	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif