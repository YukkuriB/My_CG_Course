#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;




uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
 // 使用固定颜色代替纹理
uniform vec3 fixedColor = vec3(1.0, 0.5, 0.2); // 比如橙色
vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
	float ambient = 0.20f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);


	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection),0.0f), 16);
	float specular = specAmount * specularLight;

	float diffuse = max(dot(normal, lightDirection), 0.0f);
	

    // 将固定颜色与光照效果相结合
    return vec4(fixedColor * (diffuse * inten + ambient) + specular * inten, 1.0) * lightColor;
}

vec4 direcLight()
{
	
	float ambient = 0.20f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));


	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection),0.0f), 16);
	float specular = specAmount * specularLight;

	float diffuse = max(dot(normal, lightDirection), 0.0f);
	return (vec4(fixedColor, 1.0) * lightColor * (diffuse + ambient) + texture(specular0, texCoord).r + specular) * lightColor ;
}

vec4 spotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;
	float ambient = 0.20f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);


	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection),0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);


	float diffuse = max(dot(normal, lightDirection), 0.0f);
	return (texture(diffuse0, texCoord) * lightColor * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
	vec3 normalColor = (Normal + 1.0) * 0.5; // 将法线映射到[0, 1]范围
    FragColor = direcLight(); // 用dirlight渲染
}