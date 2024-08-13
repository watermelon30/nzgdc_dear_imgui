#pragma once
#include <memory>
#include <vector>

#include "Particle.h"

namespace nzgdc_demo
{
    class Camera;
    class Shader;

    struct ParticleSystemData
    {
        bool Looping { true };
        float Duration { 3.0f };
        int MaxParticles { 100 };
        float EmissionRate { 5 }; // particles per second
        int Arc { 45 };    // 0 - 360
        float AngularVelocity { 0.0f };
        float ParticleSpeed { 100.0f };
    };
    
    class ParticleSystem
    {
    public:
        ParticleSystem(const ParticleSystemData& data, const Shader& shader, const std::shared_ptr<Camera>& camera);

        void Update(float deltaTime);
        void Render();

        void Play();
        void Pause();
        void Restart();
        void Stop();

        int GetNumParticles() const { return m_particles.size(); }
        float GetPlaybackTime() const { return m_timer; }
        bool IsPlaying() const { return m_isPlaying; }

    private:
        void EmitParticle();
        std::shared_ptr<Particle> GenerateParticle() const;
        glm::vec2 CalculateParticleVelocity() const;

    private:
        ParticleSystemData m_data;
        std::shared_ptr<Camera> m_camera;
        std::vector<std::shared_ptr<Particle>> m_particles;
        const Shader& m_shader;
        float m_timer { 0.0f };
        float m_emissionTimer { 0.0f };
        bool m_isPlaying { false };
    };
}
