#pragma once

namespace neu {
	class LightComponent : public Component
	{
	public:
		enum class LightType
		{
			Point,
			Directional,
			Spot
		};

		


	public:
		CLASS_PROTOTYPE(LightComponent)

			void Update(float dt) override;
		void SetProgram(Program& program, const std::string& name, const glm::mat4& view);

		void Read(const serial_data_t& value) override;

	public:
		glm::vec3 color{ 1, 1, 1 };
		float intensity{ 1 };
		float range{ 20 };
		LightType lightType = LightType::Point; //0: point, 1: directional, 2: spot
		float outerSpotAngle = 30.0f;
		float innerSpotAngle;//for spot light
		// Inherited via GUI
		void UpdateGUI() override;
	};
}