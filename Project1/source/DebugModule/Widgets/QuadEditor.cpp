#include "QuadEditor.h"

#include <fstream>
#include <vec3.hpp>

#include "imgui.h"
#include <Quad.h>

#include <json/reader.h>
#include <json/writer.h>

nzgdc_demo::QuadEditor::QuadEditor(const std::shared_ptr<Quad>& quad)
	: m_quad(quad), m_loadedJson(Json::nullValue) {
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
	std::string popupId;
	drawMenuBar(popupId);
	bool edited{ false };
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

void nzgdc_demo::QuadEditor::drawMenuBar(std::string popupId) {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Load")) {
				loadFromJson();
			}
			if (ImGui::MenuItem("Save")) {
				saveToJson();
			}
		}
	}
}

bool nzgdc_demo::QuadEditor::saveToJson()
{
	m_loadedJson.clear();
	m_loadedJson["scale_x"] = m_scale[0];
	m_loadedJson["scale_y"] = m_scale[1];

	m_loadedJson["rotation"] = m_rotation;

	m_loadedJson["position_x"] = m_position[0];
	m_loadedJson["position_y"] = m_position[1];

	std::ofstream file(saveSettingsPath);

	if (!file.is_open()) {
		// TODO: Log error
		return false;
	}
	Json::StyledStreamWriter writer;
	writer.write(file, m_loadedJson);
	return true;
}

bool nzgdc_demo::QuadEditor::loadFromJson()
{
	std::ifstream file(saveSettingsPath);

	if (!file.is_open()) {
		// TODO: Log error
		return false;
	}
	Json::Reader reader;

	if (!reader.parse(file, m_loadedJson)) {
		// TODO: Log error
		return false;
	}

	if (m_loadedJson["scale_x"].empty()) {
		m_scale[0] = m_loadedJson["scale_x"].asFloat();
	}
	if (m_loadedJson["scale_y"].empty()) {
		m_scale[1] = m_loadedJson["scale_y"].asFloat();
	}
	if (m_loadedJson["rotation"].empty()) {
		m_rotation = m_loadedJson["rotation"].asFloat();
	}
	if (m_loadedJson["position_x"].empty()) {
		m_position[0] = m_loadedJson["position_x"].asFloat();
	}
	if (m_loadedJson["position_y"].empty()) {
		m_position[1] = m_loadedJson["position_y"].asFloat();
	}
	return true;
}
