﻿#include "ParticleSystemEditor.h"

#include "ParticleSystem/ParticleSystem.h"

nzgdc_demo::ParticleSystemEditor::ParticleSystemEditor(const std::shared_ptr<ParticleSystem>& particleSystem)
    : m_particleSystem(particleSystem)
{
}

std::string nzgdc_demo::ParticleSystemEditor::GetWindowId() const
{
    return "Particle System Editor";
}

void nzgdc_demo::ParticleSystemEditor::RenderContent()
{
    if (ImGui::Button(m_particleSystem->IsPlaying() ? "Pause" : "Play"))
    {
        if (m_particleSystem->IsPlaying())
        {
            m_particleSystem->Pause();
        }
        else
        {
            m_particleSystem->Play();
        }
    }
    if (ImGui::Button("Restart"))
    {
        m_particleSystem->Stop();
        m_particleSystem->Play();
    }
    if (ImGui::Button("Stop"))
    {
        m_particleSystem->Stop();
    }
    ImGui::Text("Emitter");
    ImGui::LabelText("Playback Time", std::to_string(m_particleSystem->GetPlaybackTime()).c_str());
    ImGui::LabelText("Playing", m_particleSystem->IsPlaying() ? "true" : "false");
    ImGui::Checkbox("Looping", &m_particleSystem->GetData().Looping);
    ImGui::DragFloat("Duration", &m_particleSystem->GetData().Duration, 0.1f, 0.001f, 1000.0f);
    ImGui::DragInt("Max Particles", &m_particleSystem->GetData().MaxParticles, 1, 0, 10000);
    ImGui::DragFloat("Emission Rate", &m_particleSystem->GetData().EmissionRate, 0.1f, 0.0f, 10000.0f);
    ImGui::DragFloat("Radius", &m_particleSystem->GetData().Radius, 0.1f, 0.001f, 10000.0f);
    ImGui::DragInt("Arc", &m_particleSystem->GetData().Arc, 1, 0, 360);
    ImGui::DragFloat("Angular Velocity", &m_particleSystem->GetData().AngularVelocity);
    ImGui::DragFloat("Speed", &m_particleSystem->GetData().ParticleSpeed);
    ImGui::DragFloat3("Position", glm::value_ptr(m_particleSystem->GetData().Position));
    ImGui::DragFloat("Rotation", &m_particleSystem->GetData().Rotation, 1.0f, 0.0f, 360.0f);
    ImGui::Text("Particle");
    ImGui::LabelText("Num Particles", std::to_string(m_particleSystem->GetNumParticles()).c_str());
    ImGui::DragFloat("Life Time", &m_particleSystem->GetData().LifeTime, 0.001f, 1000.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_particleSystem->GetData().StartSize));
    ImGui::DragFloat3("Acceleration", glm::value_ptr(m_particleSystem->GetData().Acceleration));
    ImGui::Checkbox("Size Over Lifetime", &m_particleSystem->GetData().UseSizeOverLifeTime);
    if (m_particleSystem->GetData().UseSizeOverLifeTime)
    {
        ImGui::DragFloat3("End Scale", glm::value_ptr(m_particleSystem->GetData().EndSize));
    }
    ImGui::ColorEdit4("Color", glm::value_ptr(m_particleSystem->GetData().StartColor));
    ImGui::Checkbox("Color Over Lifetime", &m_particleSystem->GetData().UseColorOverLifeTime);
    if (m_particleSystem->GetData().UseColorOverLifeTime)
    {
        ImGui::ColorEdit4("End Color", glm::value_ptr(m_particleSystem->GetData().EndColor));
    }
}
