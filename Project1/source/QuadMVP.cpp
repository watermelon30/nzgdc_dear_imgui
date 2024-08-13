#include "QuadMVP.h"

#include "Shader.h"
#include "glad/glad.h"

void nzgdc_demo::QuadMVP::Render() const
{
    m_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "model"), 1, GL_FALSE, glm::value_ptr(m_transform.Get()));

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void nzgdc_demo::QuadMVP::SetView(const glm::mat4& view) const
{
    m_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void nzgdc_demo::QuadMVP::SetProjection(const glm::mat4& projection) const
{
    m_shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

