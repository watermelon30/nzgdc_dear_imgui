#include "Quad.h"
#include "Shader.h"
#include <glad/glad.h>

namespace nzgdc_demo
{
    Quad::Quad(const Shader& shader) :
        m_shader(shader)
    {
        constexpr float vertices[] =
        {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        constexpr unsigned int indices[] =
        {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Quad::Render()
    {
        m_shader.Use();

        glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "transform"), 1, GL_FALSE,
                           glm::value_ptr(m_transform.Get()));

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
