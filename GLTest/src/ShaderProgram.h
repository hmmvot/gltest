#pragma once

#include <GL/glew.h> 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


class ShaderProgram
{
public:
	// constructor reads and builds the shader
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	~ShaderProgram();

	// use/activate the shader
	void Use() const;

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec4(const std::string& name, const glm::vec4& vec) const;
	void SetVec4(const std::string& name, const glm::vec3& vec) const;
	void SetVec2(const std::string& name, const glm::vec2& vec) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
	GLuint m_Id;
};
