#include "PostProcessComponent.h"
#include "Engine.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace neu {
	FACTORY_REGISTER(PostProcessComponent)

		void PostProcessComponent::Update(float dt) {
		// Nothing yet; could add animation over time
	}

	void PostProcessComponent::Apply(Program& program) {
		program.SetUniform("u_parameters", (uint32_t)parameters);
		program.SetUniform("u_colorTint", colorTint);
		//program.SetUniform("u_time", Engine::Instance().GetTime());  // engine time
		program.SetUniform("u_blend", blend);                        // blend factor 0-1
	}

	void PostProcessComponent::Read(const serial_data_t& value) {
		Object::Read(value);
	}

	void PostProcessComponent::UpdateGUI() {
		uint32_t iparameters = (uint32_t)parameters;
		bool parameter = false;

		// GrayScale
		parameter = iparameters & (uint32_t)Parameters::GrayScale;
		if (ImGui::Checkbox("Gray Scale", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::GrayScale;
			else iparameters &= ~(uint32_t)Parameters::GrayScale;
		}

		// ColorTint
		parameter = iparameters & (uint32_t)Parameters::ColorTint;
		if (ImGui::Checkbox("Color Tint", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ColorTint;
			else iparameters &= ~(uint32_t)Parameters::ColorTint;
		}

		// ScanLine
		parameter = iparameters & (uint32_t)Parameters::ScanLine;
		if (ImGui::Checkbox("Scan Line", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ScanLine;
			else iparameters &= ~(uint32_t)Parameters::ScanLine;
		}

		// InvertColors
		parameter = iparameters & (uint32_t)Parameters::InvertColors;
		if (ImGui::Checkbox("Invert Colors", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::InvertColors;
			else iparameters &= ~(uint32_t)Parameters::InvertColors;
		}

		// Grain
		parameter = iparameters & (uint32_t)Parameters::Grain;
		if (ImGui::Checkbox("Grain", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Grain;
			else iparameters &= ~(uint32_t)Parameters::Grain;
		}

		// Commit back
		parameters = (Parameters)iparameters;

		// Color tint editor
		ImGui::ColorEdit3("Color Tint", glm::value_ptr(colorTint));

		// Blend slider 0-1
		ImGui::SliderFloat("Blend", &blend, 0.0f, 1.0f);
	}
}
