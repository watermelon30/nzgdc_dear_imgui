#include "ParticleSystemEditor.h"

#include <fstream>

#include "JsonHelper.h"

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
    DrawParticleSystemEditor(m_particleSystem);
}
void nzgdc_demo::ParticleSystemEditor::DrawParticleSystemEditor(std::shared_ptr<ParticleSystem>& particleSystem)
{
    ImGui::PushID("Button panel");
    {
        if (ImGui::Button(particleSystem->IsPlaying() ? "Pause" : "Play"))
        {
            if (particleSystem->IsPlaying())
            {
                particleSystem->Pause();
            }
            else
            {
                particleSystem->Play();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Restart"))
        {
            particleSystem->Stop();
            particleSystem->Play();
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            particleSystem->Stop();
        }
    }
    ImGui::PopID();

    ImGui::Separator();
    
    ImGui::PushID("Emitter");
    {
        ImGui::Text("Emitter");
        ImGui::LabelText("Playback Time", std::to_string(particleSystem->GetPlaybackTime()).c_str());
        ImGui::LabelText("Playing", particleSystem->IsPlaying() ? "true" : "false");
        ImGui::Checkbox("Looping", &particleSystem->GetData().Looping);
        ImGui::DragFloat("Duration", &particleSystem->GetData().Duration, 0.1f, 0.001f, 1000.0f);
        ImGui::DragInt("Max Particles", &particleSystem->GetData().MaxParticles, 1, 0, 10000);
        ImGui::DragFloat("Emission Rate", &particleSystem->GetData().EmissionRate, 0.1f, 0.0f, 10000.0f);
        ImGui::DragFloat("Radius", &particleSystem->GetData().Radius, 0.1f, 0.001f, 10000.0f);
        ImGui::DragInt("Arc", &particleSystem->GetData().Arc, 1, 0, 360);
        ImGui::DragFloat("Angular Velocity", &particleSystem->GetData().AngularVelocity);
        ImGui::DragFloat("Speed", &particleSystem->GetData().ParticleSpeed);
        ImGui::DragFloat3("Position", glm::value_ptr(particleSystem->GetData().Position));
        ImGui::DragFloat("Rotation", &particleSystem->GetData().Rotation, 1.0f, 0.0f, 360.0f);
    }
    ImGui::PopID();

    ImGui::Separator();

    ImGui::PushID("Particle");
    {
        ImGui::Text("Particle");
        ImGui::LabelText("Num Particles", std::to_string(particleSystem->GetNumParticles()).c_str());
        ImGui::DragFloat("Life Time", &particleSystem->GetData().LifeTime, 0.001f, 1000.0f);
        ImGui::DragFloat3("Scale", glm::value_ptr(particleSystem->GetData().StartSize));
        ImGui::DragFloat3("Acceleration", glm::value_ptr(particleSystem->GetData().Acceleration));
        ImGui::Checkbox("Size Over Lifetime", &particleSystem->GetData().UseSizeOverLifeTime);
        if (particleSystem->GetData().UseSizeOverLifeTime)
        {
            ImGui::DragFloat3("End Scale", glm::value_ptr(particleSystem->GetData().EndSize));
        }
        ImGui::ColorEdit4("Color", glm::value_ptr(particleSystem->GetData().StartColor));
        ImGui::Checkbox("Color Over Lifetime", &particleSystem->GetData().UseColorOverLifeTime);
        if (particleSystem->GetData().UseColorOverLifeTime)
        {
            ImGui::ColorEdit4("End Color", glm::value_ptr(particleSystem->GetData().EndColor));
        }
    }
    ImGui::PopID();
}
Json::Value nzgdc_demo::ParticleSystemEditor::Serialize(const ParticleSystemData& data)
{
    Json::Value outJson;
    outJson["looping"] = data.Looping;
    outJson["duration"] = data.Duration;
    outJson["max_particles"] = data.MaxParticles;
    outJson["emission_rate"] = data.EmissionRate;
    outJson["radius"] = data.Radius;
    outJson["arc"] = data.Arc;
    outJson["angular_velocity"] = data.AngularVelocity;
    outJson["particle_speed"] = data.ParticleSpeed;
    
    outJson["position_x"] = data.Position.x;
    outJson["position_y"] = data.Position.y;
    outJson["position_z"] = data.Position.z;

    outJson["rotation"] = data.Rotation;

    outJson["start_size_x"] = data.StartSize.x;
    outJson["start_size_y"] = data.StartSize.y;
    outJson["start_size_z"] = data.StartSize.z;

    outJson["lifetime"] = data.LifeTime;

    outJson["use_size_over_lifetime"] = data.UseSizeOverLifeTime;
    outJson["end_size_x"] = data.EndSize.x;
    outJson["end_size_y"] = data.EndSize.y;
    outJson["end_size_z"] = data.EndSize.z;

    outJson["acceleration_x"] = data.Acceleration.x;
    outJson["acceleration_y"] = data.Acceleration.y;
    outJson["acceleration_z"] = data.Acceleration.z;

    outJson["start_color_r"] = data.StartColor.r;
    outJson["start_color_g"] = data.StartColor.g;
    outJson["start_color_b"] = data.StartColor.b;
    outJson["start_color_a"] = data.StartColor.a;

    outJson["use_color_over_lifetime"] = data.UseColorOverLifeTime;
    outJson["end_color_r"] = data.EndColor.r;
    outJson["end_color_g"] = data.EndColor.g;
    outJson["end_color_b"] = data.EndColor.b;
    outJson["end_color_a"] = data.EndColor.a;

    return outJson;
}

bool nzgdc_demo::ParticleSystemEditor::SaveToJson()
{
    const auto& particleData = m_particleSystem->GetData();
    return JsonHelper::SaveToJson(ParticleSystem::settingsPath, Serialize(particleData));
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
                if (JsonHelper::LoadJson(ParticleSystem::settingsPath, jsonVal))
                {
                    ParticleSystemData data;
                    ParticleSystem::ParseJson(jsonVal, data);
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
