#version 410 core

out vec4 FragColor;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 fragNormal;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool toonMode;

void main()
{
	int toonLevels = 5;
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	if (toonMode)
	{	
		diff = floor(diff * toonLevels) / toonLevels;
	}

	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse;

	// Edge Detection
	if (toonMode) {
		float edge = step(0.05, dot(norm, lightDir));
		FragColor = vec4(result * edge, 1.0f);
	} else {
		FragColor = vec4(result, 1.0);
	}
}

