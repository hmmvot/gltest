#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"

class Object;
using ObjectRef = std::shared_ptr<Object>;

class Object
{
public:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	Transform transform;
	std::shared_ptr<Light> light;

	static ObjectRef Create()
	{
		return std::make_shared<Object>();
	}
};
