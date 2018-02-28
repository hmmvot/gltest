#pragma once
#include <glm/detail/type_vec3.hpp>

class DirectionalLight
{
public:
	DirectionalLight() = default;
	DirectionalLight(const DirectionalLight&) = default;

	float ambient{0.1f};
	glm::vec3 color{1, 1, 1};
};
