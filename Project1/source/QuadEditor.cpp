#include "QuadEditor.h"
#include "imgui.h"
#include "Quad.h"

nzgdc_demo::QuadEditor::QuadEditor(const std::shared_ptr<Quad>& quad)
    : m_quad(quad)
{
}

void nzgdc_demo::QuadEditor::UpdateQuadSettings()
{
    m_quad->GetTransform().Position = glm::vec3(m_position[0], m_position[1], 0.0f);
    m_quad->GetTransform().Rotation = glm::vec3(0.0f, 0.0f, m_rotation);
    m_quad->GetTransform().Scale = glm::vec3(m_scale[0], m_scale[1], 0.0f);
}

void nzgdc_demo::QuadEditor::Render()
{
    ImGui::Begin("Quad Editor");

    ImGui::InputFloat2("Position", m_position);
    ImGui::InputFloat("Rotation", &m_rotation);
    ImGui::InputFloat2("Scale", m_scale);
    ImGui::End();
}
