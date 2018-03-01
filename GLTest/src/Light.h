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
};
