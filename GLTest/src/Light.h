#pragma once
#include <glm/detail/type_vec3.hpp>

class Light
{
public:
	enum class Type
	{
		Directional,
		Point
	};

	Light(Type type) : type(type) {}
	Light(const Light&) = default;

	const Type type;

	glm::vec3 direction{-1, -1, -1};

	float ambientStrength{0.1f};
	glm::vec3 ambient{1, 1, 1};
	glm::vec3 diffuse{1, 1, 1};
	glm::vec3 specular{1, 1, 1};
	
	float constant{0};
	float linear{0};
	float quadratic{0};

	void SetColor(const glm::vec3& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
	}

	void Setup(const std::shared_ptr<ShaderProgram>& shader, int index, const glm::vec3& pos)
	{
		std::string prefix;
		switch (type)
		{
		case Type::Directional:
			prefix = "dirLight.";
			break;
		case Type::Point:
			prefix = "pointLights[" + std::to_string(index) + "].";
			break;
		default:
			break;
		}
		
		shader->SetFloat(prefix + "ambientStrength", ambientStrength);
		shader->SetVec3(prefix + "ambient", ambient);
		shader->SetVec3(prefix + "diffuse", diffuse);
		shader->SetVec3(prefix + "specular", specular);
		shader->SetVec3(prefix + "pos", pos);
		shader->SetVec3(prefix + "dir", direction);
		shader->SetFloat(prefix + "constant", constant);
		shader->SetFloat(prefix + "linear", linear);
		shader->SetFloat(prefix + "quadratic", quadratic);
	}
};
