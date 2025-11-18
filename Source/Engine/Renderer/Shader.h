#pragma once
#include "Resources/Resource.h"
#include "Math/Vector2.h"
#include <string>

#include <glad/glad.h> 

namespace neu {
	class Shader : public Resource {
	public:
		~Shader();

		bool Load(const std::string& filename, GLuint shaderType);
		void UpdateGUI() override;

	public:
		GLuint m_shader = 0;
	};
}