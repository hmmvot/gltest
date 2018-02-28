#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	glm::vec3 m_Pos{0};
	glm::quat m_Orient{glm::mat4{1}};
	float m_Fov{0};
	float m_Aspect{0};
	float m_ZNear{0};
	float m_ZFar{0};

public:
	Camera() = default;
	Camera(const Camera &) = default;

	Camera& operator =(const Camera &) = default;

	const glm::vec3& GetPosition() const
	{
		return m_Pos;
	}

	const glm::quat& GetOrientation() const
	{
		return m_Orient;
	}

	void SetProjection(float fov, float aspect, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Aspect = aspect;
		m_ZNear = zNear;
		m_ZFar = zFar;
	}

	void SetPosition(const glm::vec3 &pos)
	{
		m_Pos = pos;
	}

	void SetOrientation(const glm::quat &orient)
	{
		m_Orient = orient;
	}

	glm::mat4 GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(m_Fov), m_Aspect, m_ZNear, m_ZFar);
	}

	glm::mat4 GetViewMatrix() const
	{
		const auto front = m_Orient * glm::vec3({0, 0, -1});
		return glm::lookAt(m_Pos, m_Pos + front, {0, 1, 0});
		//		return glm::translate(glm::mat4_cast(m_orient), m_pos);
	}

	void Translate(const glm::vec3 &v)
	{
		m_Pos += v * m_Orient;
	}

	void Translate(float x, float y, float z)
	{
		m_Pos += glm::vec3(x, y, z) * m_Orient;
	}

	void Rotate(float angle, const glm::vec3 &axis)
	{
		m_Orient *= glm::angleAxis(angle, axis * m_Orient);
	}

	void Rotate(float angle, float x, float y, float z)
	{
		m_Orient *= glm::angleAxis(glm::radians(angle), glm::vec3(x, y, z) * m_Orient);
	}

	void Yaw(float angle)
	{
		Rotate(angle, 0.0f, 1.0f, 0.0f);
	}

	void Pitch(float angle)
	{
		Rotate(angle, 1.0f, 0.0f, 0.0f);
	}

	void Roll(float angle)
	{
		Rotate(angle, 0.0f, 0.0f, 1.0f);
	}
};
