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

	float constant;
	float linear;
	float quadratic;
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

	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	if (toonMode)
	{	
		if (diff < 0.1) diff = 0.0f;
		else if (diff < 0.2) diff = 0.2f;
		else if (diff < 0.4) diff = 0.4f;
		else if (diff < 0.6) diff = 0.6f;
		else if (diff < 0.8) diff = 0.8f;
		else diff = 1.0f;
	}

	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	if (toonMode)
	{	
		if (spec < 0.5) spec = 0.0f;
		else spec = 1.0f;
	}

	vec3 specular = light.specular * (spec * material.specular);

	// Attenuation
	float dist = length(light.position - fragPos);
	float attn = 1.0 / (light.constant + (light.linear * dist) + (light.quadratic * (dist * dist)));

	ambient *= attn;
	diffuse *= attn;
	specular *= attn;

	vec3 result = ambient + diffuse + specular;

	// Edge Detection
	if (toonMode) {
		float edge = step(0.05, dot(norm, lightDir));
		FragColor = vec4(result * edge, 1.0f);
	} else {
		FragColor = vec4(result, 1.0);
	}
}

