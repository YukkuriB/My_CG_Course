#include"shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	//������ɫ��
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//�����ӡid�Ա�鿴
	printf("Vertex Shader ID: %u\n", vertexShader);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	//Ƭ����ɫ��
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	printf("Fragment Shader ID: %u\n", fragmentShader);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	// ����shader��������shader
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// ɾ��shader��Ϊ�Ѿ��齨���
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::Activate()
{
	glUseProgram(ID);
 }

void Shader::Delete()
{
	glDeleteProgram(ID);
}