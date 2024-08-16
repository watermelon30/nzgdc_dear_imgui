#include "Particle.h"

#include "gtx/easing.hpp"

nzgdc_demo::Particle::Particle(ParticleData data, const Shader& shader)
    : QuadMVP(shader)
    , m_data(data)
    , m_elapsedTime(0.0f)
{
    m_transform.Position = m_data.StartPosition;
    m_transform.Scale = m_data.StartSize;
}

void nzgdc_demo::Particle::Update(float deltaTime)
{
    m_elapsedTime += deltaTime;
    if (m_elapsedTime >= m_data.LifeTime)
    {
        if (m_onLifeTimeEnd != nullptr)
        {
            m_onLifeTimeEnd();
        }
    }
    m_transform.Position += glm::vec3(m_data.Velocity * deltaTime, 0.0f);
    m_transform.Rotation += glm::vec3(0.0f, 0.0f, m_data.AngularVelocity * deltaTime);

    if (m_data.UseSizeOverLifeTime)
    {
        const auto normalizedTime = m_elapsedTime / m_data.LifeTime;
        const auto lerpValue = glm::linearInterpolation(normalizedTime);
        m_transform.Scale = glm::mix(m_data.StartSize, m_data.EndSize, lerpValue);
    }
}

void nzgdc_demo::Particle::BindOnLifeTimeEnd(const std::function<void()>& onLifeTimeEnd)
{
    m_onLifeTimeEnd = onLifeTimeEnd;
}
