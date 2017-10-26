#version 450 core

struct Material {
	//vec3 ambient;
	//vec3 diffuse;
	//��������ɫ������������¶�������������ɫ
	sampler2D diffuse;
	sampler2D specular;
	float shininess;//�����
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
	vec3 lightDir = normalize(light.position - FragPos);//��Դ����-��Ʒ���� -����������
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//���������
	
	
	float diffuseStrength = max(dot(norm, lightDir), 0.0);//���������ǿ��
	float ambientStrength = 0.5f;//������ǿ��
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//���淴�����ǿ��
	float specularIntensity = 1.0f;//���淴��̶�
	
	vec3 diffuse = diffuseStrength * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;
	vec3 ambient = ambientStrength * vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	vec3 specular = specularIntensity * specularStrength * vec3(texture(material.specular, TexCoords)) * light.specular;

	vec3 result = diffuse + ambient + specular;
	
	FragColor = vec4(result, 1.0);
}