#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

void Object::UpdateTransform() const
{
	if (!m_TransformDirty)
	{
		return;
	}

	glm::mat4 t{1.0f};

	t = glm::translate(t, m_Position);

	t = glm::rotate(t, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	t = glm::rotate(t, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	t = glm::rotate(t, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

	t = glm::scale(t, m_Scale);

	m_Transform = t;

	m_TransformDirty = false;
}
