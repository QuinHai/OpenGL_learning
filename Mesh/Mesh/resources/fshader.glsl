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
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
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
	vec3 norm = normalize(Normal);//物品面法线
	vec3 lightDir = normalize(light.position - FragPos);//片段指向光源
	vec3 viewDir = normalize(viewPos - FragPos);//片段指向摄影机
	vec3 reflectDir = reflect(-lightDir, norm);//反射光向量

	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	//diffuse
	float diffuseRadio = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseRadio * light.diffuse * texture(material.diffuse, TexCoords).rgb;
	
	//specular
	float specularRadio = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = specularRadio * light.specular * texture(material.specular, TexCoords).rgb;

	//spotlight(soft edges)
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	//attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}