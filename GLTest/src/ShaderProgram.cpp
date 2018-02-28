#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	int success;
	char infoLog[512];

	// vertex Shader
	const auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// similiar for Fragment Shader
	const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	

	// shader Program
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertex);
	glAttachShader(m_Id, fragment);
	glLinkProgram(m_Id);
	// print linking errors if any
	glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Use() const
{
	glUseProgram(m_Id);
	SetInt("tex0", 0);
	SetInt("tex1", 1);
}

void ShaderProgram::SetBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_Id, name.c_str()), static_cast<int>(value));
}
void ShaderProgram::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}
void ShaderProgram::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
}
void ShaderProgram::SetVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z);
}

void ShaderProgram::SetVec2(const std::string& name, const glm::vec2& vec) const
{
	glUniform2f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y);
}

void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
