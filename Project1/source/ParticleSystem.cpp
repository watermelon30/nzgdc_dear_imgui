﻿#include "ParticleSystem.h"

#include "Camera.h"
#include "Particle.h"

nzgdc_demo::ParticleSystem::ParticleSystem(const ParticleSystemData& data, const Shader& shader, const std::shared_ptr<Camera>& camera)
    : m_data(data)
    , m_camera(camera)
    , m_shader(shader)
{
    std::srand(std::time(nullptr));
}

void nzgdc_demo::ParticleSystem::Update(float deltaTime)
{
    if (!m_isPlaying) return;

    std::erase_if(m_particles, [](const std::shared_ptr<Particle>& particle)
    {
        return particle->GetRemainingLifeTime() <= 0;
    });

    for (const auto& particle : m_particles)
    {
        if (particle == nullptr) continue; // kek
        particle->SetView(m_camera->GetView());
        particle->SetProjection(m_camera->GetProjection());
        particle->Update(deltaTime);
    }
    
    if (m_timer >= m_data.Duration)
    {
        if (!m_data.Looping) return;
        Restart();
    }

    m_timer += deltaTime;
    m_emissionTimer += deltaTime;

    if (m_emissionTimer >= 1.0f / m_data.EmissionRate)
    {
        EmitParticle();
        m_emissionTimer = 0.0f;
    }
}

void nzgdc_demo::ParticleSystem::Render()
{
    for (const auto& particle : m_particles)
    {
        particle->Render();
    }
}

void nzgdc_demo::ParticleSystem::Play()
{
    m_isPlaying = true;
    EmitParticle();
}

void nzgdc_demo::ParticleSystem::Pause()
{
    m_isPlaying = false;
}

void nzgdc_demo::ParticleSystem::Restart()
{
    m_isPlaying = true;
    m_timer = 0.0f;
}

void nzgdc_demo::ParticleSystem::Stop()
{
    m_isPlaying = false;
    m_timer = 0.0f;
    m_particles.clear();
}

void nzgdc_demo::ParticleSystem::EmitParticle()
{
    if (m_particles.size() >= m_data.MaxParticles) return;
    auto particle = GenerateParticle();
    particle->BindOnLifeTimeEnd([this, particle]()
    {
        m_particles.erase(std::find(m_particles.begin(), m_particles.end(), particle));
    });
    m_particles.emplace_back(particle);
}

std::shared_ptr<nzgdc_demo::Particle> nzgdc_demo::ParticleSystem::GenerateParticle() const
{
    ParticleData data;
    data.AngularVelocity = m_data.AngularVelocity;
    data.Velocity = CalculateParticleVelocity() * m_data.ParticleSpeed;
    data.StartPosition = m_data.Position;
    data.StartSize = m_data.StartSize;
    // TODO: use a pool?
    return std::make_shared<Particle>(data, m_shader);
}

glm::vec2 nzgdc_demo::ParticleSystem::CalculateParticleVelocity() const
{
    const auto randomDirection = static_cast<float>(std::rand() % (m_data.Arc + 1));
    return {glm::cos(glm::radians(randomDirection)), glm::sin(glm::radians(randomDirection))};
}