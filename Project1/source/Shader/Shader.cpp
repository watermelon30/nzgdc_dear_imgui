#include "Shader/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

namespace nzgdc_demo
{
	Shader::Shader(const std::string& _vertexPath, const std::string& _fragmentPath)
	{
		std::string vertexCode{};
		std::string fragmentCode{};
		std::ifstream vShaderFile{};
		std::ifstream fShaderFile{};

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(_vertexPath);
			fShaderFile.open(_fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& _e)
		{
			std::cerr << "ERROR: Failed to read shader file. Error:\n" << _e.what();
		}

		unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexCode);
		unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

		m_ShaderId = glCreateProgram();
		glAttachShader(m_ShaderId, vertex);
		glAttachShader(m_ShaderId, fragment);
		glLinkProgram(m_ShaderId);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::Use() const noexcept
	{
		glUseProgram(m_ShaderId);
	}

	unsigned int Shader::CompileShader(const unsigned int _shaderType, const std::string& _source)
	{
		unsigned int shader = glCreateShader(_shaderType);
		const char* src = _source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success{ 0 };
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
}