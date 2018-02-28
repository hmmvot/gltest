#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class Camera;

class Renderer
{
public:
	void Render(const Camera& camera, const std::vector<ObjectRef>& objects);
};
