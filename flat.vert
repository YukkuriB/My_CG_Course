#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float scale;
uniform mat4 rotationMatrix;
uniform mat4 modelMatrix;

flat out vec3 flatColor; // flat��ɫ�������ɫ
void main()
{
   gl_Position = modelMatrix * vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);

   flatColor = aColor; // ������ɫ�������в�ֵ
}