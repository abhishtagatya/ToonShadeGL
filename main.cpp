#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "shader.h"
#include "camera.h"
#include "light_manager.h"
#include "light_editor.h"

// Primitives
#include "cube.h"
#include "torus.h"
#include "cone.h"

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

double lastInputDelay = 0.0; // Initially 0, meaning no press
double inputDelay = 1.0;    // Delay of 1.0 seconds (500ms)

bool polygonMode = false;

void checkOpenGLError(const std::string& functionName) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		switch (error) {
		case GL_INVALID_ENUM:
			std::cerr << "OpenGL Error in " << functionName << ": GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cerr << "OpenGL Error in " << functionName << ": GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "OpenGL Error in " << functionName << ": GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "OpenGL Error in " << functionName << ": GL_OUT_OF_MEMORY" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cerr << "OpenGL Error in " << functionName << ": GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cerr << "OpenGL Error in " << functionName << ": GL_STACK_OVERFLOW" << std::endl;
			break;
		default:
			std::cerr << "OpenGL Error in " << functionName << ": Unknown error" << std::endl;
			break;
		}
	}
}

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);   // Cull back faces (this is default, but ensure it's set)
	glFrontFace(GL_CCW);   // Counter-clockwise is the front face (also default)

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");

	ImGui::StyleColorsDark();

	// DATA: START
	//Torus torus(0.5f, 1.0f, 16, 16);
	Cube lightCube;
	Cone cone(1.0f, 2.0f, 30);
	// DATA: END

	// Light
	LightManager lightManager;
	LightEditor lightEditor(lightManager);
	
	// Material
	glm::vec3 objectPositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, 10.0f),
		glm::vec3(-1.5f, -12.2f, -2.5f),
		glm::vec3(-9.8f, -2.0f, -8.3f),
		glm::vec3(2.4f, 10.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -15.5f),
		glm::vec3(16.3f, -2.0f, 2.5f),
		glm::vec3(8.5f,  6.2f, -18.5f),
		glm::vec3(-14.3f,  -4.0f, -1.5f)
	};

	glm::vec3 objectAmbient(1.0f, 0.5f, 0.31f);
	glm::vec3 objectDiffuse(1.0f, 0.5f, 0.31f);
	glm::vec3 objectSpecular(0.5f, 0.5f, 0.5f);
	float objectShininess = 32.0f;

	float outlineScale = 1.05f;

	Shader phongLightShader("Resources/Shaders/phong_light.vert", "Resources/Shaders/phong_light.frag");
	Shader outlineShader("Resources/Shaders/outline.vert", "Resources/Shaders/outline.frag");
	Shader defaultShader("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// GUI: START
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//std::cout << "Frame Time: " << deltaTime << " seconds" << std::endl;

		lightEditor.BuildGUI();

		ImGui::Begin("Material Editor");

		// Adjust material colors
		ImGui::Text("Material Colors");
		ImGui::ColorEdit3("Ambient", &cone.pm.matertial.ambient[0]);
		ImGui::ColorEdit3("Diffuse", &cone.pm.matertial.diffuse[0]);
		ImGui::ColorEdit3("Specular", &cone.pm.matertial.specular[0]);

		//// Adjust material shininess
		ImGui::Text("Material Shininess");
		ImGui::SliderFloat("Shininess", &cone.pm.matertial.shininess, 0.0f, 128.0f);

		ImGui::End();
		// GUI: END

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // Replace stencil value with reference value

		// Positional
		glm::mat4 proj = mainCamera.GetProjectionMatrix((float)SCREEN_WIDTH / SCREEN_HEIGHT);
		glm::mat4 view = mainCamera.GetViewMatrix();

		for (unsigned int i = 0; i < sizeof(objectPositions) / sizeof(glm::vec3); i++)
		{
			float angle = 20.0f * i;

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, objectPositions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(outlineScale));

			// 1st Pass : Phong Shading
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			phongLightShader.Use();

			phongLightShader.SetMat4("projection", proj);
			phongLightShader.SetMat4("view", view);
			phongLightShader.SetMat4("model", model);

			// Lighting
			lightManager.Use(phongLightShader);

			// Settings
			phongLightShader.SetVec3("viewPos", mainCamera.Position);
			phongLightShader.SetBool("toonMode", true);

			cone.Draw(phongLightShader);
			checkOpenGLError("Draw Torus");

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);  // Enable stencil writing
			glCullFace(GL_FRONT);

			outlineShader.Use();

			model = glm::scale(model, glm::vec3(outlineScale));
			outlineShader.SetMat4("projection", proj);
			outlineShader.SetMat4("view", view);
			outlineShader.SetMat4("model", model);

			cone.Draw(outlineShader);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			glCullFace(GL_BACK);

			glClear(GL_STENCIL_BUFFER_BIT);
		}

		// Light Object
		defaultShader.Use();
		defaultShader.SetMat4("projection", proj);
		defaultShader.SetMat4("view", view);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightManager.pl.position);
		defaultShader.SetMat4("model", lightModel);

		lightCube.DrawBasic();
		checkOpenGLError("Draw Cube");

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check for any errors in the rendering process
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "OpenGL Error: " << error << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lightCube.Delete();
	cone.Delete();

	phongLightShader.Delete();
	defaultShader.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

	double currentTime = glfwGetTime();
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (currentTime - lastInputDelay >= inputDelay)
		{
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) 
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				mainCamera.active = true;
			}
			else 
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mainCamera.active = false;
			}

			lastInputDelay = currentTime;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (currentTime - lastInputDelay >= inputDelay)
		{
			if (!polygonMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			polygonMode = !polygonMode;
			lastInputDelay = currentTime;
		}
	}

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
