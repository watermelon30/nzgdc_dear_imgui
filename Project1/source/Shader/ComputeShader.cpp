#include "ComputeShader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

nzgdc_demo::ComputeShader::ComputeShader(const std::string& _computePath)
{
	std::string computeCode{};
	std::ifstream cShaderFile{};

	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		cShaderFile.open(_computePath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << cShaderFile.rdbuf();
		cShaderFile.close();
		computeCode = vShaderStream.str();
	}
	catch (std::ifstream::failure& _e)
	{
		std::cerr << "ERROR: Failed to read shader file. Error:\n" << _e.what();
	}

	unsigned int compute = CompileShader(GL_VERTEX_SHADER, computeCode);

	m_ShaderId = glCreateProgram();
	glAttachShader(m_ShaderId, compute);
	glLinkProgram(m_ShaderId);
	glDeleteShader(compute);
}

void nzgdc_demo::ComputeShader::Use() const noexcept
{
	glUseProgram(m_ShaderId);
}

unsigned int nzgdc_demo::ComputeShader::CompileShader(const unsigned int _shaderType, const std::string& _source)
{
	unsigned int shader = glCreateShader(_shaderType);
	const char* src = _source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success{0};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);

		if (_shaderType == GL_VERTEX_SHADER)
		{
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		}
		else if (_shaderType == GL_FRAGMENT_SHADER)
		{
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		}
	}

	return shader;
}
