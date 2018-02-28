#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "DirectionalLight.h"

class Object;
using ObjectRef = std::shared_ptr<Object>;

class Object
{
public:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	Transform transform;
	std::shared_ptr<DirectionalLight> directionalLight;

	static ObjectRef Create()
	{
		return std::make_shared<Object>();
	}
};
