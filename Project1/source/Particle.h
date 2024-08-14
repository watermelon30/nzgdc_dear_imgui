#pragma once
#include <functional>
#include <glm.hpp>

#include "QuadMVP.h"

namespace nzgdc_demo
{
    struct ParticleData
    {
        float LifeTime { 5.0f };
        glm::vec3 StartPosition { 0.0f };
        glm::vec3 StartSize { 100.0f };
        glm::vec2 Velocity { 0.0f };
        float AngularVelocity { 0.0f };
        glm::vec4 Color { 1.0f };
    };
    
    class Particle : public QuadMVP
    {
    public:
        Particle(ParticleData data, const Shader& shader);

        void Update(float deltaTime);
        float GetRemainingLifeTime() const { return m_remainingLifeTime; }
        void BindOnLifeTimeEnd(const std::function<void()>& onLifeTimeEnd);
        
    private:
        std::function<void()> m_onLifeTimeEnd;
        
        ParticleData m_data;
        float m_remainingLifeTime;
    };
}
