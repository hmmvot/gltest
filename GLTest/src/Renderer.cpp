#include "Renderer.h"
#include "Camera.h"
#include <list>

void Renderer::Render(const Camera &camera, const std::vector<ObjectRef> &objects)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto projection = camera.GetProjectionMatrix();
	const auto view = camera.GetViewMatrix();

	std::list<ObjectRef> lights;
	for (const auto &obj : objects)
	{
		if (obj->light)
		{
			if (obj->light->type == Light::Type::Directional)
			{
				if (lights.size() > 0 && lights.front()->light->type == Light::Type::Directional)
				{
					std::cout << "Only one directional ligth on scene supported" << std::endl;
					continue;
				}
				lights.push_front(obj);
			}
			else if (obj->light->type == Light::Type::Point)
			{
				lights.push_back(obj);
			}
		}
	}

	for (const auto &obj : objects)
	{
		auto material = obj->material;
		auto mesh = obj->mesh;

		if (!mesh || !material)
		{
			continue;
		}

		auto shader = material->GetShader();

		shader->Use();

		shader->SetMat4("projection", projection);
		shader->SetMat4("view", view);
		shader->SetMat4("model", obj->transform.GetMatrix());
		shader->SetMat3("normalMatrix", obj->transform.GetNormalMatrix());

		shader->SetVec3("cameraPos", camera.GetPosition());

		shader->SetVec3("material.ambient", material->ambient);
		shader->SetVec3("material.diffuse", material->diffuse);
		shader->SetVec3("material.specular", material->specular);
		shader->SetFloat("material.shiness", material->shiness);
		
		if (obj->light)
		{
			shader->SetVec3("color", obj->light->ambient);
		}
		else
		{
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
		}
		
		if (obj->light || lights.size() == 0)
		{
			mesh->Draw();
		}
		else
		{
			for (auto light : lights)
			{
				shader->SetFloat("light.ambientStrength", light->light->ambientStrength);
				shader->SetVec3("light.ambient", light->light->ambient);
				shader->SetVec3("light.diffuse", light->light->diffuse);
				shader->SetVec3("light.specular", light->light->specular);
				shader->SetVec3("light.pos", light->transform.GetPosition());
				shader->SetFloat("light.constant", light->light->constant);
				shader->SetFloat("light.linear", light->light->linear);
				shader->SetFloat("light.quadratic", light->light->quadratic);

				bool directionalLight = light->light->type == Light::Type::Directional;
				bool pointLight = light->light->type == Light::Type::Point;
				
				shader->SetInt("DirectionalLight", directionalLight);
				shader->SetInt("PointLight", pointLight);

				mesh->Draw();
			}
		}
	}
}
