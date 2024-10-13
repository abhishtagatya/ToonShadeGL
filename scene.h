#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"

#include "light_manager.h"
#include "model.h"

class Scene
{
public:
	std::vector<Model> objects;
	std::vector<glm::vec3> transforms;

	LightManager& lightManager;

	Scene(LightManager& lm) : lightManager(lm) {}

	void Add(Model newObject, glm::vec3 newTransform)
	{
		this->objects.push_back(newObject);
		this->transforms.push_back(newTransform);
	}

	void Render(glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 camPos, Shader& objectShader, Shader& outlineShader) const
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		for (unsigned int i = 0; i < objects.size(); i++)
		{
			float angle = 20.0f * i;

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, this->transforms[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			// 1st Pass : Phong Shading
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			objectShader.Use();

			objectShader.SetMat4("projection", projMatrix);
			objectShader.SetMat4("view", viewMatrix);
			objectShader.SetMat4("model", model);

			objectShader.SetVec3("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			objectShader.SetFloat("material.shininess", 32.0f);

			lightManager.Use(objectShader);

			objectShader.SetVec3("viewPos", camPos);
			objectShader.SetBool("toonMode", true);

			static_cast<Model>(this->objects[i]).Draw(objectShader);

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);  // Enable stencil writing
			glCullFace(GL_FRONT);

			outlineShader.Use();

			model = glm::scale(model, glm::vec3(lightManager.outlineScale));
			outlineShader.SetMat4("projection", projMatrix);
			outlineShader.SetMat4("view", viewMatrix);
			outlineShader.SetMat4("model", model);

			static_cast<Model>(this->objects[i]).Draw();

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			glCullFace(GL_BACK);

			glClear(GL_STENCIL_BUFFER_BIT);
		}
	}
};

