#version 330 core
out vec4 FragColor;


flat in vec3 flatColor; // ���� flat �޶�������ɫ
void main()
{
   FragColor = vec4(flatColor, 1.0); // ʹ�� flat ��ɫ��Ϊ���
}