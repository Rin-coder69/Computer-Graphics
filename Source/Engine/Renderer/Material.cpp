#pragma once
#include "Material.h"
#include "Program.h"
#include "Resources/ResourceManager.h"

namespace neu {
	bool Material::Load(const std::string& filename) {
		// load material document
		serial::document_t document;
		if (!serial::Load(filename, document)) {
			LOG_WARNING("Could not load program file: {}", filename);
			return false;
		}
		//program
		std::string programName;
		SERIAL_READ_NAME(document, "program", programName);

		program = Resources().Get<Program>(programName);

		//texture
		std::string textureName;
		SERIAL_READ_NAME(document, "baseMap", textureName);

		if (!textureName.empty()) baseMap = Resources().Get<Texture>(textureName);

		//specular
		textureName.clear();
		SERIAL_READ_NAME(document, "specularMap", textureName);
		if (!textureName.empty()) specularMap = Resources().Get<Texture>(textureName);

		//emissiveMap
		textureName.clear();
		SERIAL_READ_NAME(document, "emissiveMap", textureName);
		if (!textureName.empty()) emissiveMap = Resources().Get<Texture>(textureName);

		//normals
		std::string normalName;
		textureName.clear();
		SERIAL_READ_NAME(document, "normalMap", textureName);
		if (!textureName.empty()) normalMap = Resources().Get<Texture>(textureName);

		//cubemap
		textureName.clear();
		std::string cubeMapName = "";
		SERIAL_READ_NAME(document, "cubeMap", textureName);
		if (!textureName.empty()) { 
			cubeMap = Resources().Get<CubeMap>(textureName); 
		}//error

		textureName.clear();
		//std::string shadowMapName = "";
		SERIAL_READ_NAME(document, "shadowMap", textureName);
		if (!textureName.empty()) {
		shadowMap = Resources().Get<Texture>(textureName);
		}


		//shininess
		SERIAL_READ(document, shininess);

		//tiling
		SERIAL_READ(document, tiling);

		//offset
		SERIAL_READ(document, offset);

		//color
		SERIAL_READ(document, baseColor);

		SERIAL_READ(document, emissiveColor);

		return true;
	}
	void Material::Bind() {
		parameters = Parameters::None;
		program->Use();

		if (baseMap) {
			baseMap->SetActive(GL_TEXTURE0);
			baseMap->Bind();
			program->SetUniform("u_baseMap", 0);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::BaseMap);
		}

		if (specularMap) { //<=make specular
			specularMap->SetActive(GL_TEXTURE1);
			specularMap->Bind();
			program->SetUniform("u_specularMap", 1);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::SpecularMap);
		}

		if (emissiveMap) {
			emissiveMap->SetActive(GL_TEXTURE2);
			emissiveMap->Bind();
			program->SetUniform("u_emissiveMap", 2);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::EmissiveMap);
		}

		if (normalMap) {
			normalMap->SetActive(GL_TEXTURE3);
			normalMap->Bind();
			program->SetUniform("u_normalMap", 3);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::NormalMap);
		}

		if (cubeMap) {
			cubeMap->SetActive(GL_TEXTURE4);
			cubeMap->Bind();
			program->SetUniform("u_cubeMap", 4);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::CubeMap);
		}
		if (shadowMap) {
			shadowMap->SetActive(GL_TEXTURE5);
			shadowMap->Bind();
			program->SetUniform("u_shadowMap", 5);
			parameters = (Parameters)((uint32_t)parameters | (uint32_t)Parameters::ShadowMap);
		}

		program->SetUniform("u_material.shininess", shininess);
		program->SetUniform("u_material.tiling", tiling);
		program->SetUniform("u_material.offset", offset);
		program->SetUniform("u_material.baseColor", baseColor);
		program->SetUniform("u_material.emissiveColor", emissiveColor);
		program->SetUniform("u_material.parameters",(uint32_t)parameters);
		program->SetUniform("u_ior", ior);
	}

	void Material::UpdateGUI()
	{
		if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Text("Name: %s", name.c_str());
			ImGui::Text("Shader: %s", program->name.c_str());

			ImGui::Separator();
			ImGui::ColorEdit3("Base Color", glm::value_ptr(baseColor));
			ImGui::Separator();
			if (baseMap) {
				ImGui::Text("BaseMap: %s", baseMap->name.c_str());
				Editor::ShowTexture(*baseMap, 64, 64);
				Editor::GetDialogResource<Texture>(baseMap, "BaseMapDialog", "Open texture", "Image(*.png;*.jpg; *.bmp; *.jpeg; *.tga){.png, .bmp, .jpeg, .jpg, .tga},.*");
			}
			if (specularMap) {
				ImGui::Text("specularMap: %s", specularMap->name.c_str());
				Editor::ShowTexture(*specularMap, 64, 64);
				Editor::GetDialogResource<Texture>(specularMap, "specularMapDialog", "Open texture", "Image(*.png;*.jpg; *.bmp; *.jpeg; *.tga){.png, .bmp, .jpeg, .jpg, .tga},.*");
			}

			if (emissiveMap) {
				ImGui::Text("emissiveMap: %s", emissiveMap->name.c_str());
				Editor::ShowTexture(*emissiveMap, 64, 64);
				Editor::GetDialogResource<Texture>(emissiveMap, "emissiveMapDialog", "Open texture", "Image(*.png;*.jpg; *.bmp; *.jpeg; *.tga){.png, .bmp, .jpeg, .jpg, .tga},.*");
			}

			if (normalMap) {
				ImGui::Text("emissiveMap: %s", normalMap->name.c_str());
				Editor::ShowTexture(*normalMap, 64, 64);
				Editor::GetDialogResource<Texture>(normalMap, "normalMapDialog", "Open texture", "Image(*.png;*.jpg; *.bmp; *.jpeg; *.tga){.png, .bmp, .jpeg, .jpg, .tga},.*");
			}
			if (cubeMap) { 
				if (emissiveMap) {
					ImGui::Text("cubeeMap: %s", cubeMap->name.c_str());
					Editor::ShowTexture(*cubeMap, 64, 64);
					Editor::GetDialogResource<Texture>(cubeMap, "cubeMapDialog", "Open texture", "Image(*.png;*.jpg; *.bmp; *.jpeg; *.tga){.png, .bmp, .jpeg, .jpg, .tga},.*");
				}
				ImGui::Text("cubeMap: %s", emissiveMap->name.c_str());
			}



			ImGui::ColorEdit3("Emissive Color", glm::value_ptr(emissiveColor));
			ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 256.0f);
			ImGui::DragFloat2("tiling", glm::value_ptr(tiling), 0.0);
			ImGui::DragFloat2("offset", glm::value_ptr(offset), 0.0);
		}
	}
}