#include "QuadEditor.h"

#include <vec3.hpp>

#include "imgui.h"
#include <Quad.h>

nzgdc_demo::QuadEditor::QuadEditor(const std::shared_ptr<Quad>& quad)
	: m_quad(quad) {
	m_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
	m_position[0] = quad->GetTransform().m_position[0];
	m_position[1] = quad->GetTransform().m_position[1];
	m_rotation = quad->GetTransform().m_rotation[2];
	m_scale[0] = quad->GetTransform().m_scale[0];
	m_scale[1] = quad->GetTransform().m_scale[1];
}

void nzgdc_demo::QuadEditor::UpdateQuadSettings() {
	m_quad->GetTransform().m_position = glm::vec3(m_position[0], m_position[1], 0.0f);
	m_quad->GetTransform().m_rotation = glm::vec3(0.0f, 0.0f, m_rotation);
	m_quad->GetTransform().m_scale = glm::vec3(m_scale[0], m_scale[1], 0.0f);
}

void nzgdc_demo::QuadEditor::RenderContent() {
	bool edited{false};
	if (ImGui::DragFloat2("Position", m_position)) {
		edited = true;
	}
	if (ImGui::DragFloat("Rotation", &m_rotation)) {
		edited = true;
	}
	if (ImGui::DragFloat2("Scale", m_scale)) {
		edited = true;
	}
	if (edited) {
		UpdateQuadSettings();
	}
}

std::string nzgdc_demo::QuadEditor::GetWindowId() const {
	return "Quad Editor";
}
