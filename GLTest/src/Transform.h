#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	glm::mat4 GetMatrix() const
	{
		UpdateTransform();
		return m_Matrix;
	}

	glm::mat3 GetNormalMatrix() const
	{
		UpdateNormalMatrix();
		return m_NormalMatrix;
	}

	glm::vec3 GetPosition() const
	{
		return m_Position;
	}

	void SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_TransformDirty = true;
		m_NormalMatrixDirty = true;
	}

	glm::vec3 GetRotation() const
	{
		return m_Rotation;
	}

	void SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		m_TransformDirty = true;
		m_NormalMatrixDirty = true;
	}

	glm::vec3 GetScale() const
	{
		return m_Scale;
	}

	void SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
		m_TransformDirty = true;
		m_NormalMatrixDirty = true;
	}

private:
	mutable glm::mat4 m_Matrix{1.0f};
	mutable glm::mat4 m_NormalMatrix{1.0f};

	glm::vec3 m_Position{0, 0, 0};
	glm::vec3 m_Rotation{0, 0, 0};
	glm::vec3 m_Scale{0, 0, 0};

	mutable bool m_TransformDirty{false};
	mutable bool m_NormalMatrixDirty{false};
	
	void UpdateTransform() const
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

		m_Matrix = t;

		m_TransformDirty = false;
	}

	void UpdateNormalMatrix() const
	{
		if (!m_NormalMatrixDirty)
		{
			return;
		}

		m_NormalMatrix = glm::mat3(glm::transpose(glm::inverse(GetMatrix())));

		m_NormalMatrixDirty = false;
	}
};
