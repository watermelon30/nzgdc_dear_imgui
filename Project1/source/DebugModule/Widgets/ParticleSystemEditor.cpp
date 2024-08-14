#include "ParticleSystemEditor.h"

#include "ParticleSystem.h"

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
    if (ImGui::Button("Play"))
    {
        m_particleSystem->Play();
    }
    if (ImGui::Button("Pause"))
    {
        m_particleSystem->Pause();
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
    ImGui::DragFloat("Duration", &m_particleSystem->GetData().Duration, 0.1f);
    ImGui::DragInt("Max Particles", &m_particleSystem->GetData().MaxParticles, 1, 0, 10000);
    ImGui::DragFloat("Emission Rate", &m_particleSystem->GetData().EmissionRate, 0.1f, 0.0f, 10000.0f);
    ImGui::DragInt("Arc", &m_particleSystem->GetData().Arc, 1, 0, 360);
    ImGui::DragFloat("Angular Velocity", &m_particleSystem->GetData().AngularVelocity);
    ImGui::DragFloat("Speed", &m_particleSystem->GetData().ParticleSpeed);
    ImGui::DragFloat3("Position", glm::value_ptr(m_particleSystem->GetData().Position));
    ImGui::Text("Particle");
    ImGui::LabelText("Num Particles", std::to_string(m_particleSystem->GetNumParticles()).c_str());
    ImGui::DragFloat("Life Time", &m_particleSystem->GetData().LifeTime);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_particleSystem->GetData().StartSize));
}
