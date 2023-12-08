#include "OBJModel.h"
#include <iostream>
#include <fstream>
OBJModel::OBJModel() {

}

OBJModel::~OBJModel() {

}

void OBJModel::LoadFromFile(const char* fileName) {
	std::vector<Position> verticies;
	std::vector<Normal> normals;

	std::ifstream file(fileName);
	if (file)
	{
		char currentMtlName[100];
		std::string line;
		while (std::getline(file, line))
		{
			if (StartWith(line, "mtllib")) {
				char mtlFileName[100];
				(void)sscanf_s(line.c_str(), "mtllib %s", mtlFileName, static_cast<unsigned int>(_countof(mtlFileName) - 1));

				// ʹ�� sizeof(mtlFileName) - 1 �����ݻ�������С������һ���ַ����� '\0'
				LoadMaterialFile(mtlFileName);
			}
			if (StartWith(line, "v "))
			{
				Position Pos;
				sscanf_s(line.c_str(), "v %f %f %f", &Pos.y, &Pos.x, &Pos.z); // ������ Y �� Z
				//Pos.z = -Pos.z; // ��һ�з�תZ
				verticies.push_back(Pos);
				std::cout << "Vertex: " << Pos.x << ", " << Pos.y << ", " << Pos.z << std::endl;
			}
			if (StartWith(line, "vn ")) 
			{
				Normal n;
				sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
				normals.push_back(n);
				std::cout << "Normal: " << n.x << ", " << n.y << ", " << n.z << std::endl;
			}
			if (StartWith(line, "f ")) {
				int v1, v2, v3;
				int n1, n2, n3;
				if (sscanf_s(line.c_str(), "f %d %d %d", &v1, &v2, &v3) == 3) {
					// ������ֻ�ж�����������
					AddVertexData(v1, -1, currentMtlName, verticies, normals);
					AddVertexData(v2, -1, currentMtlName, verticies, normals);
					AddVertexData(v3, -1, currentMtlName, verticies, normals);
				}
				else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3) == 6) {
					// �������ж���ͷ�����������
					AddVertexData(v1, n1, currentMtlName, verticies, normals);
					AddVertexData(v2, n2, currentMtlName, verticies, normals);
					AddVertexData(v3, n3, currentMtlName, verticies, normals);
				}
				else {
					std::cerr << "Unsupported face format: " << line << std::endl;
				}
			}
		}
		std::cout << "Finished loading OBJ. Vertices: " << verticies.size() << ", Normals: " << normals.size() << std::endl;
	}
	else
	{
		std::cout << "OBJ file loading failed" << std::endl;
	}
}

std::vector<float> OBJModel::GetVertexData() {
	return mVertexData;
}

int OBJModel::GetVertexCount() {
	size_t count = mVertexData.size() / 9;
	if (count > static_cast<size_t>(std::numeric_limits<int>::max())) {
		// ����򱨸����
		std::cerr << "Warning: Vertex count exceeds the limit of int type." << std::endl;
		return std::numeric_limits<int>::max();
	}
	return static_cast<int>(count);
}
void OBJModel::LoadMaterialFile(const char* fileName) {
	std::ifstream file(fileName);
		if (file) 
		{
			std::string line;
			while (std::getline(file, line)) 
			{
				char mtlName[100] = {};
				if (StartWith(line, "newmtl"))
				{
					(void)sscanf_s(line.c_str(), "newmtl %s", mtlName, static_cast<unsigned int>(_countof(mtlName) - 1));


					mMaterialMap[mtlName] = Color();
				}
				if (StartWith(line, "kd"))
				{
					Color& color = mMaterialMap[mtlName];
					sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
				}
			}
		}
		else
		{
			std::cout << "Material file loading filed" << std::endl;
		}

}


bool OBJModel::StartWith(std::string& line, const char* text) {
	size_t textLen = strlen(text);
	if (line.size() < textLen) {
		return false;
	}
	for (size_t i = 0; i < textLen; i++)
	{
		if (line[i] == text[i]) continue;
		else return false;
		
	}
	return true;
}

void OBJModel::AddVertexData(int vIdx, int nIdx, const char* mtl,
	std::vector<Position>& vertices, std::vector<Normal>& normals) {

	if (static_cast<size_t>(vIdx) > 0 && static_cast<size_t>(vIdx) <= vertices.size()) {
		Position p = vertices[static_cast<size_t>(vIdx) - 1];
		Normal n = { 0.0f, 0.0f, 0.0f }; // Ĭ�Ϸ���

		// �������Ч�ķ�����������ʹ����
		if (static_cast<size_t>(nIdx) > 0 && static_cast<size_t>(nIdx) <= normals.size()) {
			n = normals[static_cast<size_t>(nIdx) - 1];
		}

		Color c = mMaterialMap[mtl];

		mVertexData.push_back(p.x);
		mVertexData.push_back(p.y);
		mVertexData.push_back(p.z);
		mVertexData.push_back(c.r);
		mVertexData.push_back(c.g);
		mVertexData.push_back(c.b);
		mVertexData.push_back(n.x);
		mVertexData.push_back(n.y);
		mVertexData.push_back(n.z);
	}
	else {
		std::cerr << "Invalid vertex index: vIdx=" << vIdx << std::endl;
	}
}