#pragma once

#include <string>

namespace nzgdc_demo
{
    class Shader
    {
    public:
        Shader(const std::string& _vertexPath, const std::string& _fragmentPath);

        void Use() const noexcept;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(Shader&&) = delete;

    private:
        unsigned int CompileShader(const unsigned int _shaderType, const std::string& _source);

    private:
        unsigned int m_ShaderId;
    };
}