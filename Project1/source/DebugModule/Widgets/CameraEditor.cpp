#include "CameraEditor.h"

#include "imgui.h"
#include "gtc/type_ptr.hpp"

nzgdc_demo::CameraEditor::CameraEditor(const std::shared_ptr<Camera>& camera)
    : m_camera(camera)
{
    m_data = m_camera->Data;
    m_selectedProjectionIndex = static_cast<int>(m_camera->Data.Projection);
}

void nzgdc_demo::CameraEditor::RenderContent()
{
    ImGui::Begin(GetWindowId().c_str());
    bool edited{false};
    if (ImGui::DragFloat3("Position", glm::value_ptr(m_data.Position)))
    {
        edited = true;
    }
    if (ImGui::DragFloat("Pitch", &m_data.Pitch))
    {
        edited = true;
    }
    if (ImGui::DragFloat("Yaw", &m_data.Yaw))
    {
        edited = true;
    }
    if (ImGui::DragFloat("Near Plane", &m_data.NearPlane))
    {
        edited = true;
    }
    if (ImGui::DragFloat("Far Plane", &m_data.FarPlane))
    {
        edited = true;
    }

    m_currentSelectedProjection = m_projectionOptions[m_selectedProjectionIndex];
    if (ImGui::BeginCombo("Projection", m_currentSelectedProjection))
    {
        for (int i = 0; i < IM_ARRAYSIZE(m_projectionOptions); ++i)
        {
            const bool isSelected = i == m_selectedProjectionIndex;
            if (ImGui::Selectable(m_projectionOptions[i], isSelected))
            {
                m_selectedProjectionIndex = i;
                m_data.Projection = static_cast<ProjectionType>(m_selectedProjectionIndex);
                edited = true;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    
    if (static_cast<ProjectionType>(m_selectedProjectionIndex) == ProjectionType::Orthographic)
    {
        if (ImGui::DragFloat("Width", &m_data.Width))
        {
            edited = true;
        }
        if (ImGui::DragFloat("Height", &m_data.Height))
        {
            edited = true;
        }
    }
    if (static_cast<ProjectionType>(m_selectedProjectionIndex) == ProjectionType::Perspective)
    {
        if (ImGui::DragFloat("Field Of View", &m_data.FieldOfView))
        {
            edited = true;
        }
        if (ImGui::DragFloat("AspectRatio", &m_data.AspectRatio))
        {
            edited = true;
        }
    }
    
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
