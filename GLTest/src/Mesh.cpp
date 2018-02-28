#include "Mesh.h"

Mesh::Mesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::SetVertices(const std::initializer_list<glm::vec3> &vertices)
{
	SetVertices(std::begin(vertices), std::end(vertices));
}

void Mesh::SetUv0(const std::initializer_list<glm::vec2> &uvs)
{
	SetUv0(std::begin(uvs), std::end(uvs));
}

void Mesh::SetUv1(const std::initializer_list<glm::vec2> &uvs)
{
	SetUv1(std::begin(uvs), std::end(uvs));
}

void Mesh::SetUv2(const std::initializer_list<glm::vec2> &uvs)
{
	SetUv2(std::begin(uvs), std::end(uvs));
}

void Mesh::SetUv3(const std::initializer_list<glm::vec2> &uvs)
{
	SetUv3(std::begin(uvs), std::end(uvs));
}

void Mesh::SetNormals(const std::initializer_list<glm::vec3> &normals)
{
	SetNormals(std::begin(normals), std::end(normals));
}

void Mesh::SetTriangles(const std::initializer_list<Triangle> &triangles)
{
	SetTriangles(std::begin(triangles), std::end(triangles));
}

template <class T>
static void SetupAttributeArray(
	GLuint index,
	GLint size,
	GLenum type,
	GLboolean normalized,
	size_t &shift)
{
	glVertexAttribPointer(index, size, type, normalized, sizeof(Vertex), reinterpret_cast<GLvoid*>(shift));
	glEnableVertexAttribArray(index);

	shift += sizeof(T);
}

void Mesh::Update()
{
	if (!m_Dirty)
	{
		return;
	}

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Triangles.size() * sizeof(Triangle), m_Triangles.data(), GL_DYNAMIC_DRAW);

	size_t shift = 0;
	SetupAttributeArray<glm::vec3>(0, 3, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec4>(1, 4, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec2>(2, 2, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec2>(3, 2, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec2>(4, 2, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec2>(5, 2, GL_FLOAT, GL_FALSE, shift);
	SetupAttributeArray<glm::vec3>(6, 3, GL_FLOAT, GL_FALSE, shift);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	m_Dirty = false;
}

void Mesh::Resize(int targetSize)
{
	if (targetSize <= 0)
	{
		return;
	}
	const auto size = static_cast<size_t>(targetSize);
	if (m_Vertices.size() < size)
	{
		const auto oldSize = m_Vertices.size();
		m_Vertices.resize(size);
		for (auto i = oldSize; i < size; ++i)
		{
			m_Vertices[i].color = {1, 1, 1, 1};
		}
	}
}

void Mesh::Draw()
{
	Update();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Triangles.size() * 3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

std::shared_ptr<Mesh> Mesh::CreateCube()
{
	auto mesh = std::make_shared<Mesh>();
	mesh->SetVertices({
		{0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, // front
		{0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, // back

		{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}, // top
		{0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, // down

		{0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, // right
		{-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, // left
	});

	std::initializer_list<glm::vec2> uvs = {
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
	};
	mesh->SetUv0(uvs);
	mesh->SetUv1(uvs);
	mesh->SetUv2(uvs);
	mesh->SetUv3(uvs);
	mesh->SetTriangles({
		{0, 1, 3}, {1, 2, 3},
		{4, 5, 7}, {5, 6, 7},
		{8, 9, 11}, {9, 10, 11},
		{12, 13, 15}, {13, 14, 15},
		{16, 17, 19}, {17, 18, 19},
		{20, 21, 23}, {21, 22, 23},
	});
	mesh->SetNormals({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, // front
		{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, // back

		{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, // top
		{0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, // down

		{1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, // right
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, // left
	});
	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreatePlane()
{
	auto mesh = std::make_shared<Mesh>();
	mesh->SetVertices({
		{0.5f, 0.5f, 0}, {0.5f, -0.5f, 0}, {-0.5f, -0.5f, 0}, {-0.5f, 0.5f, 0}
	});
	mesh->SetUv0({
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
	});
	mesh->SetUv1({
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
	});
	mesh->SetUv2({
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
	});
	mesh->SetUv3({
		{1, 1}, {1, 0}, {0, 0}, {0, 1},
	});
	mesh->SetTriangles({
		{0, 1, 3}, {1, 2, 3},
	});
	mesh->SetNormals({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
	});
	return mesh;
}
