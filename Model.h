#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"
//����json����ģ���ļ�����
using json = nlohmann::json;

class Model
{

public:
//file����ָ���ļ�����
	Model(const char* file);
	void Draw(Shader& Shader, Camera& camera);

//�洢�Խ�������һ��JSON
private:
	const char* file;
	std::vector<unsigned char>data;
	json JSON;

	// ��������ͱ任
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;


	// �˲��ַ�ֹ��ȡ������ͼ
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;
	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json  accessor);
	std::vector<Texture> getTextures();
	//�洢�Ѿ���ȡ������
	void loadMesh(unsigned int indMesh);

	// ��ȡ�任
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	//��ȡ���㷨�ߺ������洢
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