#include "Renderer.h"
#include "Camera.h"

void Renderer::Render(const Camera &camera, const std::vector<std::shared_ptr<Object>> &objects)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto projection = camera.GetProjectionMatrix();
	const auto view = camera.GetViewMatrix();

	for (const auto &obj : objects)
	{
		auto material = obj->GetMaterial();
		auto mesh = obj->GetMesh();

		if (!mesh || !material)
		{
			continue;
		}

		auto shader = material->GetShader();

		shader->Use();

		shader->SetMat4("projection", projection);
		shader->SetMat4("view", view);
		shader->SetMat4("model", obj->GetTransform());
		shader->SetVec4("color", material->color);

		shader->SetFloat("textures[0].intensity", material->mainTex.intensity);
		shader->SetVec2("textures[0].scale", material->mainTex.scale);
		shader->SetVec2("textures[0].shift", material->mainTex.GetShift());
		
		shader->SetFloat("textures[1].intensity", material->tex1.intensity);
		shader->SetVec2("textures[1].scale", material->tex1.scale);
		shader->SetVec2("textures[1].shift", material->tex1.GetShift());
		
		shader->SetFloat("textures[2].intensity", material->tex2.intensity);
		shader->SetVec2("textures[2].scale", material->tex2.scale);
		shader->SetVec2("textures[2].shift", material->tex2.GetShift());
		
		shader->SetFloat("textures[3].intensity", material->tex3.intensity);
		shader->SetVec2("textures[3].scale", material->tex3.scale);
		shader->SetVec2("textures[3].shift", material->tex3.GetShift());

		glActiveTexture(GL_TEXTURE0);
		GLuint id = material->mainTex.texture ? material->mainTex.texture->GetId() : 0;
		glBindTexture(GL_TEXTURE_2D, id);
		shader->SetInt("textures[0].id", 0);

		glActiveTexture(GL_TEXTURE1);
		id = material->tex1.texture ? material->tex1.texture->GetId() : 0;
		glBindTexture(GL_TEXTURE_2D, id);
		shader->SetInt("textures[1].id", 1);

		glActiveTexture(GL_TEXTURE2);
		id = material->tex2.texture ? material->tex2.texture->GetId() : 0;
		glBindTexture(GL_TEXTURE_2D, id);
		shader->SetInt("textures[2].id", 2);

		glActiveTexture(GL_TEXTURE3);
		id = material->tex3.texture ? material->tex3.texture->GetId() : 0;
		glBindTexture(GL_TEXTURE_2D, id);
		shader->SetInt("textures[3].id", 3);

		mesh->Draw();
	}
}
