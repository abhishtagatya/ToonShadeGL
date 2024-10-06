#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

void framebufferSizeCB(GLFWwindow* window, int width, int height);
void mouseCB(GLFWwindow* window, double xpos, double ypos);
void scrollCB(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 960;
const unsigned int SCREEN_HEIGHT = 720;

Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;	// Set Center X
float lastY = SCREEN_HEIGHT / 2.0f;	// Set Center Y
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCB);
	glfwSetCursorPosCallback(window, mouseCB);
	glfwSetScrollCallback(window, scrollCB);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// DATA: START
	float cube[] = {
		// Front face
		-1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Bottom-left
		 1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Bottom-right
		 1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Top-right
		 1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Top-right
		-1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Top-left
		-1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,   // Bottom-left

		// Back face
		-1.0f, -1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Bottom-left
		 1.0f, -1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Bottom-right
		 1.0f,  1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Top-right
		 1.0f,  1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Top-right
		-1.0f,  1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Top-left
		-1.0f, -1.0f, -1.0f, 0.0f,  0.0f, -1.0f,   // Bottom-left

		// Left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,   // Top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,   // Top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,   // Bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,   // Bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,   // Bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,   // Top-right

		// Right face
		 1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f,   // Top-left
		 1.0f, -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,   // Bottom-right
		 1.0f,  1.0f, -1.0f, 1.0f,  0.0f,  0.0f,   // Top-right
		 1.0f, -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,   // Bottom-right
		 1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f,   // Top-left
		 1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  0.0f,   // Bottom-left

		 // Top face
		 -1.0f,  1.0f, -1.0f, 0.0f,  1.0f,  0.0f,   // Top-left
		  1.0f,  1.0f, -1.0f, 0.0f,  1.0f,  0.0f,   // Top-right
		  1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,   // Bottom-right
		  1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,   // Bottom-right
		 -1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,   // Bottom-left
		 -1.0f,  1.0f, -1.0f, 0.0f,  1.0f,  0.0f,   // Top-left

		 // Bottom face
		 -1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f,   // Top-left
		  1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f,   // Top-right
		  1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,   // Bottom-right
		  1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,   // Bottom-right
		 -1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,   // Bottom-left
		 -1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f    // Top-left
	};

	GLuint objectVAO, objectVBO;
	glGenVertexArrays(1, &objectVAO);
	glGenBuffers(1, &objectVBO);

	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	// DATA: END

	// Positions
	glm::vec3 lightPosition(2.0f, 5.0f, -5.0f);

	glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
	

	// Material
	/*glm::vec3 objectAmbient(0.25f, 0.25f, 0.25f);
	glm::vec3 objectDiffuse(0.4f, 0.4f, 0.4f);
	glm::vec3 objectSpecular(0.78f, 0.78f, 0.78f);
	float objectShininess = 0.6f;*/

	glm::vec3 objectAmbient(0.135, 0.2225f, 0.1575f);
	glm::vec3 objectDiffuse(0.54f, 0.9f, 0.63f);
	glm::vec3 objectSpecular(0.316228f, 0.316228f, 0.316228f);
	float objectShininess = 0.2f;

	Shader phongLightShader("Resources/Shaders/phong_light.vert", "Resources/Shaders/phong_light.frag");
	Shader defaultShader("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		phongLightShader.Use();

		// Positional
		glm::mat4 proj = mainCamera.GetProjectionMatrix((float)SCREEN_WIDTH / SCREEN_HEIGHT);
		glm::mat4 view = mainCamera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		phongLightShader.SetMat4("projection", proj);
		phongLightShader.SetMat4("view", view);
		phongLightShader.SetMat4("model", model);

		// Material
		phongLightShader.SetVec3("material.ambient", objectAmbient);
		phongLightShader.SetVec3("material.diffuse", objectDiffuse);
		phongLightShader.SetVec3("material.specular", objectSpecular);
		phongLightShader.SetFloat("material.shininess", objectShininess);

		// Lighting
		phongLightShader.SetVec3("light.position", lightPosition);
		phongLightShader.SetVec3("light.ambient", lightAmbient);
		phongLightShader.SetVec3("light.diffuse", lightDiffuse);
		phongLightShader.SetVec3("light.specular", lightSpecular);
		phongLightShader.SetVec3("viewPos", mainCamera.Position);
		phongLightShader.SetBool("toonMode", true);

		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Light Object
		defaultShader.Use();
		defaultShader.SetMat4("projection", proj);
		defaultShader.SetMat4("view", view);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPosition);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		defaultShader.SetMat4("model", lightModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &objectVBO);
	glDeleteBuffers(1, &lightVBO);

	phongLightShader.Delete();
	defaultShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(RIGHT, deltaTime);

}

void framebufferSizeCB(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCB(GLFWwindow* window, double dxpos, double dypos)
{
	float xpos = static_cast<float>(dxpos);
	float ypos = static_cast<float>(dypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	mainCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCB(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}
