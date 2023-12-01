#version 330 core
out vec4 FragColor;


flat in vec3 flatColor; // 接收 flat 限定符的颜色
void main()
{
   FragColor = vec4(flatColor, 1.0); // 使用 flat 颜色作为输出
}