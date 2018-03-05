#pragma once
#include <glm/detail/type_vec3.hpp>

class Light
{
public:
	enum class Type
	{
		Directional = 0,
		Point = 1,
		Flashlight = 2
	};

	Light(Type type) : type(type) {}
	Light(const Light&) = default;

	const Type type;

	float ambientStrength{0.1f};
	glm::vec3 ambient{1, 1, 1};
	glm::vec3 diffuse{1, 1, 1};
	glm::vec3 specular{1, 1, 1};
	
	float constant{0};
	float linear{0};
	float quadratic{0};

	float intensity{1};

	float cutoff;
	float cutoffDelta;

	void SetColor(const glm::vec3& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
	}

	void Setup(const std::shared_ptr<ShaderProgram>& shader, int index, const Transform& t)
	{
		std::string prefix = "lights[" + std::to_string(index) + "].";

		if (type == Type::Directional || type == Type::Flashlight)
		{
			shader->SetVec3(prefix + "dir", glm::mat3{t.GetMatrix()} * glm::vec3{0, 0, -1});
		}

		if (type == Type::Point || type == Type::Flashlight)
		{
			shader->SetVec3(prefix + "pos", t.GetPosition());
			shader->SetFloat(prefix + "constant", constant);
			shader->SetFloat(prefix + "linear", linear);
			shader->SetFloat(prefix + "quadratic", quadratic);
		}

		if (type == Type::Flashlight)
		{
			shader->SetFloat(prefix + "cutoffIn", glm::radians(cutoff));
			shader->SetFloat(prefix + "cutoffOut", glm::radians(cutoff + cutoffDelta));
		}
		
		shader->SetInt(prefix + "type", (int)type);
		shader->SetFloat(prefix + "ambientStrength", ambientStrength);
		shader->SetVec3(prefix + "ambient", ambient);
		shader->SetVec3(prefix + "diffuse", diffuse);
		shader->SetVec3(prefix + "specular", specular);
		shader->SetFloat(prefix + "intensity", intensity);
	}
};
