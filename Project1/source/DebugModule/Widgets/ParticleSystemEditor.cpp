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
    ImGui::Checkbox("Looping", &m_particleSystem->GetData().Looping);
    ImGui::DragInt("Arc", &m_particleSystem->GetData().Arc, 1, 0, 360);
}

void nzgdc_demo::ParticleSystemEditor::UpdateParticleSystemSettings()
{
}
