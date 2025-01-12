﻿#pragma once
#include <functional>
#include <glm.hpp>

#include "QuadMVP.h"

#define GLM_ENABLE_EXPERIMENTAL

namespace nzgdc_demo
{
    struct ParticleData
    {
        float LifeTime { 5.0f };
        glm::vec3 StartPosition { 0.0f };
        glm::vec3 StartSize { 100.0f };
        glm::vec3 Velocity { 0.0f };
        float AngularVelocity { 0.0f };
        bool UseSizeOverLifeTime { false };
        glm::vec3 EndSize { 0.0f };
        glm::vec3 Acceleration { 0.0f };
        bool UseColorOverLifeTime { false };
        glm::vec4 StartColor { 1.0f };
        glm::vec4 EndColor { 1.0f };
    };
    
    class Particle : public QuadMVP
    {
    public:
        Particle(ParticleData data, const Shader& shader);

        void InitVertexAttributes();
        void Update(float deltaTime);
        float GetRemainingLifeTime() const { return m_elapsedTime; }
        void BindOnLifeTimeEnd(const std::function<void()>& onLifeTimeEnd);
        void SetUniforms();
        void SetElapsedTimeAttribute();
        
    private:
        std::function<void()> m_onLifeTimeEnd;
        
        ParticleData m_data;
        glm::vec4 m_color;
        glm::vec3 m_velocity;
        float m_elapsedTime;
        unsigned int m_elapsedTimeVBO;
        float m_elapsedTimeAttribute[4] { 0.0f };
    };
}
