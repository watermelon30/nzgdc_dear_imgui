#include "SceneManagerEditor.h"

#include "CameraEditor.h"
#include "ParticleSystemEditor.h"
#include "QuadEditor.h"

#include "SceneManager/SceneManager.h"

nzgdc_demo::SceneManagerEditor::SceneManagerEditor(const std::shared_ptr<SceneManager>& sceneManager)
    : m_sceneManager(sceneManager)
{
    m_camera = m_sceneManager->GetCamera();
    auto quadVector = m_sceneManager->GetQuadsVector();
    auto particleSystemVector = m_sceneManager->GetParticleSystemsVector();

    m_targetQuad = quadVector[0];
    m_targetQuadData = m_targetQuad->GetTransformData();
    m_targetParticleSystem = particleSystemVector[0];
}

void nzgdc_demo::SceneManagerEditor::RenderContent()
{
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

void nzgdc_demo::SceneManagerEditor::DrawMenuBar(std::string popupId)
{

}

bool nzgdc_demo::SceneManagerEditor::SaveToJson()
{
    return false;
}

bool nzgdc_demo::SceneManagerEditor::LoadFromJson()
{
    return false;
}
