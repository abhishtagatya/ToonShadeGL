#version 410 core

out vec4 FragColor;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct PhongVar
{
	vec3 ambient;
	vec3 specular;
};

in vec3 fragPos;
in vec3 fragNormal;

uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform bool toonMode;

float toonQuantize5(float value)
{
	if (value < 0.1) return (0.0);
	else if (value < 0.2) return (0.2);
	else if (value < 0.4) return (0.4);
	else if (value < 0.6) return (0.6);
	else if (value < 0.8) return (0.8);
	else return (1.0);
}

float toonQuantize4(float value)
{
	if (value < 0.1) return (0.0);
	else if (value < 0.4) return (0.4);
	else if (value < 0.8) return (0.6);
	else return (1.0);
}

vec3 phongPointLight(PointLight pl, vec3 norm, vec3 viewDir)
{
	// Attenuation
	float dist = length(pointLight.position - fragPos);
	float attn = 1.0 / (pointLight.constant + (pointLight.linear * dist) + (pointLight.quadratic * (dist * dist)));

	// Ambient
	vec3 ambient = pl.ambient * material.ambient * attn;

	// Diffuse
	vec3 lightDir = normalize(pl.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pl.diffuse * (toonQuantize5(diff) * material.diffuse) * attn;

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pl.specular * (toonQuantize4(spec) * material.specular) * attn;

	return (ambient + diffuse + specular);
}

vec3 phongDirectionLight(DirectionLight dl, vec3 norm, vec3 viewDir)
{
	// Ambient
	vec3 ambient = dl.ambient * material.ambient;
	
	// Diffuse
	vec3 lightDir = normalize(-dl.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = dl.diffuse * (toonQuantize5(diff) * material.diffuse);

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = dl.specular * (toonQuantize4(spec) * material.specular);

	return (ambient + diffuse + specular);
}

void main()
{
	int toonLevels = 5;

	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = phongDirectionLight(directionLight, norm, viewDir) + phongPointLight(pointLight, norm, viewDir);

	// Edge Detection
	if (toonMode) {
		float edge = step(0.2, dot(norm, viewDir));
		FragColor = vec4(result * edge, 1.0f);
	} else {
		FragColor = vec4(result, 1.0);
	}
}

