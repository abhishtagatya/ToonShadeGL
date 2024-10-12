#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "primitive_mesh.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	float outlineSize;
};

class PrimitiveModel : public PrimitiveMesh
{
public:
	float* data;
	size_t dataSize;

	unsigned int* indices;
	size_t indicesSize;

	Material matertial;

	PrimitiveModel(float* data, unsigned int* indices, size_t dataSize, size_t indicesSize) : PrimitiveMesh()
	{
		this->matertial = Material{};
		this->matertial.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
		this->matertial.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
		this->matertial.specular = glm::vec3(0.5f, 0.5f, 0.5f);
		this->matertial.shininess = 32.0f;
		this->matertial.outlineSize = 1.05f;

		this->data = data;
		this->indices = indices;
		this->dataSize = dataSize;
		this->indicesSize = indicesSize;

		// Very Demure Debugger
		/*for (int i = 0; i < dataSize; i++)
		{
			std::cout << i << data[i] << std::endl;
		}*/

		Link(data, indices, dataSize, indicesSize);
	}

	// Draw with Toon Shading (Shader + Outline) Pass
	void Draw(Shader& shader)
	{
		// Material
		shader.SetVec3("material.ambient", this->matertial.ambient);
		shader.SetVec3("material.diffuse", this->matertial.diffuse);
		shader.SetVec3("material.specular", this->matertial.specular);
		shader.SetFloat("material.shininess", this->matertial.shininess);

		DrawBasic();
	}

	void DrawBasic()
	{
		BindVAO();
		BindEBO();
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	}
};

