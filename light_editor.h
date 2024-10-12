#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "light_manager.h"

class LightEditor
{
public:

	LightEditor(LightManager& lightManager) : lm(lightManager) {}

	void BuildGUI() const
	{
		ImGui::Begin("Light Editor");

		// Adjust point light position
		if (ImGui::CollapsingHeader("Point Light")) {
			// Add UI elements within the collapsible section
			ImGui::Text("Light Position");
			ImGui::SliderFloat3("Position", &lm.pl.position[0], -15.0f, 15.0f);

			// Adjust light colors
			ImGui::Text("Light Colors");
			ImGui::ColorEdit3("PL Ambient", &lm.pl.ambient[0]);
			ImGui::ColorEdit3("PL Diffuse", &lm.pl.diffuse[0]);
			ImGui::ColorEdit3("PL Specular", &lm.pl.specular[0]);
			ImGui::SliderFloat("PL Constant", &lm.pl.constant, 0.0f, 1.0f);
			ImGui::SliderFloat("PL Linear", &lm.pl.linear, 0.0f, 1.0f);
			ImGui::SliderFloat("PL Quadratic", &lm.pl.quadratic, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Directional Light")) {
			// Add UI elements within the collapsible section
			ImGui::Text("Light Direction");
			ImGui::SliderFloat3("DL Direction", &lm.dl.direction[0], -15.0f, 15.0f);

			// Adjust light colors
			ImGui::Text("Light Colors");
			ImGui::ColorEdit3("DL Ambient", &lm.dl.ambient[0]);
			ImGui::ColorEdit3("DL Diffuse", &lm.dl.diffuse[0]);
			ImGui::ColorEdit3("DL Specular", &lm.dl.specular[0]);
		}

		ImGui::End();
	}
private:
	LightManager& lm;  // Reference to allow modifications
};

