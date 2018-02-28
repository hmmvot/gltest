#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"

class Object
{
public:
	Object()
		: m_Mesh(nullptr),
		  m_Material(nullptr),
		  m_Transform(0),
		  m_Position(0, 0, 0),
		  m_Rotation(0, 0, 0),
		  m_Scale(1, 1, 1),
		  m_TransformDirty(true)
	{
	}

	std::shared_ptr<Mesh> GetMesh() const
	{
		return m_Mesh;
	}

	void SetMesh(const std::shared_ptr<Mesh>& mesh)
	{
		m_Mesh = mesh;
	}

	std::shared_ptr<Material> GetMaterial() const
	{
		return m_Material;
	}

	void SetMaterial(const std::shared_ptr<Material>& material)
	{
		m_Material = material;
	}

	glm::mat4 GetTransform() const
	{
		UpdateTransform();
		return m_Transform;
	}

	glm::vec3 GetPosition() const
	{
		return m_Position;
	}

	void SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_TransformDirty = true;
	}

	glm::vec3 GetRotation() const
	{
		return m_Rotation;
	}

	void SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		m_TransformDirty = true;
	}

	glm::vec3 GetScale() const
	{
		return m_Scale;
	}

	void SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
		m_TransformDirty = true;
	}

private:
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;

	mutable glm::mat4 m_Transform;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	mutable bool m_TransformDirty;

	void UpdateTransform() const;
};
