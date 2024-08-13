#include "CameraEditor.h"

#include "imgui.h"
#include "gtc/type_ptr.hpp"

nzgdc_demo::CameraEditor::CameraEditor(const std::shared_ptr<Camera>& camera)
    : m_camera(camera)
{
    m_data = m_camera->Data;
}

void nzgdc_demo::CameraEditor::RenderContent()
{
    ImGui::Begin(GetWindowId().c_str());
    bool edited{false};
    if (ImGui::DragFloat3("Position", glm::value_ptr(m_data.Position)))
    {
        edited = true;
    }
    // TODO: other camera data settings
    if (edited)
    {
        UpdateCameraSettings();
    }
    ImGui::End();
}

std::string nzgdc_demo::CameraEditor::GetWindowId() const
{
    return "Camera Editor";
}

void nzgdc_demo::CameraEditor::UpdateCameraSettings()
{
    m_camera->Data = m_data;
}
