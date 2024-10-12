#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

struct DirectionalLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

// Manages Different Lighting Conditions and Control
// Proportional to "phong_light.frag" uniforms
class LightManager
{
public:
	PointLight pl;
	DirectionalLight dl;

	LightManager()
	{
		// Default Values
		this->pl = PointLight{};
		this->pl.position = glm::vec3(0.0f, 0.0f, -10.0f);
		this->pl.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		this->pl.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		this->pl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		this->pl.constant = 1.0f;
		this->pl.linear = 0.09f;
		this->pl.quadratic = 0.032f;

		this->dl = DirectionalLight{};
		this->dl.direction = glm::vec3(-1.0f, -1.0f, 3.0f);
		this->dl.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		this->dl.diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
		this->dl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void Use(Shader& shader) const 
	{
		shader.SetVec3("directionLight.direction", dl.direction);
		shader.SetVec3("directionLight.ambient", dl.ambient);
		shader.SetVec3("directionLight.diffuse", dl.diffuse);
		shader.SetVec3("directionLight.specular", dl.specular);

		shader.SetVec3("pointLight.position", pl.position);
		shader.SetVec3("pointLight.ambient", pl.ambient);
		shader.SetVec3("pointLight.diffuse", pl.diffuse);
		shader.SetVec3("pointLight.specular", pl.specular);
		shader.SetFloat("pointLight.constant", pl.constant);
		shader.SetFloat("pointLight.linear", pl.linear);
		shader.SetFloat("pointLight.quadratic", pl.quadratic);
	}

	~LightManager() 
	{
	}
};

