#include "LightComponent.h"

namespace neu {
	FACTORY_REGISTER(LightComponent)
		void LightComponent::Update(float dt)
	{
	}
	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view)
	{
		glm::vec3 position = glm::vec3(view * glm::vec4(owner->transform.position, 1));
		glm::vec3 direction = glm::mat3(view) * owner->transform.Forward();
		

		program.SetUniform(name + ".type", (int)(lightType));
		program.SetUniform(name + ".position", position);
		program.SetUniform(name + ".direction", direction);
		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".intensity", intensity);
		program.SetUniform(name + ".range", range);
		program.SetUniform(name + ".innerSpotAngle", glm::radians(innerSpotAngle));
		program.SetUniform(name + ".outerSpotAngle", glm::radians(outerSpotAngle));
	}
	void LightComponent::Read(const serial_data_t& value)
	{
		std::string type;
		SERIAL_READ_NAME(value, "lightType", type);
		if(equalsIgnoreCase(type, "Point"))
		
			lightType = LightType::Point;
		
		else if (equalsIgnoreCase(type, "Directional"))
		
			lightType = LightType::Directional;
		
		else if (equalsIgnoreCase(type, "Spot"))
		
			lightType = LightType::Spot;
		

		SERIAL_READ(value, color);
		SERIAL_READ(value, intensity);
		SERIAL_READ(value, range);
		SERIAL_READ(value, innerSpotAngle);
		SERIAL_READ(value, outerSpotAngle);
	}
	void LightComponent::UpdateGUI()
	{
		const char* types[] = { "Point", "Directional", "Spot" };
		ImGui::Combo("Type", (int*)&lightType, types, 3);

		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f);
		if (lightType == LightType::Directional) {
			ImGui::DragFloat("Range", &range, 0.1f, 0.0f);
		}
		if (lightType == LightType::Spot) {
			ImGui::DragFloat("Inner Cutoff", &innerSpotAngle, 0.1f, 0.0f, 90.0f);
			ImGui::DragFloat("Outer Cutoff", &outerSpotAngle, 0.1f, 0.0f, 90.0f);
		}
	}
}