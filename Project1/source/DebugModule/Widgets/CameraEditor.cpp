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
    if (DrawCameraEditor(m_data))
    {
        UpdateCameraSettings();
    }
}

std::string nzgdc_demo::CameraEditor::GetWindowId() const
{
    return "Camera Editor";
}

void nzgdc_demo::CameraEditor::UpdateCameraSettings()
{
    m_camera->Data = m_data;
}

bool nzgdc_demo::CameraEditor::DrawCameraEditor(CameraData& m_data)
{
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

    int projectionIndex = static_cast<int>(m_data.Projection);

    if (ImGui::BeginCombo("Projection", projectionOptions[projectionIndex]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(projectionOptions); ++i)
        {
            const bool isSelected = i == projectionIndex;
            if (ImGui::Selectable(projectionOptions[i], isSelected))
            {
                projectionIndex = i;
                m_data.Projection = static_cast<ProjectionType>(projectionIndex);
                edited = true;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    
    if (static_cast<ProjectionType>(projectionIndex) == ProjectionType::Orthographic)
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
    if (static_cast<ProjectionType>(projectionIndex) == ProjectionType::Perspective)
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
    return edited;
}
