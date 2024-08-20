#include "Particle.h"

#include "Shader.h"
#include "glad/glad.h"
#include "gtx/easing.hpp"

nzgdc_demo::Particle::Particle(ParticleData data, const Shader& shader)
    : QuadMVP(shader)
      , m_data(data)
      , m_elapsedTime(0.0f)
{
    InitVertexAttributes();
    m_transform.Position = m_data.StartPosition;
    m_transform.Scale = m_data.StartSize;
    m_velocity = m_data.Velocity;
    m_color = m_data.StartColor;
}

void nzgdc_demo::Particle::InitVertexAttributes()
{
    glBindVertexArray(m_vao);
    
    // Setup particle elapsed time
    glGenBuffers(1, &m_elapsedTimeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_elapsedTimeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_elapsedTimeAttribute), m_elapsedTimeAttribute, GL_DYNAMIC_DRAW); // GL_DYNAMIC_DRAW because it will be updated
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Reset buffer to 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set uniforms
    m_shader.Use();
    glUniform4fv(glGetUniformLocation(m_shader.GetId(), "startColor"), 1, glm::value_ptr(m_data.StartColor));
    glm::vec4 endColor = m_data.UseColorOverLifeTime ? m_data.EndColor : m_data.StartColor;
    glUniform4fv(glGetUniformLocation(m_shader.GetId(), "endColor"), 1, glm::value_ptr(endColor));
}

void nzgdc_demo::Particle::Update(float deltaTime)
{
    m_elapsedTime += deltaTime;
    SetElapsedTimeAttribute();
    if (m_elapsedTime >= m_data.LifeTime)
    {
        if (m_onLifeTimeEnd != nullptr)
        {
            m_onLifeTimeEnd();
        }
    }

    m_velocity += m_data.Acceleration * deltaTime;
    m_transform.Position += m_velocity * deltaTime;
    m_transform.Rotation += glm::vec3(0.0f, 0.0f, m_data.AngularVelocity * deltaTime);

    if (m_data.UseSizeOverLifeTime)
    {
        const auto normalizedTime = glm::min(m_elapsedTime / m_data.LifeTime, 1.0f);
        // TODO: can use different easing curves for interpolation
        const auto lerpValue = glm::linearInterpolation(normalizedTime);
        m_transform.Scale = glm::mix(m_data.StartSize, m_data.EndSize, lerpValue);
    }

    SetUniforms();

    glBindBuffer(GL_ARRAY_BUFFER, m_elapsedTimeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_elapsedTimeAttribute), m_elapsedTimeAttribute);
}

void nzgdc_demo::Particle::BindOnLifeTimeEnd(const std::function<void()>& onLifeTimeEnd)
{
    m_onLifeTimeEnd = onLifeTimeEnd;
}

void nzgdc_demo::Particle::SetUniforms()
{
    m_shader.Use();
    glUniform1f(glGetUniformLocation(m_shader.GetId(), "totalLifetime"), m_data.LifeTime);
    glUniform4fv(glGetUniformLocation(m_shader.GetId(), "startColor"), 1, glm::value_ptr(m_data.StartColor));
    glm::vec4 endColor = m_data.UseColorOverLifeTime ? m_data.EndColor : m_data.StartColor;
    glUniform4fv(glGetUniformLocation(m_shader.GetId(), "endColor"), 1, glm::value_ptr(endColor));
}

void nzgdc_demo::Particle::SetElapsedTimeAttribute()
{
    for (auto& attribute : m_elapsedTimeAttribute)
    {
        attribute = m_elapsedTime;
    }
}
