#version 410 core

out vec4 FragColor;

struct Material 
{
	vec3 ambient;
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
in vec2 fragUV;

uniform Material material;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

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

vec3 phongPointLight(PointLight pl, vec3 norm, vec3 viewDir, vec3 diffTex, vec3 specTex)
{
	// Attenuation
	float dist = length(pointLight.position - fragPos);
	float attn = 1.0 / (pointLight.constant + (pointLight.linear * dist) + (pointLight.quadratic * (dist * dist)));

	// Ambient
	vec3 ambient = pl.ambient * material.ambient * attn;

	// Diffuse
	vec3 lightDir = normalize(pl.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pl.diffuse * (toonQuantize5(diff) * diffTex) * attn;

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pl.specular * (toonQuantize4(spec) * specTex.xyz) * attn;

	return (ambient + diffuse + specular);
}

vec3 phongDirectionLight(DirectionLight dl, vec3 norm, vec3 viewDir, vec3 diffTex, vec3 specTex)
{
	// Ambient
	vec3 ambient = dl.ambient * material.ambient;
	
	// Diffuse
	vec3 lightDir = normalize(-dl.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = dl.diffuse * (toonQuantize5(diff) * diffTex);

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = dl.specular * (toonQuantize4(spec) * specTex);

	return (ambient + diffuse + specular);
}

void main()
{
	int toonLevels = 5;

	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 diffTex = texture(diffuse0, fragUV).rgb;
	vec3 specTex = texture(specular0, fragUV).rgb;

	diffTex = mix(vec3(0.2), mix(vec3(0.5), mix(vec3(0.8), vec3(1.0), smoothstep(0.75, 0.85, diffTex)), smoothstep(0.55, 0.65, diffTex)), smoothstep(0.35, 0.45, diffTex));
	specTex = mix(vec3(0.2), mix(vec3(0.5), mix(vec3(0.8), vec3(1.0), smoothstep(0.75, 0.85, specTex)), smoothstep(0.55, 0.65, specTex)), smoothstep(0.35, 0.45, specTex));

	vec3 result = phongDirectionLight(directionLight, norm, viewDir, diffTex, specTex) + phongPointLight(pointLight, norm, viewDir, diffTex, specTex);

	// Edge Detection
	if (toonMode) {
		float edge = step(0.2, dot(norm, viewDir));
		FragColor = vec4(result * edge, 1.0f);
	} else {
		FragColor = vec4(result, 1.0);
	}
}

