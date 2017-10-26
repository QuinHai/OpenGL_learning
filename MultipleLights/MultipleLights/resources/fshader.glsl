#version 450 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;//∑¥…‰∂»
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//function
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos , vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{	
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0;i < 4;i ++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diffuseRadio = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularRadio = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseRadio * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specularRadio * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseRadio = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularRadio = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distances = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distances + light.quadratic * (distances * distances));    
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseRadio * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specularRadio * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseRadio = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularRadio = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distances = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distances + light.quadratic * (distances * distances));    

	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseRadio * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specularRadio * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);

}