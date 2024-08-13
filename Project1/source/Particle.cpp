#include "Particle.h"

nzgdc_demo::Particle::Particle(ParticleData data, const Shader& shader)
    : QuadMVP(shader)
    , m_data(data)
    , m_remainingLifeTime(data.LifeTime)
{
    m_transform.Position = m_data.StartPosition;
    m_transform.Scale = m_data.StartSize;
}

void nzgdc_demo::Particle::Update(float deltaTime)
{
    m_remainingLifeTime -= deltaTime;
    if (m_remainingLifeTime <= 0.0f)
    {
        if (m_onLifeTimeEnd != nullptr)
        {
            m_onLifeTimeEnd();
        }
    }
    m_transform.Position += glm::vec3(m_data.Velocity * deltaTime, 0.0f);
    m_transform.Rotation += glm::vec3(0.0f, 0.0f, m_data.AngularVelocity * deltaTime);
}

void nzgdc_demo::Particle::BindOnLifeTimeEnd(const std::function<void()>& onLifeTimeEnd)
{
    m_onLifeTimeEnd = onLifeTimeEnd;
}
