#pragma once
#include <future>
#include <utility>
#include "ShaderProgram.h"
#include "Texture2D.h"

class TextureSettings
{
public:
	TextureSettings() = default;
	TextureSettings(const TextureSettings&) = default;

	std::shared_ptr<Texture2D> texture{nullptr};
	float intensity{1};
	glm::vec2 scale{1, 1};

	glm::vec2 GetShift() const
	{
		return m_Shift;
	}

	void SetShift(const glm::vec2 &shift)
	{
		float x = shift.x;
		float y = shift.y;
		if (x > 1.0f)
		{
			x -= 1.0f;
		}
		if (y > 1.0f)
		{
			y -= 1.0f;
		}
		m_Shift = glm::vec2(x, y);
	}

private:
	glm::vec2 m_Shift{0, 0};
};

class Material
{
public:
//	static const size_t MAX_TEXTURE_COUNT = 4;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiness;

	Material(std::shared_ptr<ShaderProgram> shaderProgram)
		: m_ShaderProgram(std::move(shaderProgram))
	{
		SetColor({1, 1, 1});
		shiness = 32;
	}
	~Material()
	{}

//	union
//	{
//		struct
//		{
			TextureSettings mainTex;
			TextureSettings tex1;
			TextureSettings tex2;
			TextureSettings tex3;
//			TextureSettings tex4;
//			TextureSettings tex5;
//			TextureSettings tex6;
//			TextureSettings tex7;
//		};
//		TextureSettings textures[MAX_TEXTURE_COUNT];
//	};

	std::shared_ptr<ShaderProgram> GetShader() const
	{
		return m_ShaderProgram;
	}

	void SetColor(const glm::vec3& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
	}

private:
	const std::shared_ptr<ShaderProgram> m_ShaderProgram;
};
