#include "SceneManagerEditor.h"

#include <fstream>

#include "CameraEditor.h"
#include "ParticleSystemEditor.h"
#include "QuadEditor.h"

#include "json/writer.h"

#include "SceneManager/SceneManager.h"

nzgdc_demo::SceneManagerEditor::SceneManagerEditor(const std::shared_ptr<SceneManager>& sceneManager)
    : m_sceneManager(sceneManager)
{
    m_flags = ImGuiWindowFlags_MenuBar;

    m_camera = m_sceneManager->GetCamera();
    auto quadVector = m_sceneManager->GetQuadsVector();
    auto particleSystemVector = m_sceneManager->GetParticleSystemsVector();

    m_targetQuad = quadVector[0];
    m_targetQuadData = m_targetQuad->GetTransformData();
    m_targetParticleSystem = particleSystemVector[0];
}

void nzgdc_demo::SceneManagerEditor::RenderContent()
{
    std::string popupId;
    DrawMenuBar(popupId);
    if (ImGui::CollapsingHeader("Camera Editor"))
    {
        ImGui::PushID("Camera Editor");
        {
            CameraEditor::DrawCameraEditor(m_camera->Data);
        }
        ImGui::PopID();
    }

    if (ImGui::CollapsingHeader("Quad Editor"))
    {
        ImGui::PushID("Quad Editor");
        {
            ImGui::TextUnformatted("Target quad: TODO: Make a id system?!");
            if (QuadEditor::DrawQuadEditor(m_targetQuadData))
            {
                m_targetQuad->SetTransformData(m_targetQuadData);
            }
        }
        ImGui::PopID();
    }

    if (ImGui::CollapsingHeader("Particle System Editor"))
    {
        ImGui::PushID("Particle System Editor");
        {
            ImGui::TextUnformatted("Target Particle System: TODO: Make a id system?!");
            ParticleSystemEditor::DrawParticleSystemEditor(m_targetParticleSystem);
        }
        ImGui::PopID();
    }
}

std::string nzgdc_demo::SceneManagerEditor::GetWindowId() const
{
    return "Scene Manager";
}

void nzgdc_demo::SceneManagerEditor::DrawMenuBar(std::string& popupId)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Load"))
            {
                if (LoadFromJson())
                {
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

bool nzgdc_demo::SceneManagerEditor::SaveToJson()
{
    Json::Value jsonVal;
    
    for (auto& quad : m_sceneManager->GetQuadsVector())
    {
        jsonVal["Quads"].append(QuadEditor::Serialize(quad->GetTransformData()));
    }
    for (auto& particleSystem : m_sceneManager->GetParticleSystemsVector())
    {
        jsonVal["ParticleSystems"].append(ParticleSystemEditor::Serialize(particleSystem->GetData()));
    }

    std::ofstream file(SceneManager::settingsPath);
    if (!file.is_open())
    {
        // TODO: Log error
        return false;
    }
    
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(jsonVal, &file);
    return true;
}

bool nzgdc_demo::SceneManagerEditor::LoadFromJson()
{
    return false;
}
