#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <GL/glew.h>
#include <memory>

struct Vertex
{
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv0;
	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uv3;
	glm::vec3 normal;
};

struct Triangle
{
	int v1, v2, v3;
};

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	void SetVertices(const std::initializer_list<glm::vec3>& vertices);

	template <class Iterator>
	void SetVertices(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].pos = *it;
		}
		m_Dirty = true;
	}

	void SetUv0(const std::initializer_list<glm::vec2>& uvs);

	template <class Iterator>
	void SetUv0(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].uv0 = *it;
		}
		m_Dirty = true;
	}

	void SetUv1(const std::initializer_list<glm::vec2>& uvs);

	template <class Iterator>
	void SetUv1(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].uv1 = *it;
		}
		m_Dirty = true;
	}

	void SetUv2(const std::initializer_list<glm::vec2>& uvs);

	template <class Iterator>
	void SetUv2(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].uv2 = *it;
		}
		m_Dirty = true;
	}

	void SetUv3(const std::initializer_list<glm::vec2>& uvs);

	template <class Iterator>
	void SetUv3(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].uv3 = *it;
		}
		m_Dirty = true;
	}

	void SetNormals(const std::initializer_list<glm::vec3>& normals);

	template <class Iterator>
	void SetNormals(Iterator begin, Iterator end)
	{
		Resize(end - begin);
		size_t index = 0;
		for (auto it = begin; it != end; ++it, ++index)
		{
			m_Vertices[index].normal = *it;
		}
		m_Dirty = true;
	}

	void SetTriangles(const std::initializer_list<Triangle>& triangles);

	template <class Iterator>
	void SetTriangles(Iterator begin, Iterator end)
	{
		m_Triangles = std::vector<Triangle>(begin, end);
		m_Dirty = true;
	}

	void Draw();

	static std::shared_ptr<Mesh> CreateCube();
	static std::shared_ptr<Mesh> CreatePlane();
	
private:
	GLuint m_VBO;
	GLuint m_VAO;
	GLuint m_EBO;

	std::vector<Vertex> m_Vertices;
	std::vector<Triangle> m_Triangles;

	bool m_Dirty;

	void Update();
	void Resize(int targetSize);
};
