#include "ParticleSystemEditor.h"

#include <fstream>

#include "json/writer.h"

#include "ParticleSystem/ParticleSystem.h"

nzgdc_demo::ParticleSystemEditor::ParticleSystemEditor(const std::shared_ptr<ParticleSystem>& particleSystem)
    : m_particleSystem(particleSystem)
{
    m_flags = ImGuiWindowFlags_MenuBar;

}

std::string nzgdc_demo::ParticleSystemEditor::GetWindowId() const
{
    return "Particle System Editor";
}

void nzgdc_demo::ParticleSystemEditor::RenderContent()
{
    std::string popupId;
    DrawMenuBar(popupId);
    
    ImGui::PushID("Button panel");
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

        ImGui::SameLine();
        if (ImGui::Button("Restart"))
        {
            m_particleSystem->Stop();
            m_particleSystem->Play();
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            m_particleSystem->Stop();
        }
    }
    ImGui::PopID();

    ImGui::Separator();
    
    ImGui::PushID("Emitter");
    {
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
    }
    ImGui::PopID();

    ImGui::Separator();

    ImGui::PushID("Particle");
    {
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
    ImGui::PopID();
}

bool nzgdc_demo::ParticleSystemEditor::SaveToJson()
{
    const auto& particleData = m_particleSystem->GetData();
    Json::Value newVal;

    newVal["looping"] = particleData.Looping;
    newVal["duration"] = particleData.Duration;
    newVal["max_particles"] = particleData.MaxParticles;
    newVal["emission_rate"] = particleData.EmissionRate;
    newVal["radius"] = particleData.Radius;
    newVal["arc"] = particleData.Arc;
    newVal["angular_velocity"] = particleData.AngularVelocity;
    newVal["particle_speed"] = particleData.ParticleSpeed;
    
    newVal["position_x"] = particleData.Position.x;
    newVal["position_y"] = particleData.Position.y;
    newVal["position_z"] = particleData.Position.z;

    newVal["rotation"] = particleData.Rotation;

    newVal["start_size_x"] = particleData.StartSize.x;
    newVal["start_size_y"] = particleData.StartSize.y;
    newVal["start_size_z"] = particleData.StartSize.z;

    newVal["lifetime"] = particleData.LifeTime;

    newVal["use_size_over_lifetime"] = particleData.UseSizeOverLifeTime;
    newVal["end_size_x"] = particleData.EndSize.x;
    newVal["end_size_y"] = particleData.EndSize.y;
    newVal["end_size_z"] = particleData.EndSize.z;

    newVal["acceleration_x"] = particleData.Acceleration.x;
    newVal["acceleration_y"] = particleData.Acceleration.y;
    newVal["acceleration_z"] = particleData.Acceleration.z;

    newVal["start_color_r"] = particleData.StartColor.r;
    newVal["start_color_g"] = particleData.StartColor.g;
    newVal["start_color_b"] = particleData.StartColor.b;
    newVal["start_color_a"] = particleData.StartColor.a;

    newVal["use_color_over_lifetime"] = particleData.UseColorOverLifeTime;
    newVal["end_color_r"] = particleData.EndColor.r;
    newVal["end_color_g"] = particleData.EndColor.g;
    newVal["end_color_b"] = particleData.EndColor.b;
    newVal["end_color_a"] = particleData.EndColor.a;

    std::ofstream file(ParticleSystem::settingsPath);
    if (!file.is_open())
    {
        // TODO: Log error
        return false;
    }
    
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(newVal, &file);
    return true;
}

void nzgdc_demo::ParticleSystemEditor::DrawMenuBar(std::string& popupId)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Load From Json"))
            {
                Json::Value jsonVal;
                if (m_particleSystem->LoadJson(jsonVal))
                {
                    ParticleSystemData data;
                    m_particleSystem->ParseJson(jsonVal, data);
                    m_particleSystem->SetData(data);
                }
            }
            if (ImGui::MenuItem("Save"))
            {
                SaveToJson();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}
