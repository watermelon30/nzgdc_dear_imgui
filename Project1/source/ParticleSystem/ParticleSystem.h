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
        float Radius { 100.0f };
        int Arc { 45 };    // 0 - 360
        float AngularVelocity { 0.0f };
        float ParticleSpeed { 100.0f };
        glm::vec3 Position { 0.0f };
        float Rotation { 0.0f };
        glm::vec3 StartSize { 100.0f };
        float LifeTime { 1.0f };
        bool UseSizeOverLifeTime { false };
        glm::vec3 EndSize { 100.0f };
        glm::vec3 Acceleration { 0.0f };
        glm::vec4 StartColor { 1.0f };
        bool UseColorOverLifeTime { false };
        glm::vec4 EndColor { 1.0f };
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

        [[nodiscard]] int GetNumParticles() const { return static_cast<int>(m_particles.size()); }
        [[nodiscard]] float GetPlaybackTime() const { return m_timer; }
        [[nodiscard]] bool IsPlaying() const { return m_isPlaying; }

        ParticleSystemData& GetData() { return m_data; }
        void SetData(const ParticleSystemData& newData);

        bool LoadJson(Json::Value& outData) const;
        static void ParseJson(const Json::Value& inJson, ParticleSystemData& outParticleSystemData);

    private:
        void LoadLocalData();

        void EmitParticle();
        std::shared_ptr<Particle> GenerateParticle() const;
        glm::vec2 CalculateParticleVelocity() const;

    public:
        inline static std::string settingsPath {"res/assets/ParticleSystem.json"}; 

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
