#include "CameraEditor.h"

#include "imgui.h"
#include "gtc/type_ptr.hpp"

nzgdc_demo::CameraEditor::CameraEditor(const std::shared_ptr<Camera>& camera)
    : m_camera(camera)
{
    m_data = m_camera->Data;
}

void nzgdc_demo::CameraEditor::Render()
{
    ImGui::Begin("Camera Editor");
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

void nzgdc_demo::CameraEditor::UpdateCameraSettings()
{
    m_camera->Data = m_data;
}
