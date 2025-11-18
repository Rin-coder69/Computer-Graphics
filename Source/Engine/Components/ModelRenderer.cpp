#include "ModelRenderer.h"

namespace neu {
	FACTORY_REGISTER(ModelRenderer)

		void ModelRenderer::Update(float dt)
	{
	}

	void ModelRenderer::Draw(Renderer& renderer)
	{
		material->Bind();
		//material->program->SetUniform("test", 5);
		material->program->SetUniform("u_model", owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullFace);
		model->Draw(GL_TRIANGLES);

		if (model) {
			model->Draw(GL_TRIANGLES);
		}
	}

	void ModelRenderer::Read(const serial_data_t& value)
	{
		Object::Read(value);

		std::string modelName;
		SERIAL_READ_NAME(value, "model", modelName);

		model = Resources().Get<Model>(modelName);

		std::string materialName;
		SERIAL_READ_NAME(value, "material", materialName);

		material = Resources().Get<Material>(materialName);

		SERIAL_READ(value, enableDepth);

	}

	void ModelRenderer::UpdateGUI()
	{
		std::string text;

		text = (model) ? model->name : "none";
		
			ImGui::Text("Model: %s", text.c_str());
			Editor::GetDialogResource<Model>(model, "ModelDialog", "Open Model", "Model(*.obj;*.fbx;*.dae; *.3ds; *.glb){.obj, .fbx, .dae, .3ds, .glb},.*");
		
		
			text = (material) ? material->name : "none";
			ImGui::Text("Material: %s", text.c_str());
			Editor::GetDialogResource<Material>(material, "MaterialDialog", "Open Material", "Material(*.mat){.mat},.*");
		



	}
}
