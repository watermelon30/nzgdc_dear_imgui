#pragma once
#include <string>

namespace nzgdc_demo
{
	class ComputeShader
	{
	public:
		ComputeShader(const std::string& _computePath);

		void Use() const noexcept;
		unsigned int GetId() const { return m_ShaderId; }

		ComputeShader(const ComputeShader&) = delete;
		ComputeShader& operator=(const ComputeShader&) = delete;
		ComputeShader(ComputeShader&&) = delete;
		ComputeShader& operator=(ComputeShader&&) = delete;

	private:
		unsigned int CompileShader(const unsigned int _shaderType, const std::string& _source);

	private:
		unsigned int m_ShaderId;
	};
};