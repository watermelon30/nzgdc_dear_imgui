#pragma once

#include "Transform.h"

namespace nzgdc_demo
{
    class Shader;

    class Quad
    {
    public:
        explicit Quad(const Shader& shader);
        virtual ~Quad() = default;

        virtual void Render();
        Transform& GetTransform() noexcept { return m_transform; }

    protected:
        const Shader& m_shader;
        unsigned int m_vao, m_vbo, m_ebo;
        Transform m_transform;
    };
}
