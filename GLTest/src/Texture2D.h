#pragma once

#include <string>
#pragma once

#include <GL/glew.h>

enum class TextureUnit
{
	Unit0,
	Unit1,
	Unit2,
	Unit3,
};

class Texture2D
{
public:
	explicit Texture2D(const std::string& path);

	GLuint GetId() const
	{
		return m_Id;
	}

private:
	GLuint m_Id;
};
