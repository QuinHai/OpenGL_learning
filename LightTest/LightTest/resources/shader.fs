#version 450 core

struct Material {
	//vec3 ambient;
	//vec3 diffuse;
	//环境光颜色几乎所有情况下都等于漫反射颜色
	sampler2D diffuse;
	sampler2D specular;
	float shininess;//反射度
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
};

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);//光源坐标-物品坐标 -》光线向量
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//反射光向量
	
	
	float diffuseStrength = max(dot(norm, lightDir), 0.0);//漫反射光线强度
	float ambientStrength = 0.5f;//环境光强度
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//镜面反射光线强度
	float specularIntensity = 1.0f;//镜面反射程度
	
	vec3 diffuse = diffuseStrength * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;
	vec3 ambient = ambientStrength * vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	vec3 specular = specularIntensity * specularStrength * vec3(texture(material.specular, TexCoords)) * light.specular;

	vec3 result = diffuse + ambient + specular;
	
	FragColor = vec4(result, 1.0);
}